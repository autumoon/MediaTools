#pragma once
#include ".\include\gdal.h"
#include ".\include\gdal_priv.h"
#include <io.h>

#ifdef _DEBUG
#pragma comment(lib, "gdal\\x64\\gdal_i.lib")
#else
#pragma comment(lib, "gdal\\x64\\gdal_i.lib")
#endif

//是否兼容ImageX
#define _ImageX

//为了兼容ImageX
#ifdef _ImageX
typedef long HRESULT;

#ifndef S_OK
#define S_OK                                   ((HRESULT)0L)
#endif

#ifndef S_FALSE
#define S_FALSE                                ((HRESULT)1L)
#endif
#endif // _ImageX

class CGdalUser
{
public:
	CGdalUser();
	~CGdalUser();

public:
	double m_LBX;
	double m_LBY;
	double m_RTX;
	double m_RTY;
	double m_lfGSD;

	GDALDataType m_nDataType;

	int m_nImgWidth;
	int m_nImgHeight;
	int m_nBandNum;
	int m_nBPB;
	int m_nBPP;

	std::string m_strFilePath;

public:
	//创建文件初始化
	bool InitializeCreate(const char* szFilePath, GDALDataType nDataType, const int& widthDst, const int& heightDst, const int& nBandNum);
	//读取文件初始化
	bool InitializeRead(const char* szFilePath, const bool& b16Trans8 = false);

	//已经准备好读取或者写入
	bool IsImgReady() { return m_poDataset != nullptr; };

	//左下角为起始点的
	bool ReadBandImage(unsigned char* pData, int nStartCol, int nStartRow, int nWidth, int nHeight, int nBufferWidth, int nBufferHeight, int nBandIndex);
	bool ReadImage(unsigned char* pData, int nStartCol, int nStartRow, int nWidth, int nHeight);
	bool ReadImage(unsigned char* pData, int nStartCol, int nStartRow, int nWidth, int nHeight, int nBandIndex)
	{return ReadBandImage(pData, nStartCol, nStartRow, nWidth, nHeight, nWidth, nHeight, nBandIndex);}
	bool WriteImage(unsigned char* pData, int nStartCol, int nStartRow, int nWidth, int nHeight);

	//部分读取和写入
	int SetGeoTransform(const int& widthDst, const int& heightDst, const double dLBX = 0.0, const double dLBY = 0.0, const double& dlfGSD = 1.0);
	int ReadImg(int nSrcLeft, int nSrcTop, int nSrcRight, int nSrcBottom, unsigned char* pBuf, int nBufWid, int nBufHeight, int nBandNum,
		int nDestLeft, int nDestTop, int nDestRight, int nDestBottom, int nSrcSkip, int nDestSkip);
	int WriteImg(int nSrcLeft, int nSrcTop, int nSrcRight, int nSrcBottom, unsigned char* pBuf, int nBufWid, int nBufHeight, int nBandNum,
		int nDestLeft, int nDestTop, int nDestRight, int nDestBottom, int nSrcSkip, int nDestSkip);

	int CalcGeoCoordinate(const int& nX, const int& nY, double& dX, double& dY, bool bLeftDown = false) const
	{dX = m_lfGSD * nX + m_LBX; if (bLeftDown)dY = m_lfGSD * nY + m_LBY; else dY = m_RTY - m_lfGSD * nY; return 0;}

	int CalcPixCoordinate(const double& dX, const double& dY, int& nX, int& nY, const bool& bLeftDown = false) const
	{nX = int((dX - m_LBX) / m_lfGSD + 0.5); if (bLeftDown)nY = int((dY - m_LBY) / m_lfGSD + 0.5); else nY = int((m_RTY - dY) / m_lfGSD + 0.5); return 0;}

	static bool ResampleImage(const char* szSrcFile, const char* szDstFile, const double& lfNewGSD = 1.0);

	//取得影像的部分区域，左下角为起点
	static bool PartImage(const char* szSrcFile, const char* szDstFile, int nStartCol, int nStartRow, int nEndCol, int nEndRow);

	//一次性全部读取，左上角为起点，内部分配内存，记得外部delete[]，否则内存泄露
	//static bool ReadImageGDAL(const char* pFilePath, unsigned char **pImageData, GDALDataType nDataType, int& widthDst, int& heightDst, int& nBandNum);
	//一次性全部写入，左上角为起点，内部只读内存，记得外部delete[]，否则内存泄露
	//static bool WriteImageGDAL(const char* pFilePath, unsigned char *pImageData, GDALDataType nDataType, const int& widthDst, const int& heightDst, const int& nBandNum);
	//根据文件名匹配对应驱动名
	static char* findImageTypeGDAL(char *pDstImgFileName);
	//根据格式类型获取占用字节数
	static int GetBPB(const GDALDataType& nDataType);
	//数据的通道重新排序，例如BGR转RGB, arrNewSeq = {2,1,0}即可
	static int ReseqChannels(unsigned char* pData, const int& nRows, const int& nCols, const int& nBandNum, const int& nBPB, int arrNewSeq[]);
	//数据y方向进行翻转
	static int VerticalReverse(unsigned char* pData, const int& nRows, const int& nCols, const int& nBandNum, const int& nBPB);

#ifdef _ImageX
public:
	//兼容ImageX
	HRESULT GetRows(int* pRows){*pRows = m_nImgHeight;return S_OK;}
	HRESULT GetCols(int* pCols){*pCols = m_nImgWidth;return S_OK;}
	HRESULT GetBandNum(int* pBandNum){*pBandNum = m_nBandNum;return S_OK;}
	HRESULT GetGrdInfo(double* pLBX, double* pLBY, double* plfGSD){*pLBX = m_LBX;*pLBY = m_LBY;*plfGSD = m_lfGSD;return S_OK;}
	HRESULT GetBPB(int* pBPB){*pBPB = m_nBPB;return S_OK;}
	HRESULT GetBPP(int* pBPP){*pBPP = m_nBPP;return S_OK;}
	HRESULT GetDataType(int* pDataType){*pDataType = m_nDataType;return S_OK;}
	HRESULT GetBandFormat(int* pBandFormat){*pBandFormat = 0;return S_OK;}
#endif // _ImageX

private:
	bool m_bTranto8bit;

	GDALDataset *m_poDataset;

	int m_nOldBytesPerBand;

	unsigned char* m_plut;

	bool CreateDirectly(const char* szFilePath, GDALDataType nDataType, const int& widthDst, const int& heightDst, const int& nBandNum);
};
