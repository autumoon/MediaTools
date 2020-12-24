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

// MediaToolsView.h : CMediaToolsView ��Ľӿ�
//

#pragma once
#include "gdal/GDALShow.h"
#include "src/StdStrFile.h"
#include "src/MfcStrFile.h"

class CMediaToolsDoc;

class CMediaToolsView : public CView
{
protected: // �������л�����
	CMediaToolsView();
	DECLARE_DYNCREATE(CMediaToolsView)

// ����
public:
	CMediaToolsDoc* GetDocument() const;

// ����
public:
	bool m_bRealValue;
	bool m_bYReverse;

	CGDALShow* m_pGDALShow;
	int m_nDCHeight;
	int m_nDCWidth;
	unsigned char* m_pFrontBuffer;

	CBitmap m_bmp;
	CDC m_memDC;
	HBITMAP m_hBmp;
	HBITMAP m_hOldBmp;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMediaToolsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	int InitializeFile(_tstring strFilePath);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateCheckRealpixvalue(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckYreverse(CCmdUI *pCmdUI);
	afx_msg void OnCheckRealpixvalue();
	afx_msg void OnCheckYreverse();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MediaToolsView.cpp �еĵ��԰汾
inline CMediaToolsDoc* CMediaToolsView::GetDocument() const
   { return reinterpret_cast<CMediaToolsDoc*>(m_pDocument); }
#endif

