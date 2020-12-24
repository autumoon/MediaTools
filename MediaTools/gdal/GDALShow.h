//[5/25/2018 autumoon]

#pragma once
#include "GdalUser.h"

//MEM_ALLOC_X��MEM_ALLOC_X������1����3��Ϊ1ʱ��ʡ�ڴ棬Ϊ3ʱ�϶��������
#define	MEM_ALLOC_X			3
#define	MEM_ALLOC_Y			3
#define	MEM_ALLOC_NUM		(MEM_ALLOC_X * MEM_ALLOC_Y)
#define DC_OFFSET_NUM_X		((MEM_ALLOC_X - 1) >> 1)
#define DC_OFFSET_NUM_Y		((MEM_ALLOC_Y - 1) >> 1)

#define AU_MIN(x,y)	((x) < (y) ? (x) : (y))
#define AU_MAX(x,y)	((x) > (y) ? (x) : (y))

//����ʹ���ⲿ˫����
class CGDALShow
{
public:
	//������ǶԻ��򣬵ڶ��в���ȫ��ȡĬ��ֵ����
	CGDALShow(CWnd* pWnd, const char* imgPath, unsigned char* pFrontBuffer, int nDCWidth, int nDCHeight,
		BOOL bIsDlg = FALSE, CWnd* pCtrlWnd = nullptr, CBitmap* pbmp = nullptr, CDC* pmemDC = nullptr, HBITMAP* phBmp = nullptr, HBITMAP* phOldBmp = nullptr);
	~CGDALShow(void);
public:
	bool m_bIsLBtnDown;							//����Ƿ���
	bool m_bIsMBtnDown;							//�м��Ƿ���
	bool m_bUpdateScale;						//�Ƿ���Ҫ���±���
	CBitmap* m_pbmp;							//�ⲿ��ʾʹ�õı������ߴ�仯ʱ�ڲ����ܸ���
	CDC* m_pmemDC;
	CWnd* m_pCtrlWnd;
	HBITMAP* m_phBmp;
	HBITMAP* m_phOldBmp;
	int m_nDCWidth;								//��ʾ����Ŀ��
	int m_nDCHeight;							//��ʾ����ĸ߶�

	unsigned char* m_pFrontBuffer;				//�ڴ�buffer
	unsigned char* m_pFrontBufferDrag;			//DCbuffer���ݣ������϶���ʱ�򻺳�
	unsigned char* m_pFrontBufferDragThread;	//DCbuffer���ݣ������̶߳�ȡ

	//���ݿؼ����꣬��ȡ��Ϣ
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

	static double m_dMaxScale;					//����������
	static double m_dMinScale;					//������С����
	static double m_dScaleStep;					//���Ų���ֵ
	static double m_dXoffset;					//��ʾ��������x
	static double m_dYoffset;					//��ʾ��������y
	static int m_nBgValue;						//����ֵ
	static int m_nButtonMove;					//ʹ����갴ť�ƶ� 1������� 2�����м�
	
protected:
	bool m_bIsDlg;
	bool m_bDragDataReady;
	CGdalUser* m_pGu;
	double* m_pHist;					//ֱ��ͼͳ��
	double* m_plfCount;					//�ۼƸ���
	double m_dScale;					//��ǰ����
	double m_dScaleLast;				//��һ�α���
	double m_dIniScale;					//����������
	unsigned char* m_pBufferPy;			//������Buffer
	unsigned char* m_pClrLut;			//��ɫ���ұ�

	bool CreatePyramid();
	bool GetRealPixValue(int nCtrlX, int nCtrlY, std::vector<int>& vRealValues);
	int GetPointerForDlgShow(CWnd* pDlgWnd, CBitmap* pbmp, CDC* pmemDC, HBITMAP* phBmp, HBITMAP* phOldBmp);
	int ResampleArray(int nSrcBufWidth, int nSrcBufHeight, int nSrcLeft, int nSrcTop, int nSrcRight, int nSrcBottom, unsigned char* pSrc,
		int nDstBufWidth, int nDstBufHeight, int nDstLeft, int nDstTop, int nDstRight, int nDstBottom, unsigned char* pDst);
	int ResampleArray2BGR(int nSrcBufWidth, int nSrcBufHeight, int nSrcLeft, int nSrcTop, int nSrcRight, int nSrcBottom, unsigned char* pSrc, int nSrcBandNum,
		int nDstBufWidth, int nDstBufHeight, int nDstLeft, int nDstTop, int nDstRight, int nDstBottom, unsigned char* pDst, int nDstBandNum,
		int nSizeOfPointer = 1, int nSrcSkip = 0, int nDestSkip = 0);

	//�����̶߳�ȡ��������
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

//�Ի����ⲿ˫����
/************************************************************************
//Dlg�ж���
CGDALShow* m_pGDALShow;
int m_nDCHeight;
int m_nDCWidth;
unsigned char* m_pFrontBuffer;

CBitmap m_bmp;
CDC m_memDC;
HBITMAP m_hBmp;
HBITMAP m_hOldBmp;

//���캯���г�ʼ��
m_pGDALShow = nullptr;
m_nDCWidth = -1;
m_nDCHeight = -1;
m_pFrontBuffer = nullptr;

//�����������ͷ�
if (m_pGDALShow)
{
	delete m_pGDALShow;
	m_pGDALShow = nullptr;
}

//���static�ؼ�����picturectrl�ؼ�
//������ť�������������
CString strFilePath = CMfcStrFile::OpenFile();

if (m_pGDALShow)
{
	delete m_pGDALShow;
	m_pGDALShow = nullptr;
}

//DC�������Ŵ��ڵĸı���ı�
CRect rcPicture;
GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(rcPicture);
ScreenToClient(rcPicture);

m_nDCWidth = rcPicture.Width();
m_nDCHeight = rcPicture.Height();

//���ӦΪ4��������
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
GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rcclient);//��ȡ�ؼ�����Ļ����
CWnd::DefWindowProc(WM_PAINT, (WPARAM)m_memDC.m_hDC, 0);

CBrush brush;
brush.CreatePatternBrush(&m_bmp);
dc.FillRect(rcclient, &brush);
dc.BitBlt(rcclient.left, rcclient.top, rcclient.Width(), rcclient.Height(), &m_memDC, rcclient.left, rcclient.top, SRCPAINT);
brush.DeleteObject();

//OnSize()���Ի���ߴ���ܷ����ı�
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
	//����Ҫ�������buffer��ָ��
	m_pGDALShow->m_bUpdateScale = false;
	m_pGDALShow->m_pFrontBuffer = m_pFrontBuffer;
	m_pGDALShow->ThreadReadingData(FALSE);
	m_pGDALShow->ShowImage();
	m_pGDALShow->m_bUpdateScale = true;
}

/************************************************************************/

//��ͼ���ⲿ˫����ο�
/***********************************************************************
//View���ж���
CGDALShow* m_pGDALShow;
int m_nDCHeight;
int m_nDCWidth;
unsigned char* m_pFrontBuffer;

CBitmap m_bmp;
CDC m_memDC;
HBITMAP m_hBmp;
HBITMAP m_hOldBmp;

//���캯���г�ʼ��
m_pGDALShow = nullptr;
m_nDCWidth = -1;
m_nDCHeight = -1;
m_pFrontBuffer = nullptr;

//�����������ͷ�
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
//����Ҫ�������buffer��ָ��
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
//�л��ƶ�ͼ�����갴��Ϊ���
m_pGDALShow->m_nButtonMove = 1;
}
else
{	//�ָ�Ϊ�м�
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

//����������
if (m_pGDALShow)
{
delete m_pGDALShow;
m_pGDALShow = nullptr;
}
m_pGDALShow = new CGDALShow(FromHandle(m_hWnd), CStdStr::ws2s(strFilePath).c_str(), m_pFrontBuffer, m_nDCWidth, m_nDCHeight);
m_pGDALShow->ShowImage();
m_pGDALShow->ThreadReadingData();

/************************************************************************/