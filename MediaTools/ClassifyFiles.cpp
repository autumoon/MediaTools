// ClassifyFiles.cpp : 实现文件
//

#include "stdafx.h"
#include "ClassifyFiles.h"
#include "afxdialogex.h"
#include "src\Configure.h"
#include "DlgManager.h"
#include "MainFrm.h"

#define THREAD_NUMS		1

// ClassifyFiles 对话框

IMPLEMENT_DYNAMIC(CClassifyFiles, CDialog)

CClassifyFiles::CClassifyFiles(CWnd* pParent /*=NULL*/)
	: CDialog(CClassifyFiles::IDD, pParent)
{
	m_pDlgManager = nullptr;
}

CClassifyFiles::~CClassifyFiles()
{
}

void CClassifyFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClassifyFiles, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DIR, &CClassifyFiles::OnBnClickedButtonDir)
	ON_BN_CLICKED(IDC_RADIO_PICS, &CClassifyFiles::OnBnClickedRadioPics)
	ON_BN_CLICKED(IDC_RADIO_FILES, &CClassifyFiles::OnBnClickedRadioFiles)
	ON_BN_CLICKED(IDC_RADIO_REC, &CClassifyFiles::OnBnClickedRadioRec)
	ON_BN_CLICKED(IDOK_CF, &CClassifyFiles::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL_CF, &CClassifyFiles::OnBnClickedCancel)
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// ClassifyFiles 消息处理程序
BOOL CClassifyFiles::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_pDlgManager)
	{
		//读取配置
		m_pDlgManager->GetIniConfig(m_cfg);
	}
	if (m_cfg.cf_bRemLastDir)
	{
		SetDlgItemText(IDC_EDIT_DIR, m_cfg.cf_strLastDir.c_str());
	}

	//根据配置文件设置窗口状态
	SetDlgItemText(IDC_EDIT_CUSTOM, m_cfg.cf_strCustom.c_str());
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->SetCheck(m_cfg.cf_bPics[0]);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->SetCheck(m_cfg.cf_bPics[1]);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->SetCheck(m_cfg.cf_bPics[2]);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->SetCheck(m_cfg.cf_bPics[3]);
	((CButton*)GetDlgItem(IDC_RADIO_PICS))->SetCheck(TRUE);

	if (m_cfg.cf_nMethod == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->SetCheck(TRUE);
	}
	else if (m_cfg.cf_nMethod == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->SetCheck(TRUE);
	}
	
	if(m_cfg.cf_nSelIndex == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_FILES))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_PICS))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_BMP))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_JPG))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_PNG))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_GIF))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->EnableWindow(FALSE);
	}

	GetClientRect(m_rect);
	m_cfg.nCateGoryIndex = IMGTOOL_CATE_INDEX;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CClassifyFiles::OnBnClickedButtonDir()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strDir = CMfcStrFile::BrowseDir();

	if (strDir.GetLength())
	{
		SetDlgItemText(IDC_EDIT_DIR, strDir);
	}
	else
	{
		SetFocus();
	}
}


void CClassifyFiles::OnBnClickedRadioPics()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->EnableWindow(TRUE);
}


void CClassifyFiles::OnBnClickedRadioFiles()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->EnableWindow(FALSE);
}


void CClassifyFiles::OnBnClickedRadioRec()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->EnableWindow(FALSE);
}

BOOL CClassifyFiles::ShowWarnnings()
{
	return MessageBox(_T("1.本程序具有一定的危险性，请务必小心操作。\n2.所选目录指定类型的文件将全部重新排列！请确保所选目录正确！\n3.万一操作错误，请使用本程序自带的恢复功能！\n\n是否继续？"), 
		_T("提示"), MB_YESNO) == IDYES;

}

/******************************全局变量**********************************/
CString g_strDir;
static CMutex g_Mutex;
static std::map<_tstring, _tstring> g_mLogs;
static _tstring g_strSubDirName = _T("");
/************************************************************************/

int CClassifyFiles::MoveFilesByTime(const _tstring& sStr, const _tstring& sSavePath, const size_t& nProcessIndex)
{
	_tstring strDir = CMfcStrFile::CString2string(g_strDir);
	_tstring strRootDir = CStdStr::AddSlashIfNeeded(strDir) + g_strSubDirName;

	if (_access(CStdStr::ws2s(strRootDir).c_str(), 0) != 0 && !CreateDirectory(CString(strRootDir.c_str()), NULL))
	{
		return -1;
	}

	CString strLastWriteTime = GetModifyYearMonth(CString(sStr.c_str()));
	_tstring strYearMonth = CMfcStrFile::CString2string(strLastWriteTime);

	_tstring strSaveDir = strRootDir + strYearMonth;

	if (_access(CStdStr::ws2s(strSaveDir).c_str(), 0) != 0 && !CreateDirectory(CString(strSaveDir.c_str()), NULL))
	{
		return -1;
	}

	_tstring strSavePath = CStdStr::AddSlashIfNeeded(strSaveDir) + CStdStr::GetNameOfFile(sStr);
	g_Mutex.Lock();
	int nNum = 0;
	if (CStdFile::IfAccessFile(strSavePath))
	{
		do
		{
			++nNum;
			strSavePath = CStdStr::AddSlashIfNeeded(strSaveDir) + CStdStr::GetNameOfFile(sStr, false) + 
				CStdTpl::ConvertToString(nNum) + CStdStr::GetSuffixOfFile(sStr);

		} while (CStdFile::IfAccessFile(strSavePath));
	}

	if (rename(CStdStr::ws2s(sStr).c_str(), CStdStr::ws2s(strSavePath).c_str()) < 0)
	{
		rename(CStdStr::ws2s(sStr).c_str(), CStdStr::ws2s(strSavePath).c_str());
	}

	g_mLogs.insert(std::make_pair(sStr, strSavePath));
	g_Mutex.Unlock();

	return 0;
}


int CClassifyFiles::MovePicsByResolution(const _tstring& strSrcFile, const _tstring& pSavePath, const size_t& nProcessIndex)
{
	_tstring strDir = CMfcStrFile::CString2string(g_strDir);
	_tstring strRootDir = CStdStr::AddSlashIfNeeded(strDir) + g_strSubDirName;

	if (_access(CStdStr::ws2s(strRootDir).c_str(), 0) != 0 && !CreateDirectory(CString(strRootDir.c_str()), NULL))
	{
		return -1;
	}

	const std::string& strSuffix = CStdStr::ws2s(CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(strSrcFile, false)));
	ENUM_CXIMAGE_FORMATS dstFormat = (ENUM_CXIMAGE_FORMATS)CxImage::GetTypeIdFromName(CStdStr::s2ws(strSuffix).c_str());

	CxImage  image; 
	image.Load(strSrcFile.c_str(), dstFormat); 
	image.RotateExif();
	_tstring strResolution;
	if (image.IsValid())
	{
		strResolution = CStdTpl::ConvertToString(image.GetWidth()) + _T("_") + CStdTpl::ConvertToString(image.GetHeight());
	}
	else
	{
		return -1;
	}

	_tstring strSaveDir = strRootDir + strResolution;

	if (_access(CStdStr::ws2s(strSaveDir).c_str(), 0) != 0 && !CreateDirectory(CString(strSaveDir.c_str()), NULL))
	{
		return -1;
	}

	_tstring strSavePath = CStdStr::AddSlashIfNeeded(strSaveDir) + CStdStr::GetNameOfFile(strSrcFile);
	g_Mutex.Lock();
	if (CStdFile::IfAccessFile(strSavePath))
	{
		int nNum = 0;
		do
		{
			++nNum;
			strSavePath = CStdStr::AddSlashIfNeeded(strSaveDir) + CStdStr::GetNameOfFile(strSrcFile, false) + 
				CStdTpl::ConvertToString(nNum) + CStdStr::GetSuffixOfFile(strSrcFile);

		} while (CStdFile::IfAccessFile(strSavePath));
	}

	if (strSavePath != strSrcFile)
	{
		rename(CStdStr::ws2s(strSrcFile).c_str(), CStdStr::ws2s(strSavePath).c_str());
	}

	g_mLogs.insert(std::make_pair(strSrcFile, strSavePath));
	g_Mutex.Unlock();

	return 0;
}

int CClassifyFiles::MovePicsByTime(const _tstring& sStr, const _tstring& sSavePath, const size_t& nProcessIndex)
{
	_tstring strDir = CMfcStrFile::CString2string(g_strDir);
	_tstring strRootDir = CStdStr::AddSlashIfNeeded(strDir) + g_strSubDirName;

	if (_access(CStdStr::ws2s(strRootDir).c_str(), 0) != 0 && !CreateDirectory(CString(strRootDir.c_str()), NULL))
	{
		return -1;
	}

	_tstring strPicTime;
	_tstring strYearMonth;

	if (CxImageUser::GetExifTime(sStr, strPicTime))
	{
		strYearMonth = strPicTime.substr(0, 7);
		strYearMonth = CStdStr::RepalceAll(strYearMonth, _T(":"));
	}
	else
	{
		CString strLastWriteTime = GetModifyYearMonth(CString(sStr.c_str()));
		strYearMonth = CMfcStrFile::CString2string(strLastWriteTime);
	}

	_tstring strSaveDir = strRootDir + strYearMonth;

	if (_access(CStdStr::ws2s(strSaveDir).c_str(), 0) != 0 && !CreateDirectory(CString(strSaveDir.c_str()), NULL))
	{
		return -1;
	}

	_tstring strSavePath = CStdStr::AddSlashIfNeeded(strSaveDir) + CStdStr::GetNameOfFile(sStr);
	g_Mutex.Lock();
	if (CStdFile::IfAccessFile(strSavePath))
	{
		int nNum = 0;
		do
		{
			++nNum;
			strSavePath = CStdStr::AddSlashIfNeeded(strSaveDir) + CStdStr::GetNameOfFile(sStr, false) + 
				CStdTpl::ConvertToString(nNum) + CStdStr::GetSuffixOfFile(sStr);

		} while (CStdFile::IfAccessFile(strSavePath));
	}

	if (strSavePath != sStr)
	{
		rename(CStdStr::ws2s(sStr).c_str(), CStdStr::ws2s(strSavePath).c_str());
	}

	g_mLogs.insert(std::make_pair(sStr, strSavePath));
	g_Mutex.Unlock();

	return 0;
}

int CClassifyFiles::RestoreFile(const _tstring& sStr, const _tstring& sSavePath, const size_t& nProcessIndex)
{
	_tstring strOldPath = g_mLogs[sStr];
	_tstring strOldDir = CStdStr::GetDirOfFile(strOldPath);
	if (_access(CStdStr::ws2s(strOldDir).c_str(), 0) != 0 && !CStdDir::CreateDir(strOldDir))
	{
		return -1;
	}

	if (strOldPath != sStr)
	{
		//路径不同则移动文件
		rename(CStdStr::ws2s(sStr).c_str(), CStdStr::ws2s(strOldPath).c_str());
	}

	return 0;
}

void CClassifyFiles::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strDirFiles, strSuffixs;
	GetDlgItemText(IDC_EDIT_DIR, strDirFiles);

	if (!strDirFiles.GetLength())
	{
		strDirFiles = CMfcStrFile::BrowseDir();
		if (strDirFiles.GetLength())
		{
			SetDlgItemText(IDC_EDIT_DIR, strDirFiles);
		}
		else
		{
			SetFocus();
			return;
		}
	}

	if (m_cfg.cf_bRemLastDir)
	{
		m_cfg.cf_strLastDir = CMfcStrFile::CString2string(strDirFiles);
	}

	bool bPics = ((CButton*)GetDlgItem(IDC_RADIO_PICS))->GetCheck() == TRUE;
	bool bFiles = ((CButton*)GetDlgItem(IDC_RADIO_FILES))->GetCheck() == TRUE;
	bool bRec = ((CButton*)GetDlgItem(IDC_RADIO_REC))->GetCheck() == TRUE;

	if (((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->GetCheck() == TRUE)
	{
		m_cfg.cf_nMethod = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->GetCheck() == TRUE)
	{
		m_cfg.cf_nMethod = 1;
	}

	std::vector<_tstring> vSuffixs;
	if (bPics)
	{
		if (ShowWarnnings() == FALSE)
		{
			SetFocus();
			return;
		}
		m_cfg.cf_nSelIndex = 0;
		for (int i = 0; i < 4; ++i)
		{
			m_cfg.cf_bPics[i] = false;
		}

		//获取需要处理的类型
		if (((CButton*)GetDlgItem(IDC_CHECK_BMP))->GetCheck())
		{
			vSuffixs.push_back(_T("*.bmp"));
			m_cfg.cf_bPics[0] = true;
		}
		else
		{
			m_cfg.cf_bPics[0] = false;
		}
		if (((CButton*)GetDlgItem(IDC_CHECK_JPG))->GetCheck())
		{
			vSuffixs.push_back(_T("*.jpg"));
			vSuffixs.push_back(_T("*.jpeg"));
			m_cfg.cf_bPics[1] = true;
		}
		else
		{
			m_cfg.cf_bPics[1] = false;
		}
		if (((CButton*)GetDlgItem(IDC_CHECK_PNG))->GetCheck())
		{
			vSuffixs.push_back(_T("*.png"));
			m_cfg.cf_bPics[2] = true;
		}
		else
		{
			m_cfg.cf_bPics[2] = false;
		}
		if (((CButton*)GetDlgItem(IDC_CHECK_GIF))->GetCheck())
		{
			vSuffixs.push_back(_T("*.gif"));
			m_cfg.cf_bPics[3] = true;
		}
		else
		{
			m_cfg.cf_bPics[3] = false;
		}
	}
	else if (bFiles)
	{
		if (ShowWarnnings() == FALSE)
		{
			SetFocus();
			return;
		}
		m_cfg.cf_nSelIndex = 1;
		CString strFileSuffixs;
		GetDlgItemText(IDC_EDIT_CUSTOM, strFileSuffixs);
		if (!strFileSuffixs.GetLength())
		{
			return;
		}
		_tstring strSuffixs = CMfcStrFile::CString2string(strFileSuffixs);
		m_cfg.cf_strCustom = strSuffixs;
		vSuffixs = CStdStr::Split(strSuffixs, _T("|"));
	}

	//将输入分为文件和文件夹
	CStringArray arrDirFiles;
	std::vector<_tstring> vFilesRead, vDirsRead;
	CMfcStrFile::Split(strDirFiles, arrDirFiles, '|');
	int nDirFileNum = (int)arrDirFiles.GetCount();
	for (int i = 0; i < nDirFileNum; ++i)
	{
		_tstring strCurItem = CMfcStrFile::CString2string(arrDirFiles[i]);
		if(_access(CStdStr::ws2s(strCurItem).c_str(), 0) == 0)
		{
			if (CStdFile::IfAccessFile(strCurItem))
			{
				vFilesRead.push_back(strCurItem);
			}
			else
			{
				vDirsRead.push_back(strCurItem);
			}
		}
	}

	//测试时间
	clock_t start, finish;
	double duration;
	start = clock();

	m_pDlgManager->m_pMainFrame->InitTaskBarProgress();
	CProgressInterface* ppi = m_pDlgManager->m_pMainFrame->m_pTb;
	m_pDlgManager->StartMainProgress();

	//如果是文件，则处理文件
	const int nFileNum = (int)vFilesRead.size();
	std::vector<_tstring> vPreFiles;

	if (bPics || bFiles && !CStdTpl::VectorContains(vSuffixs, CStdStr::s2ws("*.*")))
	{
		//此时需要按照后缀过滤文件
		for (int m = 0; m < nFileNum; ++m)
		{
			_tstring strCurFile = vFilesRead[m];
			_tstring strCurFileSuffix = _T('*') + CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(strCurFile));
			if (CStdTpl::VectorContains(vSuffixs, strCurFileSuffix))
			{
				vPreFiles.push_back(strCurFile);
			}
		}
	}
	else
	{
		vPreFiles = vFilesRead;
	}

	const int nPreFileNum = (int)vPreFiles.size();
	if (nPreFileNum > 0)
	{
		//根据输入路径得到输出路径
		_tstring strCommonDir = CStdStr::GetCommonDir(vPreFiles);
		g_strDir = strCommonDir.c_str();

		//开始显示进度
		ppi->Start();
		
		if (m_pDlgManager)
		{
			m_pDlgManager->MiddleMainProgress();
			m_pDlgManager->SetBarInfo(PROCESSING_INFO);
			m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
		}

		/************主程序****************/
		if (bPics)
		{
			if (m_cfg.cf_nMethod == 0)
			{
				ProcessVector(vPreFiles, vPreFiles, MovePicsByTime, ppi, THREAD_NUMS);
			}
			else if (m_cfg.cf_nMethod == 1)
			{
				ProcessVector(vPreFiles, vPreFiles, MovePicsByResolution, ppi, THREAD_NUMS);
			}
		}
		else if (bFiles)
		{
			ProcessVector(vPreFiles, vPreFiles, MoveFilesByTime, ppi, THREAD_NUMS);
		}
		/**********************************/

		if (m_pDlgManager)
		{
			m_pDlgManager->FlashWindow(TRUE);
			m_pDlgManager->SetBarInfo(_T("处理完成！"));
			m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		}

		//保存log，当前时间以及耗时
		_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(strCommonDir.c_str()) + g_strSubDirName + _T("log.log");
		std::vector<_tstring> vLogContent;
		for (std::map<_tstring, _tstring>::iterator it = g_mLogs.begin(); it != g_mLogs.end(); ++it)
		{
			vLogContent.push_back(it->first + _T("->") + it->second + _T('\n'));
		}
		CStdFile::SaveTXTFile(strMoveLogFile, vLogContent);
	}

	int nDirNum = (int)vDirsRead.size();
	//如果是图片文件
	if (bPics)
	{
		for (int m = 0; m < nDirNum; ++m)
		{
			//处理多个文件夹
			g_strDir = vDirsRead[m].c_str();
			g_mLogs.clear();
			_tstring sDir = CMfcStrFile::CString2string(g_strDir);
			if (_access(CStdStr::ws2s(sDir).c_str(), 0) != 0)
			{
				//文件夹不存在则继续
				continue;
			}

 			std::vector<_tstring> vFiles;
			getFiles(sDir, vFiles, vSuffixs, true);

			//开始显示进度
			ppi->Start();
			
			if (m_pDlgManager)
			{
				m_pDlgManager->MiddleMainProgress(m * 100 / nDirFileNum + 1);
				m_pDlgManager->SetBarInfo(PROCESSING_INFO);
				m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
			}

			if (m_cfg.cf_nMethod == 0)
			{
				ProcessVector(vFiles, vFiles, MovePicsByTime, ppi, THREAD_NUMS);
			}
			else if (m_cfg.cf_nMethod == 1)
			{
				ProcessVector(vFiles, vFiles, MovePicsByResolution, ppi, THREAD_NUMS);
			}

			if (m_pDlgManager)
			{
				m_pDlgManager->FlashWindow(TRUE);
				m_pDlgManager->SetBarInfo(_T("处理完成！"));
				m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
				ppi->End();
			}

			//保存log，当前时间以及耗时
			_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(sDir.c_str()) + g_strSubDirName + _T("log.log");
			std::vector<_tstring> vLogContent;
			for (std::map<_tstring, _tstring>::iterator it = g_mLogs.begin(); it != g_mLogs.end(); ++it)
			{
				vLogContent.push_back(it->first + _T("->") + it->second + _T('\n'));
			}
			CStdFile::SaveTXTFile(strMoveLogFile, vLogContent);
		}
	}
	//如果是自定义文件
	else if (bFiles)
	{
		for (int m = 0; m < nDirNum; ++m)
		{
			//处理多个文件夹
			g_strDir = vDirsRead[m].c_str();
			g_mLogs.clear();
			_tstring sDir = CMfcStrFile::CString2string(g_strDir);
			if (_access(CStdStr::ws2s(sDir).c_str(), 0) != 0)
			{
				//文件夹不存在则继续
				continue;
			}

			std::vector<_tstring> vFiles;
			for (int i = 0; i < vSuffixs.size(); ++i)
			{
				std::vector<_tstring> vTmpFiles;
				getFiles(sDir, vTmpFiles, CStdStr::ws2s(vSuffixs[i]).c_str(), true);
				vFiles.insert(vFiles.end(), vTmpFiles.begin(), vTmpFiles.end());
			}

			//开始显示进度
			ppi->Start();
			
			if (m_pDlgManager)
			{
				m_pDlgManager->MiddleMainProgress(m * 100 / nDirFileNum + 1);
				m_pDlgManager->SetBarInfo(PROCESSING_INFO);
				m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
			}

			ProcessVector(vFiles, vFiles, MoveFilesByTime, ppi, THREAD_NUMS);
			if (m_pDlgManager)
			{
				m_pDlgManager->FlashWindow(TRUE);
				m_pDlgManager->SetBarInfo(_T("处理完成！"));
				m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
				ppi->End();
			}

			//保存log，当前时间以及耗时
			_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(sDir) + g_strSubDirName + _T("log.log");
			std::vector<_tstring> vLogContent;
			for (std::map<_tstring, _tstring>::iterator it = g_mLogs.begin(); it != g_mLogs.end(); ++it)
			{
				vLogContent.push_back(it->first + _T("->") + it->second + _T('\n'));
			}
			CStdFile::SaveTXTFile(strMoveLogFile, vLogContent);
		}
	}
	//如果是恢复
	else if (((CButton*)GetDlgItem(IDC_RADIO_REC))->GetCheck())
	{
		//此时下次默认选择PICS
		m_cfg.cf_nSelIndex = 0;
		for (int m = 0; m < nDirNum; ++m)
		{
			//处理多个文件夹
			g_strDir = vDirsRead[m].c_str();
			g_mLogs.clear();
			_tstring sDir = CMfcStrFile::CString2string(g_strDir);
			if (_access(CStdStr::ws2s(sDir).c_str(), 0) != 0)
			{
				//文件夹不存在则继续
				continue;
			}

			_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(sDir) + g_strSubDirName + _T("log.log");
			if (!CStdFile::IfAccessFile(strMoveLogFile))
			{
				continue;
			}
			std::vector<_tstring> vLogContent;
			std::vector<_tstring> vFiles;	//本参数是为了传递给回调函数
			CStdFile::ParseTXTFile(strMoveLogFile, vLogContent);
			for (int i = 0; i < vLogContent.size(); ++i)
			{
				std::vector<_tstring> vLines = CStdStr::Split(vLogContent[i], _T("->"));
				if (vLines.size() >= 2)
				{
					g_mLogs.insert(std::make_pair(vLines[1].c_str(), vLines[0].c_str()));
					vFiles.push_back(vLines[1]);
				}
			}

			//开始显示进度
			ppi->Start();
			
			if (m_pDlgManager)
			{
				m_pDlgManager->MiddleMainProgress(m * 100 / nDirFileNum + 1);
				m_pDlgManager->SetBarInfo(PROCESSING_INFO);
				m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
			}

			ProcessVector(vFiles, vFiles, RestoreFile, ppi, THREAD_NUMS);

			if (m_pDlgManager)
			{
				m_pDlgManager->FlashWindow(TRUE);
				m_pDlgManager->SetBarInfo(_T("处理完成！"));
				m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
				ppi->End();
			}

			//清理工作
			DeleteFile(CString(strMoveLogFile.c_str()));
		}
	}

	if (m_pDlgManager)
	{
		m_pDlgManager->EndMainProgress();
	}

	finish = clock();
	duration = double(finish - start) / CLOCKS_PER_SEC;
	int nMinutes = int(duration / 60);
	int nSeconds = (int)duration % 60;
	CString strTips;
	strTips.Format(_T("%d分%d秒"), nMinutes, nSeconds);

	CString strTime, strDate; //获取系统时间 　　
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y年%m月%d日 %X");
	strDate = tm.Format("%Y年%m月%d日");

	CString strSaveLine = strTips + "\t\t" + strTime + "\t\t" + g_strDir + '\n';

	TCHAR exeFullPath[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	strPath = (CString)exeFullPath;
	int position = strPath.ReverseFind('\\');
	strPath = strPath.Left(position);

	CStdioFile sFile(strPath + "\\" + strDate + ".log", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	sFile.SeekToEnd();
	setlocale(LC_CTYPE, "chs");//设定f
	sFile.WriteString(strSaveLine);
	setlocale(LC_ALL, "C"); //还原区域设定
	sFile.Close();

	if (m_pDlgManager)
	{
		//写入ini文件
		m_cfg.nImgToolIndex = 0;
		m_pDlgManager->SetIniConfig(m_cfg);
	}

	MessageBox(_T("耗时:") + strTips + '\n' + _T("文件整理到目录中啦！"), _T("終わり！"));
	SetFocus();

	//CDialog::OnOK();
}


void CClassifyFiles::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DIR, _T(""));
	SetDlgItemText(IDC_EDIT_CUSTOM, _T("*.*"));
	
	((CButton*)GetDlgItem(IDC_RADIO_PICS))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_FILES))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_REC))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->SetCheck(FALSE);
	OnBnClickedRadioPics();

	//CDialog::OnCancel();
}


void CClassifyFiles::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 定义一个缓冲区来存放读取的文件名信息
	wchar_t szDirPath[MAX_PATH + 1] = { 0 };
	// 通过设置iFiles参数为0xFFFFFFFF,可以取得当前拖动的文件数量，
	// 当设置为0xFFFFFFFF,函数间忽略后面两个参数。
	UINT nNum = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	// 通过循环依次取得拖动文件的File Name信息，并把它添加到ListBox中
	CString strDirFiles;
	for (UINT i = 0; i < nNum; ++i)
	{
		DragQueryFile(hDropInfo, i, szDirPath, MAX_PATH);
		if (_access(CStdStr::ws2s(szDirPath).c_str(), 0) == 0)
		{
			strDirFiles += szDirPath + CString('|');
		}
	}

	strDirFiles.Trim('|');
	SetDlgItemText(IDC_EDIT_DIR, strDirFiles);

	if (m_pDlgManager)
	{
		if(strDirFiles.GetLength())
		{
			if (nNum == 1)
			{
				_tstring strDirName = CStdStr::GetNameOfFile(CMfcStrFile::CString2string(strDirFiles));
				m_pDlgManager->SetBarInfo(strDirName + _T(" 加载成功！"));
			}
			else
			{
				m_pDlgManager->SetBarInfo(_T("多个文件(夹)加载成功！"));
			}
		}
		else
		{
			m_pDlgManager->SetBarInfo(_T("什么也没有发生！"));
		}
	}

	// 结束此次拖拽操作，并释放分配的资源
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

BOOL CClassifyFiles::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnEraseBkgnd(pDC);
	HBITMAP hBitmap;
	HDC m_hBkDC;
	hBitmap = ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP1));
	BITMAP bitmap;
	GetObject(hBitmap, sizeof (BITMAP), &bitmap) ;//获取位图尺寸
	CRect rect;
	GetClientRect(&rect);
	m_hBkDC = ::CreateCompatibleDC(pDC->m_hDC);
	if(hBitmap && m_hBkDC)
	{
		::SelectObject(m_hBkDC,hBitmap);
		::StretchBlt(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), m_hBkDC,0,0, bitmap.bmWidth, bitmap.bmHeight,SRCCOPY);
		::DeleteObject(hBitmap);
		::DeleteDC(m_hBkDC);
	}

	return TRUE;
}


HBRUSH CClassifyFiles::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);//设置背景透明
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CClassifyFiles::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	const int nCtrlNum = DIALOG_ITEM_COUNT;
	for (int i = 0; i < nCtrlNum; i++)
	{
		//获取控件句柄
		CWnd* pWnd = GetDlgItem(1000 + i); 
		if (pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建 
		{
			CRect rect; //获取控件变化前大小 
			pWnd->GetWindowRect(&rect);
			//将控件大小转换为在对话框中的区域坐标 
			ScreenToClient(&rect);
			//cx/m_rect.Width()为对话框在横向的变化比例 
			rect.left = rect.left*cx / m_rect.Width();
			rect.right = rect.right*cx / m_rect.Width();
			rect.top = rect.top*cy / m_rect.Height();
			rect.bottom = rect.bottom*cy / m_rect.Height();
			pWnd->MoveWindow(rect);
		}
	}
}
