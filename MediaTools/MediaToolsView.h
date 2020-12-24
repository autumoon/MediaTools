// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MediaToolsView.h : CMediaToolsView 类的接口
//

#pragma once
#include "gdal/GDALShow.h"
#include "src/StdStrFile.h"
#include "src/MfcStrFile.h"

class CMediaToolsDoc;

class CMediaToolsView : public CView
{
protected: // 仅从序列化创建
	CMediaToolsView();
	DECLARE_DYNCREATE(CMediaToolsView)

// 特性
public:
	CMediaToolsDoc* GetDocument() const;

// 操作
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

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMediaToolsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // MediaToolsView.cpp 中的调试版本
inline CMediaToolsDoc* CMediaToolsView::GetDocument() const
   { return reinterpret_cast<CMediaToolsDoc*>(m_pDocument); }
#endif

