// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MediaToolsView.cpp : CMediaToolsView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MediaTools.h"
#endif

#include "MediaToolsDoc.h"
#include "MediaToolsView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMediaToolsView

IMPLEMENT_DYNCREATE(CMediaToolsView, CView)

BEGIN_MESSAGE_MAP(CMediaToolsView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMediaToolsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_CHECK_REALPIXVALUE, &CMediaToolsView::OnUpdateCheckRealpixvalue)
	ON_UPDATE_COMMAND_UI(ID_CHECK_YREVERSE, &CMediaToolsView::OnUpdateCheckYreverse)
	ON_COMMAND(ID_CHECK_REALPIXVALUE, &CMediaToolsView::OnCheckRealpixvalue)
	ON_COMMAND(ID_CHECK_YREVERSE, &CMediaToolsView::OnCheckYreverse)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CMediaToolsView ����/����

CMediaToolsView::CMediaToolsView()
{
	// TODO: �ڴ˴���ӹ������
	m_bRealValue = false;
	m_bYReverse = false;

	m_pGDALShow = nullptr;
	m_nDCWidth = -1;
	m_nDCHeight = -1;
	m_pFrontBuffer = nullptr;
}

CMediaToolsView::~CMediaToolsView()
{
	CStdTpl::DelPointerSafely(m_pGDALShow);
}

BOOL CMediaToolsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMediaToolsView ����

void CMediaToolsView::OnDraw(CDC* pDC)
{
	CMediaToolsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (m_pGDALShow)
	{
		BitBlt(pDC->m_hDC, 0, 0, m_nDCWidth, m_nDCHeight, m_memDC.m_hDC, 0, 0, SRCCOPY);
	}
}


// CMediaToolsView ��ӡ


void CMediaToolsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMediaToolsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMediaToolsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMediaToolsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMediaToolsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMediaToolsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

int CMediaToolsView::InitializeFile(_tstring strFilePath)
{
	CStdTpl::DelPointerSafely(m_pGDALShow);

	m_pGDALShow = new CGDALShow(FromHandle(m_hWnd), CStdStr::ws2s(strFilePath).c_str(), m_pFrontBuffer, m_nDCWidth, m_nDCHeight);
	m_pGDALShow->ShowImage();
	m_pGDALShow->ThreadReadingData();

	return TRUE;
}

// CShowInViewView ���

#ifdef _DEBUG
void CMediaToolsView::AssertValid() const
{
	CView::AssertValid();
}

void CMediaToolsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMediaToolsDoc* CMediaToolsView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMediaToolsDoc)));
	return (CMediaToolsDoc*)m_pDocument;
}
#endif //_DEBUG


// CMediaToolsView ��Ϣ�������


int CMediaToolsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CDC *pDC = GetDC();
	m_memDC.CreateCompatibleDC(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CMediaToolsView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
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
}


BOOL CMediaToolsView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//PreTranslateMessage(MSG* pMsg)
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
	return CView::PreTranslateMessage(pMsg);
}


BOOL CMediaToolsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pGDALShow)
	{
		m_pGDALShow->OnMouseWheel(zDelta, pt);
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMediaToolsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��ȡ��ǰ����������ֵ
	if (m_pGDALShow)
	{
		std::vector<int> values(4, -1);
		if (m_pGDALShow->GetPixValue(point.x, point.y, values, m_bRealValue))
		{
			//״̬����ʾ
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

			CString strValue[2];
			int nImgX = 0, nImgY = 0;
			m_pGDALShow->GetImgCoors(point.x, point.y, nImgX, nImgY, m_bYReverse);
			strValue[0].Format(_T("X=%6lf Y=%6lf"), (float)nImgX, (float)nImgY);
			strValue[1].Format(_T("R=%d G=%d B=%d"), values[0], values[1], values[2]);

			for (int i = 0; i < 2; ++i)
			{
				pMainFrame->SetStatusBarText(i, strValue[i]);
			}
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CMediaToolsView::OnUpdateCheckRealpixvalue(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (m_pGDALShow)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(m_bRealValue);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMediaToolsView::OnUpdateCheckYreverse(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (m_pGDALShow)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(m_bYReverse);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMediaToolsView::OnCheckRealpixvalue()
{
	// TODO: �ڴ���������������
	m_bRealValue = !m_bRealValue;
}

void CMediaToolsView::OnCheckYreverse()
{
	// TODO: �ڴ���������������
	m_bYReverse = !m_bYReverse;
}


void CMediaToolsView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CMediaToolsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CString strImgPath = CMfcStrFile::OpenFile();
	if (strImgPath.GetLength())
	{
		pDoc->OnOpenDocument(strImgPath);
	}

	CView::OnLButtonDblClk(nFlags, point);
}
