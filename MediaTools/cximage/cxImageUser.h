#pragma once
#include <string>
#include "ximage.h"
#include "ximagif.h"
#include "xiofile.h"
#include "..\src\StdStrFile.h"
#include "..\src\ProgressInterface.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

#ifdef _DEBUG
#pragma comment(lib, "cximage//db//cximage.lib")
#pragma comment(lib, "cximage//db//jasper.lib")
#pragma comment(lib, "cximage//db//jbig.lib")
#pragma comment(lib, "cximage//db//jpeg.lib")
#pragma comment(lib, "cximage//db//libdcr.lib")
#pragma comment(lib, "cximage//db//libpsd.lib")
#pragma comment(lib, "cximage//db//mng.lib")
#pragma comment(lib, "cximage//db//png.lib")
#pragma comment(lib, "cximage//db//tiff.lib")
#pragma comment(lib, "cximage//db//zlib.lib")
#else
#pragma comment(lib, "cximage//cximage.lib") 
#pragma comment(lib, "cximage//jasper.lib")
#pragma comment(lib, "cximage//jbig.lib")
#pragma comment(lib, "cximage//jpeg.lib")
#pragma comment(lib, "cximage//libdcr.lib")
#pragma comment(lib, "cximage//libpsd.lib")
#pragma comment(lib, "cximage//mng.lib")
#pragma comment(lib, "cximage//png.lib")
#pragma comment(lib, "cximage//tiff.lib")
#pragma comment(lib, "cximage//zlib.lib")
#endif

class CxImageUser
{
public:
	CxImageUser();
	~CxImageUser();
	static bool GetExifTime(_tstring fileName, _tstring& strTime);
	static bool MergeImages(const std::vector<_tstring> vImgsToMerge, const _tstring& strDstFile, const bool& bVertically = false,
		const int& nLimiteLength = -1, const bool& bGrayScale = false, CProgressInterface* ppi = nullptr);
	//gif特殊处理
	static bool ResizeGif(const _tstring& strSrcFile, const _tstring& strDstFile, const int& nWidth, const int& nHeight,
		const bool& bGrayScale = false, CProgressInterface* ppi = nullptr);
	//同时具有转换格式和尺寸的功能
	static bool pic2pic(const _tstring& strSrcFile, const _tstring& strDstFile, const int& nWidth = -1, const int& nHeight = -1, const bool& bGrayScale = false);
	static int GetJpgQuality();
	static int SetJpgQuality(const int& nJpgQuality = 90);
	

private:
	static int m_nJpgQuality;
	//ico特殊处理
	static bool ico2pic(const _tstring& strSrcFile, const _tstring& strDstFile, const int& nWidth = -1, const int& nHeight = -1, const bool& bGrayScale = false);
	static bool GetIcoInfo(const _tstring& strSrcFile, int& nBigFrameIndex, int& nBigWidth, int& nBigHeight);
	static bool MergeImagesHorizontally(CxImage* img1, CxImage* img2, CxImage* imgDst);
	static bool MergeImagesVertially(CxImage* img1, CxImage* img2, CxImage* imgDst);

	static ENUM_CXIMAGE_FORMATS GetFormatBySuffix(const _tstring& strFileName);
	static int GetPropLength(const int& nOldLength1, const int& nOldLength2, const int& nNewLength);
};

inline int CxImageUser::GetJpgQuality()
{
	return m_nJpgQuality;
}

inline int CxImageUser::SetJpgQuality(const int& nJpgQuality /*= 90*/)
{
	if (nJpgQuality > 0 && nJpgQuality <= 100)
	{
		m_nJpgQuality = nJpgQuality;
	}

	return m_nJpgQuality;
}