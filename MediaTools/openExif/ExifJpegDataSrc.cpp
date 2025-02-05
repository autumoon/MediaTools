/*
 * Copyright (c) 2000-2009, Eastman Kodak Company
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification,are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the 
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Eastman Kodak Company nor the names of its 
 *       contributors may be used to endorse or promote products derived from 
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 * Creation Date: 07/14/2001
 *
 * Original Author: 
 * George Sotak george.sotak@kodak.com 
 *
 * Contributor(s): 
 * FirstName LastName <email address>
 * FirstName LastName <email address>
 */ 

#include "ExifConf.h"

#ifndef HAVE_SWIO_JPEG

#ifndef OPENEXIF_NO_IJG

#include "ExifJpegExtn.h"
#include "OpenExifJpeg\OpenExif_jerror.h"
#include "string.h"

#define INPUT_BUF_SIZE  60000   /* choose a much larger in memory buffer for NIFTY*/


METHODDEF (void)
init_mem_source (oe_j_decompress_ptr cinfo)
{
  nif_src_ptr src = (nif_src_ptr) cinfo->src;

  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
  src->start_of_file = TRUE;
}

#ifdef macintosh
/* and a variation on the above for NIFty. */
METHODDEF (void)
init_ref_source (oe_j_decompress_ptr cinfo)
{
  nif_ref_src_ptr src = (nif_ref_src_ptr) cinfo->src;
 
  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
  src->start_of_file = TRUE;
}   
#endif


/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 */

METHODDEF (boolean)
fill_mem_input_buffer (oe_j_decompress_ptr cinfo)
{
    nif_src_ptr src = (nif_src_ptr)cinfo->src;
    size_t nbytes;
    
    if(src->bufSize > INPUT_BUF_SIZE)
	nbytes = INPUT_BUF_SIZE-50;
    else
	nbytes = src->bufSize;	
    
    if((src->bufSize - src->offset) < INPUT_BUF_SIZE)
        nbytes = src->bufSize - src->offset; 
    
    (void)memcpy(src->buffer, src->NIFbuffer+(src->offset), nbytes);
    
    src->offset +=  nbytes;
    
    src->pub.next_input_byte = src->buffer;
    src->pub.bytes_in_buffer = nbytes;
    src->start_of_file = FALSE;
    
    return TRUE;
}

#ifdef macintosh
METHODDEF (boolean)
fill_ref_input_buffer (oe_j_decompress_ptr cinfo)
{
  nif_ref_src_ptr src = (nif_ref_src_ptr) cinfo->src;
  size_t nbytes;
  nbytes = INPUT_BUF_SIZE;
  OSErr fsErr;

// do FSRead here
  	 
	fsErr = FSRead(src->refNum, (int*)&nbytes, src->buffer);

  if (nbytes <= 0) {
    if (src->start_of_file) /* Treat empty input file as fatal error */
      ERREXIT(cinfo, JERR_INPUT_EMPTY);
    WARNMS(cinfo, JWRN_JPEG_EOF);
    /* Insert a fake EOI marker */
    src->buffer[0] = (OE_JOCTET) 0xFF;
    src->buffer[1] = (OE_JOCTET) JPEG_EOI;
    nbytes = 2;
  }

  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = nbytes;
  src->start_of_file = FALSE;

  return TRUE;
}
#endif // macintosh


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */

METHODDEF(void)
skip_mem_input_data (oe_j_decompress_ptr cinfo, long num_bytes)
{
    nif_src_ptr src = (nif_src_ptr)cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (int) src->pub.bytes_in_buffer) {
      num_bytes -= (int) src->pub.bytes_in_buffer;
      (void) fill_mem_input_buffer(cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }    
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}

#ifdef macintosh
METHODDEF(void)
skip_ref_input_data (oe_j_decompress_ptr cinfo, int num_bytes)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;
 
  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (int) src->pub.bytes_in_buffer) {
      num_bytes -= (int) src->pub.bytes_in_buffer;
      (void) fill_ref_input_buffer(cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}
#endif



/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

METHODDEF(void)
term_source (oe_j_decompress_ptr cinfo)
{
  /* no work necessary here */
}

GLOBAL(void)
openexif_jpeg_mem_src (oe_j_decompress_ptr cinfo, OE_JOCTET *UserBuffer, int size)
{
  nif_src_ptr src;

  if (cinfo->src == NULL) 
  {     /* first time for this JPEG object? */
      cinfo->src = (struct openexif_jpeg_source_mgr *)
          (*cinfo->mem->alloc_small) ((oe_j_common_ptr) cinfo, OE_JPOOL_PERMANENT,
                                      SIZEOF(nif_source_mgr));

      src = (nif_src_ptr) cinfo->src;
      src->buffer = (OE_JOCTET *)
          (*cinfo->mem->alloc_small) ((oe_j_common_ptr) cinfo, OE_JPOOL_PERMANENT,
                                      (INPUT_BUF_SIZE) * SIZEOF(OE_JOCTET));
      
  }

  if( cinfo->client_data == NULL )
  {
      cinfo->client_data = (void *)
          (*cinfo->mem->alloc_small) ((oe_j_common_ptr) cinfo, OE_JPOOL_PERMANENT,
                                      SIZEOF(openexif_jpeg_mem_client));
  }
  

  src = (nif_src_ptr) cinfo->src;
  src->pub.init_source = init_mem_source;
  src->pub.fill_input_buffer = fill_mem_input_buffer;
  src->pub.skip_input_data = skip_mem_input_data;
  src->pub.term_source = term_source;
  src->NIFbuffer = UserBuffer;
  src->bufSize = size;
  src->offset = 0;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}

#ifdef macintosh
GLOBAL (void)
jpeg_ref_src (oe_j_decompress_ptr cinfo, short refNum )
{
  nif_ref_src_ptr src;
 
  /* The source object and input buffer are made permanent so that a series
   * of JPEG images can be read from the same file by calling jpeg_stdio_src
   * only before the first one.  (If we discarded the buffer at the end of
   * one image, we'd likely lose the start of the next one.)
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
  if (cinfo->src == NULL) { /* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((oe_j_common_ptr) cinfo, JPOOL_PERMANENT,
                  SIZEOF(nif_ref_source_mgr));
    src = (nif_ref_src_ptr) cinfo->src;
    src->buffer = (OE_JOCTET *)
      (*cinfo->mem->alloc_small) ((oe_j_common_ptr) cinfo, JPOOL_PERMANENT,
                  INPUT_BUF_SIZE * SIZEOF(OE_JOCTET));
  }

  src = (nif_ref_src_ptr) cinfo->src;
  src->pub.init_source = init_ref_source;
  src->pub.fill_input_buffer = fill_ref_input_buffer;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.skip_input_data = skip_ref_input_data;
  src->pub.term_source = term_source;
  src->refNum = refNum;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}
#endif

#endif

#endif

