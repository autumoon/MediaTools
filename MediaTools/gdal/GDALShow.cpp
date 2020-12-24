#include "..\StdAfx.h"
#include "GDALShow.h"
#include "afxdialogex.h"

//[6/26/2016 yufuxiang]
//[5/24/2018 autumoon]


CMutex mutexDrag;
CMutex mutexDragThread;


double CGDALShow::m_dMaxScale = 8.0;
double CGDALShow::m_dMinScale = 0.01;
double CGDALShow::m_dScaleStep = 120.0 / 0.125;
double CGDALShow::m_dXoffset = 0.0;
double CGDALShow::m_dYoffset = 0.0;
int CGDALShow::m_nBgValue = 240;
int CGDALShow::m_nButtonMove = 2;

CGDALShow::CGDALShow(CWnd* pWnd, const char* imgPath, unsigned char* pFrontBuffer, int nDCWidth, int nDCHeight,
	BOOL bIsDlg /*= FALSE*/, CWnd* pCtrlWnd /*= nullptr*/, CBitmap* pbmp /*= nullptr*/, CDC* pmemDC /*= nullptr*/, HBITMAP* phBmp /*= nullptr*/, HBITMAP* phOldBmp /*= nullptr*/)
{
	if (pWnd == nullptr || imgPath == nullptr)
	{
		return;
	}
	
	//复位所有参数
	ResetParameters();
	
	//获取重要参数
	if (bIsDlg)
	{
		//对话框的时候每次显示都会计算宽度与高度
		GetPointerForDlgShow(pCtrlWnd, pbmp, pmemDC, phBmp, phOldBmp);
	}
	else
	{
		if (pFrontBuffer == nullptr || nDCWidth < 1 || nDCHeight < 1)
		{
			return;
		}
	}
 	
	m_bIsDlg = bIsDlg == TRUE;
 	m_pWnd = pWnd;
 	m_pFrontBuffer = pFrontBuffer;
 
 	m_pGu = new CGdalUser();
 	if (!m_pGu->InitializeRead(imgPath))
 	{
 		return;
 	}
 	//先获取这些数据是为了计算偏移
 	m_nImgWidth = m_pGu->m_nImgWidth;
 	m_nImgHeight = m_pGu->m_nImgHeight;
 	m_nDCWidth = nDCWidth;
 	m_nDCHeight = nDCHeight;
 	
 	//居中显示
 	ResetOffset();
 	//剩下的数据在这里获取
 	CreatePyramid();
}

CGDALShow::~CGDALShow(void)
{
	if (m_pGu)
	{
		delete m_pGu;
		m_pGu = nullptr;
	}

	if (m_pHist)
	{
		delete[] m_pHist;
		m_pHist = nullptr;
	}

	if (m_plfCount)
	{
		delete[] m_plfCount;
		m_plfCount = nullptr;
	}

	if (m_pBufferPy)
	{
		delete[] m_pBufferPy;
		m_pBufferPy = nullptr;
	}

	if (m_pClrLut)
	{
		delete[] m_pClrLut;
		m_pClrLut = nullptr;
	}

	if (m_pFrontBufferDrag)
	{
		delete[] m_pFrontBufferDrag;
		m_pFrontBufferDrag = nullptr;
	}

	if (m_pFrontBufferDragThread)
	{
		delete[] m_pFrontBufferDragThread;
		m_pFrontBufferDragThread = nullptr;
	}
}

void CGDALShow::OnLButtonDown(POINT point)
{
	if (m_nButtonMove == 1)
	{
		CRect rect;
		if (m_bIsDlg)
		{
			m_pCtrlWnd->GetWindowRect(&rect);
		}
		else
		{
			m_pWnd->GetWindowRect(&rect);
			SetCapture(m_pWnd->GetSafeHwnd());
		}

		if (rect.PtInRect(point))
		{
			m_bIsLBtnDown = true;
			m_bUpdateScale = false;
			m_pMouDownPnt = point;
		}
	}
}

void CGDALShow::OnLButtonUp(POINT point)
{
	if (m_nButtonMove == 1)
	{
		m_bIsLBtnDown = false;
		m_pMouDownPnt = point;

		m_pXstart += m_pXoffset;
		m_pYstart += m_pYoffset;
		m_dXoffset = m_pXoffset;
		m_dYoffset = m_pYoffset;
		m_pXoffset = 0.0;
		m_pYoffset = 0.0;

		if (!m_bIsDlg)
		{
			ReleaseCapture();
		}

		ThreadReadingData();
	}
}

void CGDALShow::OnMButtonDown(POINT point)
{
	if (m_nButtonMove == 2)
	{
		CRect rect;
		if (m_bIsDlg)
		{
			m_pCtrlWnd->GetWindowRect(&rect);
		}
		else
		{
			m_pWnd->GetWindowRect(&rect);
			SetCapture(m_pWnd->GetSafeHwnd());
		}

		if (rect.PtInRect(point))
		{
			m_bIsMBtnDown = true;
			m_bUpdateScale = false;
			m_pMouDownPnt = point;
		}
	}
}

void CGDALShow::OnMButtonUp(POINT point)
{
	if (m_nButtonMove == 2)
	{
		m_bIsMBtnDown = false;
		m_pMouDownPnt = point;

		m_pXstart += m_pXoffset;
		m_pYstart += m_pYoffset;
		m_dXoffset = m_pXoffset;
		m_dYoffset = m_pYoffset;
		m_pXoffset = 0.0;
		m_pYoffset = 0.0;

		if (!m_bIsDlg)
		{
			ReleaseCapture();
		}

		ThreadReadingData();
	}
}

void CGDALShow::OnMouseMove(POINT point)
{
	if (m_bIsLBtnDown  && m_bIsLBtnDown == 1|| m_bIsMBtnDown && m_nButtonMove == 2 )
	{
		CRect rect;
		if (m_bIsDlg)
		{
			m_pCtrlWnd->GetWindowRect(&rect);
		}
		else
		{
			m_pWnd->GetWindowRect(&rect);
		}
		if (rect.PtInRect(point) || !m_bIsDlg)
		{
			m_nX = int(m_pXstart + m_pXoffset + 0.5);
			m_nY = int(m_pYstart + m_pYoffset + 0.5);
			m_nShowWidth = int(m_nImgWidth*m_dScale + 0.5);
			m_nShowHeight = int(m_nImgHeight*m_dScale + 0.5);

			m_pXoffset = point.x - m_pMouDownPnt.x;
			m_pYoffset = m_pMouDownPnt.y - point.y;
			ShowImage();
		}
	}
}

void CGDALShow::OnMouseWheel(short zDelta, CPoint pt)
{
	if(zDelta>120)  zDelta=120;
	if(zDelta<-120) zDelta=-120;

	double tmpOffsetX=0;double tmpOffsetY=0;

	CRect rect;
	if (m_bIsDlg)
	{
		m_pCtrlWnd->GetWindowRect(&rect);
	}
	else
	{
		m_pWnd->GetWindowRect(&rect);
	}
	
	POINT point;
	point.x=pt.x;
	point.y=pt.y;

	if (rect.PtInRect(point))
	{
		if(m_dScale > m_dMinScale && zDelta < 0 || m_dScale < m_dMaxScale && zDelta > 0)
		{
			double tmpscale = zDelta/m_dScaleStep;
			tmpscale *= m_dScale;

			tmpOffsetX=m_nImgWidth*tmpscale*((double)(point.x-rect.left-m_pXstart)/(double)(m_nImgWidth*(m_dScale)));
			tmpOffsetY=m_nImgHeight*tmpscale*((double)(rect.bottom - point.y - m_pYstart)/(double)(m_nImgHeight*(m_dScale)));

			int nX = int(m_pXstart-tmpOffsetX + 0.5);
			int nY = int(m_pYstart-tmpOffsetY + 0.5);
			int nShowWidth = int(m_nImgWidth*(m_dScale+tmpscale) + 0.5);
			int nShowHeight = int(m_nImgHeight*(m_dScale+tmpscale) + 0.5);
			if (nShowWidth > 0 && nShowHeight > 0)
			{
				//这里需要同步缩放drag中的数据
				int nBufferWidth = m_nDCWidth * MEM_ALLOC_X;
				int nBufferHeight = m_nDCHeight * MEM_ALLOC_Y;

				int nSrcLeft = m_nDCWidth * DC_OFFSET_NUM_X + m_nX;
				int nSrcTop = m_nDCHeight * DC_OFFSET_NUM_Y + m_nY;
				int nSrcRight = nSrcLeft + m_nShowWidth;
				int nSrcBottom = nSrcTop + m_nShowHeight;

				int nDstLeft = m_nDCWidth * DC_OFFSET_NUM_X + nX;
				int nDstTop = m_nDCHeight * DC_OFFSET_NUM_Y + nY;
				int nDstRight = nDstLeft + nShowWidth;
				int nDstBottom = nDstTop + nShowHeight;

				mutexDrag.Lock();
				ResampleArray(nBufferWidth, nBufferHeight, nSrcLeft, nSrcTop, nSrcRight, nSrcBottom, m_pFrontBufferDrag,
					nBufferWidth, nBufferHeight, nDstLeft, nDstTop, nDstRight, nDstBottom, m_pFrontBufferDrag);
				mutexDrag.Unlock();

				//采用此次缩放的结果
				m_nX = nX;
				m_nY = nY;
				m_nShowWidth = nShowWidth;
				m_nShowHeight = nShowHeight;
				ShowImage();
				m_dScale += tmpscale;
				m_pXstart -= tmpOffsetX;
				m_pYstart -= tmpOffsetY;
			}
		}
	}

	m_bUpdateScale = fabs(m_dScale - m_dScaleLast) > 0.00001;
	m_dScaleLast = m_dScale;

	ThreadReadingData();
}

bool CGDALShow::CreatePyramid()
{
	//显示照片
	m_nBandNum = m_pGu->m_nBandNum;
	m_nBPB = m_pGu->m_nBPB;

	//金字塔buffer,用于显示全局的时候读取显示
	const int nPyIniLength = 10000;

	double dWidthIniProp = (double)nPyIniLength / m_nImgWidth;
	double dHeightIniProp = (double)nPyIniLength / m_nImgHeight;
	m_dIniScale = dWidthIniProp < dHeightIniProp ? dWidthIniProp : dHeightIniProp;
	m_nIniWidth = int(m_pGu->m_nImgWidth * m_dIniScale + 0.5);
	m_nIniHeight = int(m_pGu->m_nImgHeight * m_dIniScale + 0.5);

	if (m_dIniScale >= 1.0)
	{
		//此时不需要缩放
		m_dIniScale = 1.0;
		m_nIniWidth = m_nImgWidth;
		m_nIniHeight = m_nImgHeight;
	}

	//分块读取更高效
	const int nBlock = 1024;
	BYTE* pBlockBuffer = new BYTE[m_nImgWidth * m_nBandNum * m_nBPB * nBlock];
	memset(pBlockBuffer, 0, sizeof(BYTE)* m_nImgWidth * m_nBandNum * m_nBPB * nBlock);
	int nBPP = m_nBPB * m_nBandNum;

	//用于显示的金字塔
	m_pBufferPy = new BYTE[m_nIniWidth * m_nIniHeight * 3];
	memset(m_pBufferPy, 0, m_nIniWidth * m_nIniHeight * 3);
	int nRowBlockNum = (m_nImgHeight + nBlock - 1) / nBlock;
	
	if (m_nBPB == 2)
	{
		//16位影像需要读取源数据，取得颜色查找表
		unsigned char* pBufferPyOri = new BYTE[m_nIniWidth * m_nIniHeight * m_nBandNum * m_nBPB];
		memset(pBufferPyOri, 0, m_nIniWidth * m_nIniHeight * m_nBandNum * m_nBPB);

		for (int j = 0; j < nRowBlockNum; ++j)
		{
			memset(pBlockBuffer, 0, sizeof(BYTE)* m_nImgWidth * m_nBandNum * m_nBPB * nBlock);
			m_pGu->ReadImg(0, j * nBlock, m_nImgWidth, j * nBlock + nBlock, pBlockBuffer,
				m_nImgWidth, nBlock, m_nBandNum, 0, 0, m_nImgWidth, nBlock, -1, 0);

			for (int m = 0; m < m_nIniHeight; m++)
			{
				int nSrcRows = int(m / m_dIniScale /*+ 0.5*/) - j * nBlock;
				if (nSrcRows >= nBlock || nSrcRows < 0)
				{
					continue;
				}
				BYTE *pBufferPyIndex = pBufferPyOri + m * m_nIniWidth * nBPP;
				BYTE *pBufferBlockIndex = pBlockBuffer + nSrcRows * m_nImgWidth * nBPP;
				for (int n = 0; n < m_nIniWidth; n++)
				{
					int nSrcCols = int(n / m_dIniScale /*+ 0.5*/);
					if (nSrcCols >= m_nImgWidth)
					{
						continue;
					}
					BYTE *pSubBufferPyIndex = pBufferPyIndex + n * nBPP;
					BYTE *pSubBufferBlockIndex = pBufferBlockIndex + nSrcCols * nBPP;
					memcpy(pSubBufferPyIndex, pSubBufferBlockIndex, nBPP);
				}
			}
		}
		//如果是16位影像
		if (m_pHist)
		{
			delete[] m_pHist;
			m_pHist = NULL;
		}
		m_pHist = new double[65536 * m_nBandNum];
		memset(m_pHist, 0, sizeof(double) * 65536 * m_nBandNum);

		for (int j = 0; j < m_nIniHeight; ++j)
		{
			unsigned short *pBufferIndex = (unsigned short*)pBufferPyOri + j * m_nIniWidth * m_nBandNum;
			for (int i = 0; i < m_nIniWidth; ++i)
			{
				unsigned short *pSubBufferIndex = pBufferIndex + i * m_nBandNum;
				for (int k = 0; k < m_nBandNum; ++k)
				{
					m_pHist[k * 65536 + pSubBufferIndex[k]] += 1.0;
				}
			}
		}

		if (m_pClrLut)
		{
			delete[] m_pClrLut;
			m_pClrLut = NULL;
		}

		m_pClrLut = new BYTE[m_nBandNum * 65536];
		memset(m_pClrLut, 0, sizeof(BYTE) * 65536 * m_nBandNum);

		if (m_plfCount)
		{
			delete[] m_plfCount;
			m_plfCount = NULL;
		}
		m_plfCount = new double[10];
		memset(m_plfCount, 0, sizeof(double) * 10);
		for (int i = 0; i < m_nBandNum; i++)
		{
			for (int j = 1; j < 65536; j++)
			{
				m_plfCount[i] += m_pHist[i * 65536 + j];
			}
		}

		double lfMinThreshold = 0.001, lfMaxThreshold = 0.001;

		for (int i = 0; i < m_nBandNum; i++)
		{
			double lfTmpCount = 0.0001;
			int nMinCut = 1, nMaxCut = 65535;
			for (int j = 1; j < 65536; j++)
			{
				lfTmpCount += m_pHist[i * 65536 + j];
				if (lfTmpCount / m_plfCount[i] > lfMinThreshold)
				{
					nMinCut = j;
					break;
				}
			}
			int nMinValue = 0, nMaxValue = 0;
			for (int j = 1; j < 65536; j++)
			{
				if (m_pHist[i * 65536 + j] > 1e-3)
				{
					nMinValue = j;
					break;
				}
			}
			for (int j = 65534; j > 0; j--)
			{
				if (m_pHist[i * 65536 + j] > 1e-3)
				{
					nMaxValue = j;
					break;
				}
			}

			lfTmpCount = 0.0001;
			for (int j = 65534; j > 0; j--)
			{
				lfTmpCount += m_pHist[i * 65536 + j];
				if (lfTmpCount / m_plfCount[i] > lfMaxThreshold)
				{
					nMaxCut = j;
					break;
				}
			}
			for (int j = 1; j < nMinCut; j++)
			{
				m_pClrLut[i * 65536 + j] = 1;
			}
			for (int j = nMinCut; j <= nMaxCut; j++)
			{
				m_pClrLut[i * 65536 + j] = max(1, min(253, (int)(251.0 * ((double)j - nMinCut) / ((double)nMaxCut - nMinCut) + 2)));
			}
			for (int j = nMaxCut + 1; j < 65536; j++)
			{
				m_pClrLut[i * 65536 + j] = 254;
			}
		}

		//删除
		delete[] pBufferPyOri;
		pBufferPyOri = nullptr;
	}

	for (int j = 0; j < nRowBlockNum; ++j)
	{
		int nReadHeight = min(nBlock, m_nImgHeight - nBlock * j);
		memset(pBlockBuffer, 0, sizeof(BYTE)* m_nImgWidth * m_nBandNum * m_nBPB * nBlock);
		m_pGu->ReadImg(0, j * nBlock, m_nImgWidth, j * nBlock + nReadHeight, pBlockBuffer, m_nImgWidth, nReadHeight, m_nBandNum, 0, 0, m_nImgWidth, nReadHeight, -1, 0);

		//将当前读取的块，转换为RGB
		int nDstLeft = 0;
		int nDstTop = int(j * nBlock * m_dIniScale + 0.5);
		int nDstRight = m_nIniWidth;
		int nDstBottom = int((j * nBlock + nReadHeight) * m_dIniScale + 0.5);
		ResampleArray2BGR(m_nImgWidth, nReadHeight, 0, 0, m_nImgWidth, nReadHeight, pBlockBuffer, m_nBandNum,
			m_nIniWidth, m_nIniHeight, nDstLeft, nDstTop, nDstRight, nDstBottom, m_pBufferPy, 3);
	}

	delete[] pBlockBuffer;
	pBlockBuffer = nullptr;

	return 0;
}

bool CGDALShow::GetRealPixValue(int nCtrlX, int nCtrlY, std::vector<int>& vRealValues)
{
	int nImgX = 0, nImgY = 0;

	//用GDALUser读取的时候，y是反的
	if (!GetImgCoors(nCtrlX, nCtrlY, nImgX, nImgY, true))
	{
		return false;
	}

	vRealValues.clear();
	//读取当前行的数据，保留原始数据
	unsigned char* pData = new unsigned char[m_nBandNum * m_nBPB];
	m_pGu->ReadImage(pData, nImgX, nImgY, 1, 1);

	if (m_nBPB == 1)
	{
		for (int i = 0; i < m_nBandNum; ++i)
		{
			vRealValues.push_back(pData[i]);
		}
	}
	else if (m_nBPB == 2)
	{
		unsigned short* pSData = (unsigned short*)pData;
		for (int i = 0; i < m_nBandNum; ++i)
		{
			vRealValues.push_back(pSData[i]);
		}
	}

	//如果是单通道，则需要复制结果
	if (m_nBandNum < 3)
	{
		vRealValues.resize(3);
		vRealValues[1] = vRealValues[0];
		vRealValues[2] = vRealValues[0];
	}

	delete[] pData;
	pData = nullptr;

	return true;
}

bool CGDALShow::PreReadFrontBufferDrag(LPVOID lpParameters)
{
	mutexDragThread.Lock();
	CGDALShow* pGs = (CGDALShow*)lpParameters;

	//获取参数
	CGdalUser* pGu = pGs->m_pGu;
	unsigned char** ppBufferPy = &pGs->m_pBufferPy;
	unsigned char** ppFrontBuffer = &pGs->m_pFrontBuffer;
	unsigned char** ppFrontBufferDrag = &pGs->m_pFrontBufferDrag;
	unsigned char** ppFrontBufferDragThread = &pGs->m_pFrontBufferDragThread;
	const double& dScale = pGs->m_dScale;
	const double& dXoffset = pGs->m_dXoffset;
	const double& dYoffset = pGs->m_dYoffset;
	const double& dIniScale = pGs->m_dIniScale;
	const int& nDCWidth = pGs->m_nDCWidth;
	const int& nDCHeight = pGs->m_nDCHeight;
	const int& nBPB = pGs->m_nBPB;
	const int& nBandNum = pGs->m_nBandNum;
	const int& nIniWidth = pGs->m_nIniWidth;
	const int& nIniHeight = pGs->m_nIniHeight;
	const int& nX = pGs->m_nX;
	const int& nY = pGs->m_nY;
	const int& nShowWidth = pGs->m_nShowWidth;
	const int& nShowHeight = pGs->m_nShowHeight;

	if (pGu == nullptr && ppBufferPy == nullptr)
	{
		return false;
	}

	//根据当前的比例和金字塔比例，决定从金字塔还是从影像直接读取数据
	int nBufferWidth = nDCWidth * MEM_ALLOC_X;
	int nBufferHeight = nDCHeight * MEM_ALLOC_Y;
	const int nBufferSize = nBufferWidth * nBufferHeight * 3;

	if (*ppFrontBufferDragThread != nullptr)
	{
		delete[] *ppFrontBufferDragThread;
	}

	*ppFrontBufferDragThread = new unsigned char[nBufferSize];
	memset(*ppFrontBufferDragThread, m_nBgValue, nBufferSize);

	const int nBufferSizeOri = nBufferWidth * nBufferHeight * nBandNum * nBPB;
	BYTE* pFrontBufferDragOri = new BYTE[nBufferSizeOri];

	//将用于拖动的缓存刷成背景色
	memset(pFrontBufferDragOri, m_nBgValue, nBufferSizeOri);

	int nPosOffsetX = 0;
	int nPosOffsetY = 0;
	int nWidthOffset = 0;
	int nHeightOffset = 0;

	if (dIniScale < 1.0 && dScale > dIniScale)
	{
		//此时需要从影像直接读取
		int nSrcLeft = 0;
		int nSrcTop = 0;
		int nSrcRight = pGs->m_nImgWidth;
		int nSrcBottom = pGs->m_nImgHeight;

		int nDstLeft = 0;
		int nDstTop = 0;
		int nDstRight = nShowWidth;
		int nDstBottom = nShowHeight;

		if (nShowWidth > nBufferWidth || nShowHeight > nBufferHeight)
		{
			//此时需要读取的数据太多，用缓存读取部分数据即可
			if (nDCWidth + nX < 0)
			{
				//此时读取起点需要变更
				nSrcLeft = int(-(nDCWidth + nX) / dScale - 0.5);
				nPosOffsetX = -(nDCWidth + nX);
				nDstRight -= nPosOffsetX;
			}

			if (nDCHeight + nY < 0)
			{
				nSrcTop = int(-(nDCHeight + nY) / dScale - 0.5);
				nPosOffsetY = -(nDCHeight + nY);
				nDstBottom -= nPosOffsetY;
			}

			if (nDstRight > nBufferWidth)
			{
				nWidthOffset = nDstRight - nBufferWidth;
				nSrcRight = int(nSrcLeft + nBufferWidth / dScale /*+ 0.5*/);
				nDstRight = nBufferWidth;
			}

			if (nDstBottom > nBufferHeight)
			{
				nHeightOffset = nDstBottom - nBufferHeight;
				nSrcBottom = int(nSrcTop + nBufferHeight / dScale /*+ 0.5*/);
				nDstBottom = nBufferHeight;
			}
		}

		int nPosLeft = nDCWidth + nX + nPosOffsetX;
		int nPosTop = nDCHeight + nY + nPosOffsetY;
		int nPosRight = nPosLeft + nShowWidth - nPosOffsetX - nWidthOffset;
		int nPosBottom = nPosTop + nShowHeight - nPosOffsetY - nHeightOffset;

		//读取9倍区域到drag缓存中
		HRESULT hRes = pGu->ReadImg(nSrcLeft, nSrcTop, nSrcRight, nSrcBottom, pFrontBufferDragOri,
			nBufferWidth, nBufferHeight, nBandNum, nDstLeft, nDstTop, nDstRight, nDstBottom, -1, 0);
		//偏移到指定的位置，可能越界（已经有越界处理）
		pGs->ResampleArray2BGR(nBufferWidth, nBufferHeight, 0, 0, nDstRight, nDstBottom, pFrontBufferDragOri, nBandNum,
			nBufferWidth, nBufferHeight, nPosLeft, nPosTop, nPosRight, nPosBottom, *ppFrontBufferDragThread, 3);
	}
	else
	{
		//金字塔直接处理
		int nPosLeft = nDCWidth + nX + nPosOffsetX;
		int nPosTop = nDCHeight + nY + nPosOffsetY;
		int nPosRight = nPosLeft + nShowWidth - nPosOffsetX - nWidthOffset;
		int nPosBottom = nPosTop + nShowHeight - nPosOffsetY - nHeightOffset;
		//偏移到指定的位置，可能越界（已经有越界处理）
		pGs->ResampleArray(nIniWidth, nIniHeight, 0, 0, nIniWidth, nIniHeight, *ppBufferPy,
			nBufferWidth, nBufferHeight, nPosLeft, nPosTop, nPosRight, nPosBottom, *ppFrontBufferDragThread);
	}

	mutexDrag.Lock();
	//这个量专门用于读写时候的拖拽
	if (*ppFrontBufferDrag != nullptr)
	{
		delete[] * ppFrontBufferDrag;
	}
	*ppFrontBufferDrag = new unsigned char[nBufferSize];
	memcpy(*ppFrontBufferDrag, *ppFrontBufferDragThread, nBufferSize);
	mutexDrag.Unlock();

	mutexDragThread.Unlock();
	pGs->m_bDragDataReady = true;

	delete[] pFrontBufferDragOri;
	pFrontBufferDragOri = nullptr;

	return true;
}

int CGDALShow::GetPointerForDlgShow(CWnd* pCtrlWnd, CBitmap* pbmp, CDC* pmemDC, HBITMAP* phBmp, HBITMAP* phOldBmp)
{
	//DC可能随着窗口的改变而改变
	m_pCtrlWnd = pCtrlWnd;
	m_pbmp = pbmp;
	m_pmemDC = pmemDC;
	m_phBmp = phBmp;
	m_phOldBmp = phOldBmp;

	return 0;
}

int CGDALShow::ResampleArray(int nSrcBufWidth, int nSrcBufHeight, int nSrcLeft, int nSrcTop, int nSrcRight, int nSrcBottom, unsigned char* pSrc,
	int nDstBufWidth, int nDstBufHeight, int nDstLeft, int nDstTop, int nDstRight, int nDstBottom, unsigned char* pDst)
{
	if (pSrc == nullptr || pDst == nullptr)
	{
		return -1;
	}

	//左上为小
	int nSrcWidth = nSrcRight - nSrcLeft;
	int nSrcHeight = nSrcBottom - nSrcTop;
	int nDstWidth = nDstRight - nDstLeft;
	int nDstHeight = nDstBottom - nDstTop;

	if (nSrcWidth < 0 || nSrcHeight < 0 || nDstWidth < 0 || nDstHeight < 0)
	{
		return -1;
	}

	double dWidthProp = (double)nDstWidth / nSrcWidth;
	double dHeightProp = (double)nDstHeight / nSrcHeight;

	//如果源地址和目标地址一样
	unsigned char* pSrcBak(pSrc);
	if (pSrc == pDst)
	{
		int nSrcMemSize = nSrcBufWidth * nSrcBufHeight * 3;
		pSrcBak = new unsigned char[nSrcMemSize];
		memcpy(pSrcBak, pSrc, nSrcMemSize);
		memset(pSrc, m_nBgValue, nSrcMemSize);
	}

	//处理8位图像
	for (int j = 0; j < nDstHeight; ++j)
	{
		int nyc = nDstTop + j;
		int nyp = nSrcTop + int(j / dHeightProp/* + 0.5*/);
		if (nyp >= AU_MAX(0, nSrcTop) && nyp < AU_MIN(nSrcBottom, nSrcBufHeight)
			&& nyc >= AU_MAX(0, nDstTop) && nyc < AU_MIN(nDstBottom, nDstBufHeight))
		{
			unsigned char* pBufferDst = pDst + (nyc * nDstBufWidth) * 3;
			unsigned char* pBufferSrc = pSrcBak + (nyp * nSrcBufWidth) * 3;
			for (int i = 0; i < nDstWidth; ++i)
			{
				int nxc = nDstLeft + i;
				int nxp = nSrcLeft + int(i / dWidthProp/* + 0.5*/);
				if (nxp >= AU_MAX(0, nSrcLeft) && nxp < AU_MIN(nSrcRight, nSrcBufWidth)
					&& nxc >= AU_MAX(0, nDstLeft) && nxc < AU_MIN(nDstRight, nDstBufWidth))
				{
					unsigned char* pSubBufferDst = pBufferDst + nxc * 3;
					unsigned char* pSubBufferSrc = pBufferSrc + nxp * 3;
					memcpy(pSubBufferDst, pSubBufferSrc, 3);
				}
			}
		}
	}

	if (pSrc == pDst && pSrcBak != nullptr)
	{
		delete[] pSrcBak;
		pSrcBak = nullptr;
	}

	return 0;
}

int CGDALShow::ResampleArray2BGR(int nSrcBufWidth, int nSrcBufHeight, int nSrcLeft, int nSrcTop, int nSrcRight, int nSrcBottom, unsigned char* pSrc, int nSrcBandNum,
	int nDstBufWidth, int nDstBufHeight, int nDstLeft, int nDstTop, int nDstRight, int nDstBottom, unsigned char* pDst, int nDstBandNum,
	int nSizeOfPointer /*= 1*/, int nSrcSkip /*= 0*/, int nDestSkip /*= 0*/)
{
	if (pSrc == nullptr || pDst == nullptr || nDstBandNum < 3)
	{
		//必须转成BGR
		return -1;
	}

	//左上为小
	int nSrcWidth = nSrcRight - nSrcLeft;
	int nSrcHeight = nSrcBottom - nSrcTop;
	int nDstWidth = nDstRight - nDstLeft;
	int nDstHeight = nDstBottom - nDstTop;

	if (nSrcWidth < 0 || nSrcHeight < 0 || nDstWidth < 0 || nDstHeight < 0)
	{
		return -1;
	}

	double dWidthProp = (double)nDstWidth / nSrcWidth;
	double dHeightProp = (double)nDstHeight / nSrcHeight;

	//如果源地址和目标地址一样
	unsigned char* pSrcBak(pSrc);
	if (pSrc == pDst)
	{
		int nSrcMemSize = nSrcBufWidth * nSrcBufHeight * nSrcBandNum * nSizeOfPointer;
		pSrcBak = new unsigned char[nSrcMemSize];
		memcpy(pSrcBak, pSrc, nSrcMemSize);
		memset(pSrc, m_nBgValue, nSrcMemSize);
	}

	//采样
	const int nBandNum = AU_MIN(nDstBandNum, nSrcBandNum);

	if (m_nBPB == 1)
	{
		//处理8位图像
		for (int j = 0; j < nDstHeight; ++j)
		{
			int nyc = nDstTop + j;
			int nyp = nSrcTop + int(j / dHeightProp/* + 0.5*/);
			if (nyp >= AU_MAX(0, nSrcTop) && nyp < AU_MIN(nSrcBottom, nSrcBufHeight)
				&& nyc >= AU_MAX(0, nDstTop) && nyc < AU_MIN(nDstBottom, nDstBufHeight))
			{
				unsigned char* pBufferDst = pDst + (nyc * nDstBufWidth) * nDstBandNum * nSizeOfPointer + nSrcSkip;
				unsigned char* pBufferSrc = pSrcBak + (nyp * nSrcBufWidth) * nSrcBandNum * nSizeOfPointer + nDestSkip;
				for (int i = 0; i < nDstWidth; ++i)
				{
					int nxc = nDstLeft + i;
					int nxp = nSrcLeft + int(i / dWidthProp/* + 0.5*/);
					if (nxp >= AU_MAX(0, nSrcLeft) && nxp < AU_MIN(nSrcRight, nSrcBufWidth)
						&& nxc >= AU_MAX(0, nDstLeft) && nxc < AU_MIN(nDstRight, nDstBufWidth))
					{
						unsigned char* pSubBufferDst = pBufferDst + nxc * nDstBandNum * nSizeOfPointer;
						unsigned char* pSubBufferSrc = pBufferSrc + nxp * nSrcBandNum * nSizeOfPointer;
						if (nSrcBandNum == 1)
						{
							for (int k = 0; k < 3; ++k)
							{
								memcpy(pSubBufferDst + k, pSubBufferSrc, nSizeOfPointer);
							}
						}
						else if (nSrcBandNum >= 3)
						{
							for (int k = 0; k < 3; ++k)
							{
								//bmp为BGR的顺序
								memcpy(pSubBufferDst + k, pSubBufferSrc + 2 - k, nSizeOfPointer);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		//处理16位图像
		unsigned short* pBuffer = (unsigned short*)pSrcBak;
		for (int j = 0; j < nDstHeight; ++j)
		{
			int nyc = nDstTop + j;
			int nyp = nSrcTop + int(j / dHeightProp/* + 0.5*/);
			if (nyp >= AU_MAX(0, nSrcTop) && nyp < AU_MIN(nSrcBottom, nSrcBufHeight)
				&& nyc >= AU_MAX(0, nDstTop) && nyc < AU_MIN(nDstBottom, nDstBufHeight))
			{
				unsigned char* pBufferDst = pDst + (nyc * nDstBufWidth) * nDstBandNum + nSrcSkip;
				unsigned short* pBufferSrc = pBuffer + (nyp * nSrcBufWidth) * nSrcBandNum + nDestSkip;
				for (int i = 0; i < nDstWidth; ++i)
				{
					int nxc = nDstLeft + i;
					int nxp = nSrcLeft + int(i / dWidthProp/* + 0.5*/);
					if (nxp >= AU_MAX(0, nSrcLeft) && nxp < AU_MIN(nSrcRight, nSrcBufWidth)
						&& nxc >= AU_MAX(0, nDstLeft) && nxc < AU_MIN(nDstRight, nDstBufWidth))
					{
						unsigned char* pSubBufferDst = pBufferDst + nxc * nDstBandNum;
						unsigned short* pSubBufferSrc = pBufferSrc + nxp * nSrcBandNum;
						if (nSrcBandNum == 1)
						{
							for (int k = 0; k < 3; k++)
							{
								pSubBufferDst[k] = m_pClrLut[pSubBufferSrc[0]];
							}
						}
						else if (nSrcBandNum >= 3)
						{
							for (int k = 0; k < 3; k++)
							{
								//bmp为BGR的顺序
								pSubBufferDst[k] = m_pClrLut[(2 - k) * 65536 + pSubBufferSrc[2 - k]];
							}
						}
					}
				}
			}
		}
	}

	if (pSrc == pDst && pSrcBak != nullptr)
	{
		delete[] pSrcBak;
		pSrcBak = nullptr;
	}

	return 0;
}

int CGDALShow::ThreadReadingData(BOOL bThread /*= TRUE*/)
{
	m_bDragDataReady = false;

	if (bThread)
	{
		AfxBeginThread(AFX_THREADPROC(PreReadFrontBufferDrag), this);
	}
	else
	{
		PreReadFrontBufferDrag(this);
	}

	return 0;
}

//
void CGDALShow::ResetOffset()
{
	if (m_nDCWidth < 1 || m_nDCHeight < 1 || m_nImgWidth < 1 || m_nImgHeight < 1)
	{
		return;
	}

	CRect rect;
	if (m_bIsDlg)
	{
		m_pCtrlWnd->GetWindowRect(&rect);
	}
	else
	{
		m_pWnd->GetWindowRect(&rect);
	}

	//计算比例
	double dWidthProp = (double)m_nDCWidth / m_nImgWidth;
	double dHeightProp = (double)m_nDCHeight / m_nImgHeight;
	m_dScale = min(dWidthProp, dHeightProp);
	m_dScale = min(1.0, m_dScale);

	double dNeedWidth = m_nImgWidth * m_dScale;
	double dNeedHeight = m_nImgHeight * m_dScale;
	//计算可能需要的偏移
	if (dNeedWidth < m_nDCWidth)
	{
		//此时需要显示的图像比较小
		m_pXstart = (m_nDCWidth - dNeedWidth) / 2;
	}

	if (dNeedHeight < m_nDCHeight)
	{
		//此时需要显示的图像比较小
		m_pYstart = (m_nDCHeight - dNeedHeight) / 2;
	}

	m_nX = int(m_pXstart + 0.5);
	m_nY = int(m_pYstart + 0.5);
	m_nShowWidth = int(m_nImgWidth*m_dScale + 0.5);
	m_nShowHeight = int(m_nImgHeight*m_dScale + 0.5);
}

void CGDALShow::ResetParameters()
{
	m_bDragDataReady = false;
	m_bIsLBtnDown = false;
	m_bIsMBtnDown = false;
	m_bIsDlg = false;
	m_bUpdateScale = true;

	m_dIniScale = 1.0;
	m_dScaleLast = 1.0;

	m_nDCWidth = 0;
	m_nDCHeight = 0;
	
	m_pGu = nullptr;
	m_pHist = nullptr;
	m_plfCount = nullptr;
	m_pFrontBuffer = nullptr;
	m_pFrontBufferDrag = nullptr;
	m_pFrontBufferDragThread = nullptr;	
	m_pBufferPy = nullptr;
	m_pClrLut = nullptr;
	m_pWnd = nullptr;
	m_pMouDownPnt = POINT();

	m_dXoffset = 0.0;
	m_dYoffset = 0.0;
	m_pXoffset = 0.0;
	m_pYoffset = 0.0;
	m_pXstart = 0.0;
	m_pYstart = 0.0;

	m_nImgWidth = 0;
	m_nImgHeight = 0;
	m_nIniWidth = 0;
	m_nIniHeight = 0;
	m_nBandNum = 1;
	m_nBPB = 1;

	m_nX = 0;
	m_nY = 0;

	m_nShowWidth = 0;
	m_nShowHeight = 0;
	m_dScale = 1.0;

	m_pbmp = nullptr;
	m_pmemDC = nullptr;
	m_pCtrlWnd = nullptr;
	m_phBmp = nullptr;
	m_phOldBmp = nullptr;

	//static变量
	m_dMaxScale = 8.0;
	m_dMinScale = 0.01;
	m_dScaleStep = 120.0 / 0.125;
	m_dXoffset = 0.0;
	m_dYoffset = 0.0;
	m_nBgValue = 240;
}

void CGDALShow::ShowImage(BOOL bDirectDC /*= false*/)
{
	//如果是对话框，需要做以下操作
	if (m_bIsDlg && m_pCtrlWnd && m_phBmp && m_pbmp)
	{
		CRect rcPicture;
		m_pCtrlWnd->GetWindowRect(rcPicture);
		m_nDCWidth = rcPicture.Width();
		m_nDCHeight = rcPicture.Height();

		//宽度应为4的整数倍
		m_nDCWidth = ((m_nDCWidth + 3) / 4) * 4;

		if (*m_phBmp)
		{
			::DeleteObject(*m_phBmp);
			*m_phBmp = nullptr;
		}

		BITMAPINFO bmpInfo;
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = m_nDCWidth;
		bmpInfo.bmiHeader.biHeight = m_nDCHeight;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 24;
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		bmpInfo.bmiHeader.biSizeImage = 0;
		bmpInfo.bmiHeader.biXPelsPerMeter = 0;
		bmpInfo.bmiHeader.biYPelsPerMeter = 0;
		bmpInfo.bmiHeader.biClrUsed = 0;
		bmpInfo.bmiHeader.biClrImportant = 0;

		*m_phBmp = CreateDIBSection(m_pmemDC->m_hDC, &bmpInfo, DIB_RGB_COLORS, (void **)&m_pFrontBuffer, NULL, 0);
		*m_phOldBmp = (HBITMAP)::SelectObject(m_pmemDC->m_hDC, *m_phBmp);

		m_pbmp->Detach();
		m_pbmp->Attach(*m_phBmp);
	}

	//必须使用外部双缓存
	memset(m_pFrontBuffer, m_nBgValue, m_nDCWidth * m_nDCHeight * 3);
	if(m_bUpdateScale || !m_bDragDataReady)
	{
		//线程正在读取数据的时候，读取金字塔
		ResampleArray(m_nIniWidth, m_nIniHeight, 0, 0, m_nIniWidth, m_nIniHeight, m_pBufferPy, m_nDCWidth, m_nDCHeight,
			m_nX, m_nY, m_nX + m_nShowWidth, m_nY + m_nShowHeight, m_pFrontBuffer);
	}
	else if (m_pFrontBufferDragThread)
	{
		//显示部分影像时，将m_pFrontBufferDrag中间部分的内容平移到当前DC当中
		int nBufferWidth = m_nDCWidth * MEM_ALLOC_X;
		int nBufferHeight = m_nDCHeight * MEM_ALLOC_Y;

		int nSrcLeft = int(m_nDCWidth - m_pXoffset - 0.5);
		int nSrcTop = int(m_nDCHeight - m_pYoffset - 0.5);
		int nSrcRight = nSrcLeft + m_nDCWidth;
		int nSrcBottom = nSrcTop + m_nDCHeight;

		int nDstLeft = 0;
		int nDstTop = 0;
		int nDstRight = m_nDCWidth;
		int nDstBottom = m_nDCHeight;

		mutexDrag.Lock();
		for (int j = 0; j < m_nDCHeight; ++j)
		{
			unsigned char* pBufferDst = m_pFrontBuffer + (j * m_nDCWidth) * 3;
			unsigned char* pBufferSrc = m_pFrontBufferDrag + ((nSrcTop + j) * nBufferWidth) * 3 + nSrcLeft * 3;
			if (nSrcTop + j >= 0 && nSrcTop + j < nBufferHeight)
			{
				int nCopyWidth = AU_MIN((nBufferWidth - nSrcLeft), m_nDCWidth);
				if (nCopyWidth > 0)
				{
					memcpy(pBufferDst, pBufferSrc, nCopyWidth * 3);
				}
			}
		}
		mutexDrag.Unlock();
	}

	m_pWnd->Invalidate(FALSE);
}

bool CGDALShow::GetPixValue(int nCtrlX, int nCtrlY, std::vector<int>& values, bool bGetRealValue /*= false*/)
{
	if (nCtrlX < 0 || nCtrlX >= m_nDCWidth || nCtrlY <= 0 || nCtrlY > m_nDCHeight)
	{
		return false;
	}

	if (bGetRealValue)
	{
		//获取真实的像素值
		return GetRealPixValue(nCtrlX, nCtrlY, values);
	}

	//读取当前屏幕像素值
	nCtrlY = m_nDCHeight - nCtrlY;
	values.clear();
	int x1 = *(m_pFrontBuffer + (nCtrlY * m_nDCWidth + nCtrlX) * 3 + 0);
	int x2 = *(m_pFrontBuffer + (nCtrlY * m_nDCWidth + nCtrlX) * 3 + 1);
	int x3 = *(m_pFrontBuffer + (nCtrlY * m_nDCWidth + nCtrlX) * 3 + 2);

	//输出RGB
	values.push_back(x3);
	values.push_back(x2);
	values.push_back(x1);

	return true;
}

bool CGDALShow::GetImgCoors(int nCtrlX, int nCtrlY, int& nImgX, int& nImgY, bool bYReverse /*= false*/)
{
	nImgX = int((nCtrlX - m_nX) / m_dScale);
	nImgY = int((nCtrlY - (m_nDCHeight - m_nShowHeight - m_nY)) / m_dScale);

	if (bYReverse)
	{
		//从0开始到m_nImgHeight - 1
		nImgY = m_nImgHeight - 1 - nImgY;
	}

	return nImgX >= 0 && nImgX < m_nImgWidth && nImgY >= 0 && nImgY < m_nImgHeight;
}
