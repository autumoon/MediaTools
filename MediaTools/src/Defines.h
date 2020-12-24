#pragma once

//author:autumoon
//2018.03.05

#define INI_CAT_INDEX		_T("CAT_INDEX")
#define INI_SEL_INDEX		_T("SEL_INDEX")
#define INI_BMP				_T("BMP")			
#define INI_JPG				_T("JPG")			
#define INI_PNG				_T("PNG")			
#define INI_GIF				_T("GIF")		
#define INI_SUFFIXS			_T("*.jpg|*.jpeg|*.png|*.bmp|*.gif|*.ico")

#define INI_CLASSIFY_FILES		_T("CLASSIFY_FILES")
#define INI_CF_REMEMBERDIR		_T("REMEMBERDIR")	
#define INI_CF_DIR_VALUE		_T("DIR_VALUE")	
#define INI_CF_INDEX			_T("INDEX")			
#define INI_CF_METHOD			_T("METHOD")			
#define INI_CF_CUSTOM			_T("CUSTOM")	

#define INI_RESIZE_PICS			_T("RESIZE_PICS")
#define INI_RP_REMEMBERDIR		_T("REMEMBERDIR")	
#define INI_RP_DIR_VALUE		_T("DIR_VALUE")	
#define INI_RP_PIC_SUFFIXS		_T("PIC_SUFFIXS")	
#define INI_RP_INDEX			_T("INDEX")		
#define INI_RP_WIDTH			_T("WIDTH")		
#define INI_RP_HEIGHT			_T("HEIGHT")		
#define INI_RP_KEEPWIDTH		_T("KEEPWIDTH")	
#define INI_RP_KEEPSCALE		_T("KEEPSCALE")	
#define INI_RP_KEEPHEIGHT		_T("KEEPHEIGHT")	
#define INI_RP_GRAYSCALE		_T("GRAYSCALE")	
#define INI_RP_THREAD_NUM		_T("THREAD_NUM")	
#define INI_RP_JPG_QUALITY		_T("JPG_QUALITY")
#define INI_RP_IGNORE_MIN		_T("IGNORE_MIN")

#define INI_CLONE_EXIF			_T("CLONE_EXIF")
#define INI_CE_INDEX			_T("INDEX")			
#define INI_CE_REMEMBERFILE		_T("REMEMBERFILE")	
#define INI_CE_FILE_VALUE		_T("FILE_VALUE")	
#define INI_CE_FILE_DST			_T("FILE_DST")		
#define INI_CE_DIR_VALUE		_T("DIR_VALUE")		
#define INI_CE_DIR_DST			_T("DIR_DST")		
#define INI_CE_PIC_SUFFIXS		_T("PIC_SUFFIXS")	
#define INI_CE_THREAD_NUM		_T("THREAD_NUM")	

#define INI_CONVERTOR			_T("CONVERTOR")
#define INI_CON_REMEMBERDIR		_T("REMEMBERDIR")	
#define INI_CON_DIR_VALUE		_T("DIR_VALUE")	
#define INI_CON_PIC_SUFFIXS		_T("PIC_SUFFIXS")		
#define INI_CON_INDEX			_T("INDEX")		
#define INI_CON_KEEP_MODIFYTIME	_T("KEEP_MODIFYTIME")
#define INI_CON_SET_PHOTOTIME	_T("SET_PHOTOTIME")
#define INI_CON_SET_GRAYSCALE	_T("SET_GRAYSCALE")
#define INI_CON_THREAD_NUM		_T("THREAD_NUM")	
#define INI_CON_JPG_QUALITY		_T("JPG_QUALITY")	

#define INI_MERGE_IMGS			_T("MERGE_IMGS")
#define INI_MI_REMEMBERDIR		_T("REMEMBERDIR")	
#define INI_MI_DIR_VALUE		_T("DIR_VALUE")	
#define INI_MI_PIC_SUFFIXS		_T("PIC_SUFFIXS")	
#define INI_MI_INDEX			_T("INDEX")		
#define INI_MI_WIDTH			_T("WIDTH")		
#define INI_MI_HEIGHT			_T("HEIGHT")		
#define INI_MI_KEEPWIDTH		_T("KEEPWIDTH")	
#define INI_MI_KEEPHEIGHT		_T("KEEPHEIGHT")	
#define INI_MI_GRAYSCALE		_T("GRAYSCALE")	
#define INI_MI_JPG_QUALITY		_T("JPG_QUALITY")	

#define DIALOG_ITEM_COUNT	50
#define IMGTOOL_CATE_INDEX	2
#define SECOND_INFO_INDEX	1

#define PROCESSING_INFO		_T("正在处理中，请稍后...")
#define BUSY_INFO_TEXT		_T("任务栏显示当前进度！")
#define OVER_INFO_TEXT		_T("OVER!")

#ifdef _UNICODE

#ifndef _tstring
#define _tstring std::wstring
#endif // _tstring

#else

#ifndef _tstring
#define _tstring std::string
#endif // _tstring

#endif
