// ClassifyFiles.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClassifyFiles.h"
#include "afxdialogex.h"
#include "src\Configure.h"
#include "DlgManager.h"
#include "MainFrm.h"

#define THREAD_NUMS		1

// ClassifyFiles �Ի���

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

// ClassifyFiles ��Ϣ�������
BOOL CClassifyFiles::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (m_pDlgManager)
	{
		//��ȡ����
		m_pDlgManager->GetIniConfig(m_cfg);
	}
	if (m_cfg.cf_bRemLastDir)
	{
		SetDlgItemText(IDC_EDIT_DIR, m_cfg.cf_strLastDir.c_str());
	}

	//���������ļ����ô���״̬
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CClassifyFiles::OnBnClickedButtonDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->EnableWindow(TRUE);
}


void CClassifyFiles::OnBnClickedRadioFiles()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->EnableWindow(FALSE);
}


void CClassifyFiles::OnBnClickedRadioRec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_PHOTO_TIME))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_RESOLUTION))->EnableWindow(FALSE);
}

BOOL CClassifyFiles::ShowWarnnings()
{
	return MessageBox(_T("1.���������һ����Σ���ԣ������С�Ĳ�����\n2.��ѡĿ¼ָ�����͵��ļ���ȫ���������У���ȷ����ѡĿ¼��ȷ��\n3.��һ����������ʹ�ñ������Դ��Ļָ����ܣ�\n\n�Ƿ������"), 
		_T("��ʾ"), MB_YESNO) == IDYES;

}

/******************************ȫ�ֱ���**********************************/
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
		//·����ͬ���ƶ��ļ�
		rename(CStdStr::ws2s(sStr).c_str(), CStdStr::ws2s(strOldPath).c_str());
	}

	return 0;
}

void CClassifyFiles::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

		//��ȡ��Ҫ���������
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

	//�������Ϊ�ļ����ļ���
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

	//����ʱ��
	clock_t start, finish;
	double duration;
	start = clock();

	m_pDlgManager->m_pMainFrame->InitTaskBarProgress();
	CProgressInterface* ppi = m_pDlgManager->m_pMainFrame->m_pTb;
	m_pDlgManager->StartMainProgress();

	//������ļ��������ļ�
	const int nFileNum = (int)vFilesRead.size();
	std::vector<_tstring> vPreFiles;

	if (bPics || bFiles && !CStdTpl::VectorContains(vSuffixs, CStdStr::s2ws("*.*")))
	{
		//��ʱ��Ҫ���պ�׺�����ļ�
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
		//��������·���õ����·��
		_tstring strCommonDir = CStdStr::GetCommonDir(vPreFiles);
		g_strDir = strCommonDir.c_str();

		//��ʼ��ʾ����
		ppi->Start();
		
		if (m_pDlgManager)
		{
			m_pDlgManager->MiddleMainProgress();
			m_pDlgManager->SetBarInfo(PROCESSING_INFO);
			m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
		}

		/************������****************/
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
			m_pDlgManager->SetBarInfo(_T("������ɣ�"));
			m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		}

		//����log����ǰʱ���Լ���ʱ
		_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(strCommonDir.c_str()) + g_strSubDirName + _T("log.log");
		std::vector<_tstring> vLogContent;
		for (std::map<_tstring, _tstring>::iterator it = g_mLogs.begin(); it != g_mLogs.end(); ++it)
		{
			vLogContent.push_back(it->first + _T("->") + it->second + _T('\n'));
		}
		CStdFile::SaveTXTFile(strMoveLogFile, vLogContent);
	}

	int nDirNum = (int)vDirsRead.size();
	//�����ͼƬ�ļ�
	if (bPics)
	{
		for (int m = 0; m < nDirNum; ++m)
		{
			//�������ļ���
			g_strDir = vDirsRead[m].c_str();
			g_mLogs.clear();
			_tstring sDir = CMfcStrFile::CString2string(g_strDir);
			if (_access(CStdStr::ws2s(sDir).c_str(), 0) != 0)
			{
				//�ļ��в����������
				continue;
			}

 			std::vector<_tstring> vFiles;
			getFiles(sDir, vFiles, vSuffixs, true);

			//��ʼ��ʾ����
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
				m_pDlgManager->SetBarInfo(_T("������ɣ�"));
				m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
				ppi->End();
			}

			//����log����ǰʱ���Լ���ʱ
			_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(sDir.c_str()) + g_strSubDirName + _T("log.log");
			std::vector<_tstring> vLogContent;
			for (std::map<_tstring, _tstring>::iterator it = g_mLogs.begin(); it != g_mLogs.end(); ++it)
			{
				vLogContent.push_back(it->first + _T("->") + it->second + _T('\n'));
			}
			CStdFile::SaveTXTFile(strMoveLogFile, vLogContent);
		}
	}
	//������Զ����ļ�
	else if (bFiles)
	{
		for (int m = 0; m < nDirNum; ++m)
		{
			//�������ļ���
			g_strDir = vDirsRead[m].c_str();
			g_mLogs.clear();
			_tstring sDir = CMfcStrFile::CString2string(g_strDir);
			if (_access(CStdStr::ws2s(sDir).c_str(), 0) != 0)
			{
				//�ļ��в����������
				continue;
			}

			std::vector<_tstring> vFiles;
			for (int i = 0; i < vSuffixs.size(); ++i)
			{
				std::vector<_tstring> vTmpFiles;
				getFiles(sDir, vTmpFiles, CStdStr::ws2s(vSuffixs[i]).c_str(), true);
				vFiles.insert(vFiles.end(), vTmpFiles.begin(), vTmpFiles.end());
			}

			//��ʼ��ʾ����
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
				m_pDlgManager->SetBarInfo(_T("������ɣ�"));
				m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
				ppi->End();
			}

			//����log����ǰʱ���Լ���ʱ
			_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(sDir) + g_strSubDirName + _T("log.log");
			std::vector<_tstring> vLogContent;
			for (std::map<_tstring, _tstring>::iterator it = g_mLogs.begin(); it != g_mLogs.end(); ++it)
			{
				vLogContent.push_back(it->first + _T("->") + it->second + _T('\n'));
			}
			CStdFile::SaveTXTFile(strMoveLogFile, vLogContent);
		}
	}
	//����ǻָ�
	else if (((CButton*)GetDlgItem(IDC_RADIO_REC))->GetCheck())
	{
		//��ʱ�´�Ĭ��ѡ��PICS
		m_cfg.cf_nSelIndex = 0;
		for (int m = 0; m < nDirNum; ++m)
		{
			//�������ļ���
			g_strDir = vDirsRead[m].c_str();
			g_mLogs.clear();
			_tstring sDir = CMfcStrFile::CString2string(g_strDir);
			if (_access(CStdStr::ws2s(sDir).c_str(), 0) != 0)
			{
				//�ļ��в����������
				continue;
			}

			_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(sDir) + g_strSubDirName + _T("log.log");
			if (!CStdFile::IfAccessFile(strMoveLogFile))
			{
				continue;
			}
			std::vector<_tstring> vLogContent;
			std::vector<_tstring> vFiles;	//��������Ϊ�˴��ݸ��ص�����
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

			//��ʼ��ʾ����
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
				m_pDlgManager->SetBarInfo(_T("������ɣ�"));
				m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
				ppi->End();
			}

			//������
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
	strTips.Format(_T("%d��%d��"), nMinutes, nSeconds);

	CString strTime, strDate; //��ȡϵͳʱ�� ����
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y��%m��%d�� %X");
	strDate = tm.Format("%Y��%m��%d��");

	CString strSaveLine = strTips + "\t\t" + strTime + "\t\t" + g_strDir + '\n';

	TCHAR exeFullPath[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	strPath = (CString)exeFullPath;
	int position = strPath.ReverseFind('\\');
	strPath = strPath.Left(position);

	CStdioFile sFile(strPath + "\\" + strDate + ".log", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	sFile.SeekToEnd();
	setlocale(LC_CTYPE, "chs");//�趨f
	sFile.WriteString(strSaveLine);
	setlocale(LC_ALL, "C"); //��ԭ�����趨
	sFile.Close();

	if (m_pDlgManager)
	{
		//д��ini�ļ�
		m_cfg.nImgToolIndex = 0;
		m_pDlgManager->SetIniConfig(m_cfg);
	}

	MessageBox(_T("��ʱ:") + strTips + '\n' + _T("�ļ�����Ŀ¼������"), _T("�K��꣡"));
	SetFocus();

	//CDialog::OnOK();
}


void CClassifyFiles::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ����һ������������Ŷ�ȡ���ļ�����Ϣ
	wchar_t szDirPath[MAX_PATH + 1] = { 0 };
	// ͨ������iFiles����Ϊ0xFFFFFFFF,����ȡ�õ�ǰ�϶����ļ�������
	// ������Ϊ0xFFFFFFFF,��������Ժ�������������
	UINT nNum = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	// ͨ��ѭ������ȡ���϶��ļ���File Name��Ϣ����������ӵ�ListBox��
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
				m_pDlgManager->SetBarInfo(strDirName + _T(" ���سɹ���"));
			}
			else
			{
				m_pDlgManager->SetBarInfo(_T("����ļ�(��)���سɹ���"));
			}
		}
		else
		{
			m_pDlgManager->SetBarInfo(_T("ʲôҲû�з�����"));
		}
	}

	// �����˴���ק���������ͷŷ������Դ
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

BOOL CClassifyFiles::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnEraseBkgnd(pDC);
	HBITMAP hBitmap;
	HDC m_hBkDC;
	hBitmap = ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP1));
	BITMAP bitmap;
	GetObject(hBitmap, sizeof (BITMAP), &bitmap) ;//��ȡλͼ�ߴ�
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
		pDC->SetBkMode(TRANSPARENT);//���ñ���͸��
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CClassifyFiles::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	const int nCtrlNum = DIALOG_ITEM_COUNT;
	for (int i = 0; i < nCtrlNum; i++)
	{
		//��ȡ�ؼ����
		CWnd* pWnd = GetDlgItem(1000 + i); 
		if (pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ���� 
		{
			CRect rect; //��ȡ�ؼ��仯ǰ��С 
			pWnd->GetWindowRect(&rect);
			//���ؼ���Сת��Ϊ�ڶԻ����е��������� 
			ScreenToClient(&rect);
			//cx/m_rect.Width()Ϊ�Ի����ں���ı仯���� 
			rect.left = rect.left*cx / m_rect.Width();
			rect.right = rect.right*cx / m_rect.Width();
			rect.top = rect.top*cy / m_rect.Height();
			rect.bottom = rect.bottom*cy / m_rect.Height();
			pWnd->MoveWindow(rect);
		}
	}
}
