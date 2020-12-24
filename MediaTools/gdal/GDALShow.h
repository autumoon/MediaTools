//[5/25/2018 autumoon]

#pragma once
#include "GdalUser.h"

//MEM_ALLOC_X、MEM_ALLOC_X必须是1或者3，为1时节省内存，为3时拖动体验更好
#define	MEM_ALLOC_X			3
#define	MEM_ALLOC_Y			3
#define	MEM_ALLOC_NUM		(MEM_ALLOC_X * MEM_ALLOC_Y)
#define DC_OFFSET_NUM_X		((MEM_ALLOC_X - 1) >> 1)
#define DC_OFFSET_NUM_Y		((MEM_ALLOC_Y - 1) >> 1)

#define AU_MIN(x,y)	((x) < (y) ? (x) : (y))
#define AU_MAX(x,y)	((x) > (y) ? (x) : (y))

//必须使用外部双缓存
class CGDALShow
{
public:
	//如果不是对话框，第二行参数全部取默认值即可
	CGDALShow(CWnd* pWnd, const char* imgPath, unsigned char* pFrontBuffer, int nDCWidth, int nDCHeight,
		BOOL bIsDlg = FALSE, CWnd* pCtrlWnd = nullptr, CBitmap* pbmp = nullptr, CDC* pmemDC = nullptr, HBITMAP* phBmp = nullptr, HBITMAP* phOldBmp = nullptr);
	~CGDALShow(void);
public:
	bool m_bIsLBtnDown;							//左键是否按下
	bool m_bIsMBtnDown;							//中键是否按下
	bool m_bUpdateScale;						//是否需要更新比例
	CBitmap* m_pbmp;							//外部显示使用的变量，尺寸变化时内部可能更新
	CDC* m_pmemDC;
	CWnd* m_pCtrlWnd;
	HBITMAP* m_phBmp;
	HBITMAP* m_phOldBmp;
	int m_nDCWidth;								//显示区域的宽度
	int m_nDCHeight;							//显示区域的高度

	unsigned char* m_pFrontBuffer;				//内存buffer
	unsigned char* m_pFrontBufferDrag;			//DCbuffer备份，用于拖动的时候缓冲
	unsigned char* m_pFrontBufferDragThread;	//DCbuffer备份，用于线程读取

	//根据控件坐标，获取信息
	bool GetPixValue(int nCtrlX, int nCtrlY, std::vector<int>& values, bool bGetRealValue = false);
	bool GetImgCoors(int nCtrlX, int nCtrlY, int& nImgX, int& nImgY, bool bYReverse = false);

	int ThreadReadingData(BOOL bThread = TRUE);
	void OnLButtonDown(POINT point);
	void OnLButtonUp(POINT point);
	void OnMButtonDown(POINT point);
	void OnMButtonUp(POINT point);
	void OnMouseMove(POINT point);
	void OnMouseWheel(short zDelta, CPoint pt);
	void ShowImage(BOOL bDirectDC = FALSE);

	static double m_dMaxScale;					//缩放最大比例
	static double m_dMinScale;					//缩放最小比例
	static double m_dScaleStep;					//缩放步进值
	static double m_dXoffset;					//显示区域留白x
	static double m_dYoffset;					//显示区域留白y
	static int m_nBgValue;						//背景值
	static int m_nButtonMove;					//使用鼠标按钮移动 1代表左键 2代表中键
	
protected:
	bool m_bIsDlg;
	bool m_bDragDataReady;
	CGdalUser* m_pGu;
	double* m_pHist;					//直方图统计
	double* m_plfCount;					//累计概率
	double m_dScale;					//当前比例
	double m_dScaleLast;				//上一次比例
	double m_dIniScale;					//金字塔比例
	unsigned char* m_pBufferPy;			//金字塔Buffer
	unsigned char* m_pClrLut;			//颜色查找表

	bool CreatePyramid();
	bool GetRealPixValue(int nCtrlX, int nCtrlY, std::vector<int>& vRealValues);
	int GetPointerForDlgShow(CWnd* pDlgWnd, CBitmap* pbmp, CDC* pmemDC, HBITMAP* phBmp, HBITMAP* phOldBmp);
	int ResampleArray(int nSrcBufWidth, int nSrcBufHeight, int nSrcLeft, int nSrcTop, int nSrcRight, int nSrcBottom, unsigned char* pSrc,
		int nDstBufWidth, int nDstBufHeight, int nDstLeft, int nDstTop, int nDstRight, int nDstBottom, unsigned char* pDst);
	int ResampleArray2BGR(int nSrcBufWidth, int nSrcBufHeight, int nSrcLeft, int nSrcTop, int nSrcRight, int nSrcBottom, unsigned char* pSrc, int nSrcBandNum,
		int nDstBufWidth, int nDstBufHeight, int nDstLeft, int nDstTop, int nDstRight, int nDstBottom, unsigned char* pDst, int nDstBandNum,
		int nSizeOfPointer = 1, int nSrcSkip = 0, int nDestSkip = 0);

	//用于线程读取备用数据
	static bool PreReadFrontBufferDrag(LPVOID lpParameters);
	void ResetOffset();
	void ResetParameters();

private:
	CWnd* m_pWnd;
	double m_pXoffset;
	double m_pYoffset;
	double m_pXstart;
	double m_pYstart;

	int m_nImgWidth;
	int m_nImgHeight;
	int m_nIniWidth, m_nIniHeight, m_nBandNum, m_nBPB;
	int m_nX;
	int m_nY;
	int m_nShowWidth;
	int m_nShowHeight;

	POINT m_pMouDownPnt;
};

//对话框外部双缓存
/************************************************************************
//Dlg中定义
CGDALShow* m_pGDALShow;
int m_nDCHeight;
int m_nDCWidth;
unsigned char* m_pFrontBuffer;

CBitmap m_bmp;
CDC m_memDC;
HBITMAP m_hBmp;
HBITMAP m_hOldBmp;

//构造函数中初始化
m_pGDALShow = nullptr;
m_nDCWidth = -1;
m_nDCHeight = -1;
m_pFrontBuffer = nullptr;

//析构函数中释放
if (m_pGDALShow)
{
	delete m_pGDALShow;
	m_pGDALShow = nullptr;
}

//添加static控件或者picturectrl控件
//动作按钮中添加如下内容
CString strFilePath = CMfcStrFile::OpenFile();

if (m_pGDALShow)
{
	delete m_pGDALShow;
	m_pGDALShow = nullptr;
}

//DC可能随着窗口的改变而改变
CRect rcPicture;
GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(rcPicture);
ScreenToClient(rcPicture);

m_nDCWidth = rcPicture.Width();
m_nDCHeight = rcPicture.Height();

//宽度应为4的整数倍
m_nDCWidth = ((m_nDCWidth + 3) / 4) * 4;

if (m_hBmp)
{
	::DeleteObject(m_hBmp);
	m_hBmp = nullptr;
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

m_hBmp = CreateDIBSection(m_memDC.m_hDC, &bmpInfo, DIB_RGB_COLORS, (void **)&m_pFrontBuffer, NULL, 0);

m_bmp.Detach();
m_bmp.Attach(m_hBmp);

if (strFilePath.GetLength())
{
	std::string sFilePath = CStdStr::ws2s(CMfcStrFile::CString2string(strFilePath));
	m_pGDALShow = new CGDALShow(FromHandle(m_hWnd), sFilePath.c_str(), m_pFrontBuffer, m_nDCWidth, m_nDCHeight,
		TRUE, GetDlgItem(IDC_STATIC_PIC), &m_bmp, &m_memDC, &m_hBmp, &m_hOldBmp);
	m_pGDALShow->ShowImage(TRUE);
	m_pGDALShow->ThreadReadingData();
}

//PreTranslateMessage(MSG* pMsg)
CRect rect;
GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);

if (pMsg->message == WM_LBUTTONDOWN && m_pGDALShow && rect.PtInRect(pMsg->pt))
{
	m_pGDALShow->OnLButtonDown(pMsg->pt);
}

if (pMsg->message == WM_LBUTTONUP && m_pGDALShow && rect.PtInRect(pMsg->pt))
{
	m_pGDALShow->OnMButtonUp(pMsg->pt);
}

if (pMsg->message == WM_MBUTTONDOWN && m_pGDALShow && rect.PtInRect(pMsg->pt))
{
m_pGDALShow->OnMButtonDown(pMsg->pt);
}

if (pMsg->message == WM_MBUTTONUP && m_pGDALShow && rect.PtInRect(pMsg->pt))
{
m_pGDALShow->OnMButtonUp(pMsg->pt);
}

if (pMsg->message == WM_MOUSEMOVE && m_pGDALShow && rect.PtInRect(pMsg->pt))
{
	m_pGDALShow->OnMouseMove(pMsg->pt);
}

//OnMouseWheel()
if (m_pGDALShow)
{
	m_pGDALShow->OnMouseWheel(zDelta, pt);
}

//OnPaint()
CPaintDC dc(GetDlgItem(IDC_STATIC_PIC));
CRect rcclient;
GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rcclient);//获取控件的屏幕坐标
CWnd::DefWindowProc(WM_PAINT, (WPARAM)m_memDC.m_hDC, 0);

CBrush brush;
brush.CreatePatternBrush(&m_bmp);
dc.FillRect(rcclient, &brush);
dc.BitBlt(rcclient.left, rcclient.top, rcclient.Width(), rcclient.Height(), &m_memDC, rcclient.left, rcclient.top, SRCPAINT);
brush.DeleteObject();

//OnSize()若对话框尺寸可能发生改变
if (cx < 1 || cy < 1)
{
	return;
}

CWnd* pWnd = GetDlgItem(IDC_STATIC_PIC);
if (pWnd)
{
	CRect rect;
	pWnd->GetClientRect(&rect);
	m_nDCHeight = rect.Height();
	m_nDCWidth = rect.Width();
	m_nDCWidth = ((m_nDCWidth + 3) / 4) * 4;
}

if (m_pGDALShow)
{
	m_pGDALShow->m_nDCWidth = m_nDCWidth;
	m_pGDALShow->m_nDCHeight = m_nDCHeight;
}

size_t nDCMemSize = m_nDCWidth * m_nDCHeight * 3;

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

if (m_hBmp)
{
	SelectObject(m_memDC.m_hDC, m_hOldBmp);
	m_hOldBmp = NULL;
	DeleteObject(m_hBmp);
	m_hBmp = NULL;
}
m_hBmp = CreateDIBSection(m_memDC.m_hDC, &bmpInfo, DIB_RGB_COLORS, (void **)&m_pFrontBuffer, NULL, 0);
m_hOldBmp = (HBITMAP)::SelectObject(m_memDC.m_hDC, m_hBmp);

if (m_pGDALShow)
{
	//必须要更新这个buffer的指针
	m_pGDALShow->m_bUpdateScale = false;
	m_pGDALShow->m_pFrontBuffer = m_pFrontBuffer;
	m_pGDALShow->ThreadReadingData(FALSE);
	m_pGDALShow->ShowImage();
	m_pGDALShow->m_bUpdateScale = true;
}

/************************************************************************/

//视图中外部双缓存参考
/***********************************************************************
//View类中定义
CGDALShow* m_pGDALShow;
int m_nDCHeight;
int m_nDCWidth;
unsigned char* m_pFrontBuffer;

CBitmap m_bmp;
CDC m_memDC;
HBITMAP m_hBmp;
HBITMAP m_hOldBmp;

//构造函数中初始化
m_pGDALShow = nullptr;
m_nDCWidth = -1;
m_nDCHeight = -1;
m_pFrontBuffer = nullptr;

//析构函数中释放
if (m_pGDALShow)
{
	delete m_pGDALShow;
	m_pGDALShow = nullptr;
}

//OnCreate()
CDC *pDC = GetDC();
m_memDC.CreateCompatibleDC(pDC);
ReleaseDC(pDC);

//OnSize()
if (cx < 1 || cy < 1)
{
return;
}

m_nDCHeight = cy;
m_nDCWidth = cx;
m_nDCWidth = ((m_nDCWidth + 3) / 4) * 4;
if (m_pGDALShow)
{
m_pGDALShow->m_nDCWidth = m_nDCWidth;
m_pGDALShow->m_nDCHeight = m_nDCHeight;
}

size_t nDCMemSize = m_nDCWidth * m_nDCHeight * 3;

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

if (m_hBmp)
{
SelectObject(m_memDC.m_hDC, m_hOldBmp);
m_hOldBmp = NULL;
DeleteObject(m_hBmp);
m_hBmp = NULL;
}
m_hBmp = CreateDIBSection(m_memDC.m_hDC, &bmpInfo, DIB_RGB_COLORS, (void **)&m_pFrontBuffer, NULL, 0);
m_hOldBmp = (HBITMAP)::SelectObject(m_memDC.m_hDC, m_hBmp);

if (m_pGDALShow)
{
//必须要更新这个buffer的指针
m_pGDALShow->m_bUpdateScale = false;
m_pGDALShow->m_pFrontBuffer = m_pFrontBuffer;
m_pGDALShow->ThreadReadingData(FALSE);
m_pGDALShow->ShowImage();
m_pGDALShow->m_bUpdateScale = true;
}

//PreTranslateMessage(MSG* pMsg)
if (pMsg->message == WM_KEYDOWN && m_pGDALShow)
{
if (pMsg->wParam == VK_SPACE)
{
if (m_pGDALShow->m_nButtonMove == 2)
{
//切换移动图像的鼠标按键为左键
m_pGDALShow->m_nButtonMove = 1;
}
else
{	//恢复为中键
m_pGDALShow->m_nButtonMove = 2;
}
}
}
else
{
if (pMsg->message == WM_LBUTTONDOWN && m_pGDALShow)
{
m_pGDALShow->OnLButtonDown(pMsg->pt);
}

if (pMsg->message == WM_LBUTTONUP && m_pGDALShow)
{
m_pGDALShow->OnLButtonUp(pMsg->pt);
}

if (pMsg->message == WM_MBUTTONDOWN && m_pGDALShow)
{
m_pGDALShow->OnMButtonDown(pMsg->pt);
}

if (pMsg->message == WM_MBUTTONUP && m_pGDALShow)
{
m_pGDALShow->OnMButtonUp(pMsg->pt);
}

if (pMsg->message == WM_MOUSEMOVE && m_pGDALShow)
{
m_pGDALShow->OnMouseMove(pMsg->pt);
}
}

//OnMouseWheel()
if (m_pGDALShow)
{
	m_pGDALShow->OnMouseWheel(zDelta, pt);
}

//OnDraw()
if (m_pGDALShow)
{
	BitBlt(pDC->m_hDC, 0, 0, m_nDCWidth, m_nDCHeight, m_memDC.m_hDC, 0, 0, SRCCOPY);
}

//动作函数中
if (m_pGDALShow)
{
delete m_pGDALShow;
m_pGDALShow = nullptr;
}
m_pGDALShow = new CGDALShow(FromHandle(m_hWnd), CStdStr::ws2s(strFilePath).c_str(), m_pFrontBuffer, m_nDCWidth, m_nDCHeight);
m_pGDALShow->ShowImage();
m_pGDALShow->ThreadReadingData();

/************************************************************************/