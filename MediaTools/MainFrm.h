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

// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "src\ProgressInterface.h"
#include "DlgManager.h"
#include "src\Configure.h"

class CMediaToolsView;
class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	CDlgManager m_Dm;
	CTaskBarProgress* m_pTb;
	config_s m_cfg;
// 操作
public:
	CRect MoveWindowToCenter(int nWinWidth = 800, int nWinHeight = 600);

	int EndProgress();
	int HideAllImgToolsDlg();
	int InitTaskBarProgress();
	int RestoreLastSelect();
	int SetActiveCategoryIndex(const int& nCategoryIndex = 1);
	int SetProgressPos(int nPos, BOOL bRedraw = TRUE);
	int SetStatusBarText(int nID, const CString& strText);
	int SetWindowResizeSwitch(bool bEnable = true);
	int StartProgress(int nMin, int nMax);

	_tstring GetIniPath();
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAfxWmOnChangeRibbonCategory(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnButtonClassifyfiles();
	afx_msg void OnButtonResizeimages();
	afx_msg void OnButtonCloneexif();
	afx_msg void OnButtonConvertor();
	afx_msg void OnButtonMergeimgs();
	afx_msg void OnUpdateButtonClassifyfiles(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonResizeimages(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonCloneexif(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonConvertor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonMergeimgs(CCmdUI *pCmdUI);
};


