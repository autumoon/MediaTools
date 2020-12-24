#include "StdAfx.h"
#include "DlgManager.h"
#include "MainFrm.h"


CDlgManager::CDlgManager(void)
{
	m_pMainFrame = nullptr;
	m_pMTView = nullptr;

	m_pClassifyDlg = nullptr;
	m_pResizeDlg = nullptr;
	m_pCloneExifDlg = nullptr;
	m_pConvertorDlg = nullptr;
	m_pMergeImgsDlg = nullptr;
}


CDlgManager::~CDlgManager(void)
{
	CStdTpl::DelPointerSafely(m_pClassifyDlg);
	CStdTpl::DelPointerSafely(m_pResizeDlg);
	CStdTpl::DelPointerSafely(m_pCloneExifDlg);
	CStdTpl::DelPointerSafely(m_pConvertorDlg);
	CStdTpl::DelPointerSafely(m_pMergeImgsDlg);
}

bool CDlgManager::IsReady()
{
	//两个指针都不能为空
	return m_pMainFrame && m_pMTView;
}

bool CDlgManager::Initialize(CMainFrame* pMainFrame, CMediaToolsView* pMTView)
{
	m_pMainFrame = pMainFrame;
	m_pMTView = pMTView;

	return IsReady();
}

BOOL CDlgManager::FlashWindow(BOOL bInvert)
{
	m_pMainFrame->FlashWindow(bInvert);

	return TRUE;
}

int CDlgManager::GetIniConfig(config_s& cfg)
{
	if (m_pMainFrame)
	{
		cfg = m_pMainFrame->m_cfg;
	}

	return 0;
}

int CDlgManager::HideAllDlgs()
{
	if (IsReady())
	{
		if (m_pClassifyDlg)
		{
			m_pClassifyDlg->ShowWindow(SW_HIDE);
		}

		if (m_pResizeDlg)
		{
			m_pResizeDlg->ShowWindow(SW_HIDE);
		}

		if (m_pCloneExifDlg)
		{
			m_pCloneExifDlg->ShowWindow(SW_HIDE);
		}

		if (m_pConvertorDlg)
		{
			m_pConvertorDlg->ShowWindow(SW_HIDE);
		}

		if (m_pMergeImgsDlg)
		{
			m_pMergeImgsDlg->ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

int CDlgManager::SetBarInfo(const _tstring& strInfo, size_t nIndex /*= 0*/)
{
	m_pMainFrame->SetStatusBarText(nIndex, strInfo.c_str());

	return 0;
}

int CDlgManager::SetIniConfig(const config_s& cfg)
{
	if (m_pMainFrame)
	{
		m_pMainFrame->m_cfg = cfg;
	}

	return 0;
}

int CDlgManager::ShowClassifyDlg(bool bShow /*= true*/)
{
	if (IsReady())
	{
		if (bShow)
		{
			CRect rc;
			m_pMTView->GetClientRect(&rc);
			if (m_pClassifyDlg == nullptr)
			{
				m_pClassifyDlg = new CClassifyFiles(m_pMTView);
				m_pClassifyDlg->m_pDlgManager = this;
				m_pClassifyDlg->Create(IDD_DIALOG_CLASSIFYFILES, m_pMTView);
				m_pClassifyDlg->MoveWindow(&rc);
			}
			m_pClassifyDlg->ShowWindow(SW_SHOW);
			CString strTmp[2];
			strTmp[0].LoadString(IDS_STRING_LOGTIPS);
			strTmp[1].LoadString(IDS_STRING_READY);
			for (size_t i = 0; i < 2; ++i)
			{
				SetBarInfo(CMfcStrFile::CString2string(strTmp[i]), i);
			}
		}
		else
		{
			m_pClassifyDlg->ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

int CDlgManager::ShowCloneExifDlg(bool bShow /*= true*/)
{
	if (IsReady())
	{
		if (bShow)
		{
			CRect rc;
			m_pMTView->GetClientRect(&rc);
			if (m_pCloneExifDlg == nullptr)
			{
				m_pCloneExifDlg = new CCloneExif(m_pMTView);
				m_pCloneExifDlg->m_pDlgManager = this;
				m_pCloneExifDlg->Create(IDD_DIALOG_CLONEEXIF, m_pMTView);
				m_pCloneExifDlg->MoveWindow(&rc);
			}
			m_pCloneExifDlg->ShowWindow(SW_SHOW);
			CString strTmp[2];
			strTmp[0].LoadString(IDS_STRING_EXIFINFO);
			strTmp[1].LoadString(IDS_STRING_READY);
			for (size_t i = 0; i < 2; ++i)
			{
				SetBarInfo(CMfcStrFile::CString2string(strTmp[i]), i);
			}
		}
		else
		{
			m_pCloneExifDlg->ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

int CDlgManager::ShowConvertorfDlg(bool bShow /*= true*/)
{
	if (IsReady())
	{
		if (bShow)
		{
			CRect rc;
			m_pMTView->GetClientRect(&rc);
			if (m_pConvertorDlg == nullptr)
			{
				m_pConvertorDlg = new CConvertor(m_pMTView);
				m_pConvertorDlg->m_pDlgManager = this;
				m_pConvertorDlg->Create(IDD_DIALOG_CONVERTOR, m_pMTView);
				m_pConvertorDlg->MoveWindow(&rc);
			}
			m_pConvertorDlg->ShowWindow(SW_SHOW);
			CString strTmp[2];
			strTmp[0].LoadString(IDS_STRING_CONVERTORTIPS);
			strTmp[1].LoadString(IDS_STRING_READY);
			for (size_t i = 0; i < 2; ++i)
			{
				SetBarInfo(CMfcStrFile::CString2string(strTmp[i]), i);
			}
		}
		else
		{
			m_pConvertorDlg->ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

int CDlgManager::ShowResizeDlg(bool bShow /*= true*/)
{
	if (IsReady())
	{
		if (bShow)
		{
			CRect rc;
			m_pMTView->GetClientRect(&rc);
			if (m_pResizeDlg == nullptr)
			{
				m_pResizeDlg = new CResizePictures(m_pMTView);
				m_pResizeDlg->m_pDlgManager = this;
				m_pResizeDlg->Create(IDD_DIALOG_RESIZEPICS, m_pMTView);
				m_pResizeDlg->MoveWindow(&rc);
			}
			m_pResizeDlg->ShowWindow(SW_SHOW);
			CString strTmp[2];
			strTmp[0].LoadString(IDS_STRING_AUTOSEL);
			strTmp[1].LoadString(IDS_STRING_READY);
			for (size_t i = 0; i < 2; ++i)
			{
				SetBarInfo(CMfcStrFile::CString2string(strTmp[i]), i);
			}
		}
		else
		{
			m_pResizeDlg->ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

int CDlgManager::ShowMergeImgsDlg(bool bShow /*= true*/)
{
	if (IsReady())
	{
		if (bShow)
		{
			CRect rc;
			m_pMTView->GetClientRect(&rc);
			if (m_pMergeImgsDlg == nullptr)
			{
				m_pMergeImgsDlg = new CMergeImgs(m_pMTView);
				m_pMergeImgsDlg->m_pDlgManager = this;
				m_pMergeImgsDlg->Create(IDD_DIALOG_MERGEIMGS, m_pMTView);
				m_pMergeImgsDlg->MoveWindow(&rc);
			}
			m_pMergeImgsDlg->ShowWindow(SW_SHOW);
			CString strTmp[2];
			strTmp[0].LoadString(IDS_STRING_MERGETIPS);
			strTmp[1].LoadString(IDS_STRING_READY);
			for (size_t i = 0; i < 2; ++i)
			{
				SetBarInfo(CMfcStrFile::CString2string(strTmp[i]), i);
			}
		}
		else
		{
			m_pMergeImgsDlg->ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

int CDlgManager::StartMainProgress()
{
	if (m_pMainFrame)
	{
		m_pMainFrame->StartProgress(0, 100);
	}

	return 0;
}

int CDlgManager::MiddleMainProgress(const int& nMiddle /*= 50*/)
{
	if (m_pMainFrame)
	{
		m_pMainFrame->SetProgressPos(nMiddle);
	}

	return 0;
}

int CDlgManager::EndMainProgress()
{
	if (m_pMainFrame)
	{
		m_pMainFrame->SetProgressPos(100);
	}

	return 0;
}

_tstring CDlgManager::GetIniPath()
{
	if (IsReady())
	{
		return m_pMainFrame->GetIniPath();
	}

	return _tstring();
}
