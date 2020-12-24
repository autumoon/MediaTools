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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "src\ProgressInterface.h"
#include "DlgManager.h"
#include "src\Configure.h"

class CMediaToolsView;
class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	CDlgManager m_Dm;
	CTaskBarProgress* m_pTb;
	config_s m_cfg;
// ����
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
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
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


