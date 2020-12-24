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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "MediaTools.h"
#include "MainFrm.h"
#include "MediaToolsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_STATUSBAR_LINK	10086
#define ID_STATUSBAR_COOR	10087
#define ID_STATUSBAR_VALUE	10088

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_CHANGE_RIBBON_CATEGORY, &CMainFrame::OnAfxWmOnChangeRibbonCategory)
	ON_COMMAND(ID_BUTTON_CLASSIFYFILES, &CMainFrame::OnButtonClassifyfiles)
	ON_COMMAND(ID_BUTTON_RESIZEIMAGES, &CMainFrame::OnButtonResizeimages)
	ON_COMMAND(ID_BUTTON_CLONEEXIF, &CMainFrame::OnButtonCloneexif)
	ON_COMMAND(ID_BUTTON_CONVERTOR, &CMainFrame::OnButtonConvertor)
	ON_COMMAND(ID_BUTTON_MERGEIMGS, &CMainFrame::OnButtonMergeimgs)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CLASSIFYFILES, &CMainFrame::OnUpdateButtonClassifyfiles)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RESIZEIMAGES, &CMainFrame::OnUpdateButtonResizeimages)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CLONEEXIF, &CMainFrame::OnUpdateButtonCloneexif)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CONVERTOR, &CMainFrame::OnUpdateButtonConvertor)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MERGEIMGS, &CMainFrame::OnUpdateButtonMergeimgs)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
	m_pTb = nullptr;
}

CMainFrame::~CMainFrame()
{
	//保存配置文件
	WriteIniFile(GetIniPath(), m_cfg);

	if (m_pTb)
	{
		delete m_pTb;
		m_pTb = nullptr;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	SetActiveCategoryIndex();


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddElement(new CMFCRibbonProgressBar(ID_STATUSBAR_LINK, 300, 30), _T(""));
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_COOR, _T("X=--------------- Y=---------------"), TRUE), strTitlePane1);
	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_VALUE, _T("R=--------- G=--------- B=---------"), TRUE), strTitlePane1);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	_tstring strIniPath = GetIniPath();
	if (CStdFile::IfAccessFile(strIniPath))
	{
		ReadIniFile(strIniPath, m_cfg);
	}

	return 0;
}

CRect CMainFrame::MoveWindowToCenter(int nWinWidth /*= 800*/, int nWinHeight /*= 600*/)
{
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect rect(cx / 2 - nWinWidth / 2, cy / 2 - nWinHeight / 2, cx / 2 + nWinWidth / 2, cy / 2 + nWinHeight / 2);
	//屏幕居中
	MoveWindow(rect);

	return rect;
}


int CMainFrame::EndProgress()
{
	CMFCRibbonProgressBar* pStatusProgress = (CMFCRibbonProgressBar*)m_wndStatusBar.GetElement(1);
	if (pStatusProgress)
	{
		pStatusProgress->SetPos(0);
	}

	return 0;
}

int CMainFrame::HideAllImgToolsDlg()
{
	CMediaToolsView* pView = (CMediaToolsView*)GetActiveView();
	SetWindowResizeSwitch(true);
	if (pView)
	{
		if (m_Dm.IsReady())
		{
			m_Dm.HideAllDlgs();
		}
	}

	return 0;
}

int CMainFrame::InitTaskBarProgress()
{
	if (m_pTb == nullptr)
	{
		m_pTb = new CTaskBarProgress(m_hWnd);
	}

	return 0;
}

int CMainFrame::RestoreLastSelect()
{
	//读取ini文件之后
	int nCGI = m_wndRibbonBar.GetCategoryIndex(m_wndRibbonBar.GetActiveCategory());;
	switch (nCGI)
	{
	case 1:
		break;
	case 2:
		{
			switch (m_cfg.nImgToolIndex)
			{
			case 0:
				OnButtonClassifyfiles();
				break;
			case 1:
				OnButtonResizeimages();
				break;
			case 2:
				OnButtonCloneexif();
				break;
			case 3:
				OnButtonConvertor();
				break;
			case 4:
				OnButtonMergeimgs();
				break;
			default:
				break;
			}
		}
	case 3:
		break;
	default:
		break;
	}

	return m_cfg.nCateGoryIndex;
}

int CMainFrame::SetActiveCategoryIndex(const int& nCategoryIndex /*= 1*/)
{
	if (nCategoryIndex > 0)
	{
		m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(nCategoryIndex));
	}

	return 0;
}

int CMainFrame::SetProgressPos(int nPos, BOOL bRedraw /*= TRUE*/)
{
	CMFCRibbonProgressBar* pStatusProgress = (CMFCRibbonProgressBar*)m_wndStatusBar.GetElement(1);
	pStatusProgress->SetPos(nPos, bRedraw);

	return 0;
}

int CMainFrame::SetStatusBarText(int nID, const CString& strText)
{
	CMFCRibbonStatusBarPane* pStatusBar = (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetExElement(nID);

	if (pStatusBar)
	{
		pStatusBar->SetText(strText);
		pStatusBar->Redraw();
	}

	return 0;
}

int CMainFrame::SetWindowResizeSwitch(bool bEnable /*= true*/)
{
	LONG  Style;
	//获得窗口风格
	Style = ::GetWindowLong(m_hWnd, GWL_STYLE);
	//去除最大化功能，固定尺寸
	if (bEnable)
	{
		Style |= WS_MAXIMIZEBOX | WS_THICKFRAME;
	}
	else
	{
		Style &= ~(WS_MAXIMIZEBOX)&~WS_THICKFRAME;
	}
	::SetWindowLong(m_hWnd, GWL_STYLE, Style);

	return 0;
}

int CMainFrame::StartProgress(int nMin, int nMax)
{
	CMFCRibbonProgressBar* pStatusProgress = (CMFCRibbonProgressBar*)m_wndStatusBar.GetElement(1);
	if (pStatusProgress)
	{
		pStatusProgress->SetRange(nMin, nMax);
		pStatusProgress->SetPos(0);
	}

	return 0;
}

_tstring CMainFrame::GetIniPath()
{
	TCHAR chpath[MAX_PATH];
	GetModuleFileName(NULL, chpath, sizeof(chpath));

	_tstring strModulePath = CMfcStrFile::CString2string(chpath);
	_tstring strIniPath = CStdStr::ReplaceSuffix(strModulePath, _T(".ini"));

	return strIniPath;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	cs.style &= ~FWS_ADDTOTITLE;
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.lpszName = _T("媒体小工具");

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 强制关闭“打印预览”模式
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}

afx_msg LRESULT CMainFrame::OnAfxWmOnChangeRibbonCategory(WPARAM wParam, LPARAM lParam)
{
	m_cfg.nCateGoryIndex = m_wndRibbonBar.GetCategoryIndex(m_wndRibbonBar.GetActiveCategory());

	switch (m_cfg.nCateGoryIndex)
	{
	case 1:
		HideAllImgToolsDlg();
		SetStatusBarText(0, _T("点击打开或者直接拖拽文件！"));
		SetStatusBarText(1, _T("准备就绪！"));
		break;
	case 2:
		MoveWindowToCenter();
		SetWindowResizeSwitch(false);
		RestoreLastSelect();
		break;
	default:
		break;
	}

	return 0;
}

void CMainFrame::OnButtonClassifyfiles()
{
	// TODO: 在此添加命令处理程序代码
	CMediaToolsView* pView = (CMediaToolsView*)GetActiveView();
	if (pView)
	{
		if (m_Dm.IsReady())
		{
			m_Dm.HideAllDlgs();
			m_Dm.ShowClassifyDlg(true);
			m_cfg.nImgToolIndex = 0;
		}
	}
}

void CMainFrame::OnButtonResizeimages()
{
	// TODO: 在此添加命令处理程序代码
	CMediaToolsView* pView = (CMediaToolsView*)GetActiveView();
	if (pView)
	{
		if (m_Dm.IsReady())
		{
			m_Dm.HideAllDlgs();
			m_Dm.ShowResizeDlg(true);
			m_cfg.nImgToolIndex = 1;
		}
	}
}

void CMainFrame::OnButtonCloneexif()
{
	// TODO: 在此添加命令处理程序代码
	CMediaToolsView* pView = (CMediaToolsView*)GetActiveView();
	if (pView)
	{
		if (m_Dm.IsReady())
		{
			m_Dm.HideAllDlgs();
			m_Dm.ShowCloneExifDlg(true);
			m_cfg.nImgToolIndex = 2;
		}
	}
}

void CMainFrame::OnButtonConvertor()
{
	// TODO: 在此添加命令处理程序代码
	CMediaToolsView* pView = (CMediaToolsView*)GetActiveView();
	if (pView)
	{
		if (m_Dm.IsReady())
		{
			m_Dm.HideAllDlgs();
			m_Dm.ShowConvertorfDlg(true);
			m_cfg.nImgToolIndex = 3;
		}
	}
}

void CMainFrame::OnButtonMergeimgs()
{
	// TODO: 在此添加命令处理程序代码
	CMediaToolsView* pView = (CMediaToolsView*)GetActiveView();
	if (pView)
	{
		if (m_Dm.IsReady())
		{
			m_Dm.HideAllDlgs();
			m_Dm.ShowMergeImgsDlg(true);
			m_cfg.nImgToolIndex = 4;
		}
	}
}


void CMainFrame::OnUpdateButtonClassifyfiles(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_cfg.nImgToolIndex != 0);
}


void CMainFrame::OnUpdateButtonResizeimages(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_cfg.nImgToolIndex != 1);
}


void CMainFrame::OnUpdateButtonCloneexif(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_cfg.nImgToolIndex != 2);
}


void CMainFrame::OnUpdateButtonConvertor(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_cfg.nImgToolIndex != 3);
}


void CMainFrame::OnUpdateButtonMergeimgs(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_cfg.nImgToolIndex != 4);
}
