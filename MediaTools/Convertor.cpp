// Convertor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Convertor.h"
#include "afxdialogex.h"
#include "DlgManager.h"
#include "MainFrm.h"


// CConvertor �Ի���

IMPLEMENT_DYNAMIC(CConvertor, CDialogEx)

CConvertor::CConvertor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConvertor::IDD, pParent)
{
	m_pDlgManager = nullptr;
}

CConvertor::~CConvertor()
{
}

void CConvertor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConvertor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEDIR, &CConvertor::OnBnClickedButtonBrowsedir)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK_CON, &CConvertor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL_CON, &CConvertor::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_JPG, &CConvertor::OnBnClickedCheckJpg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/******************************qȫ�ֱ���**********************************/
static bool g_bKeepTime = true, g_bSetPhoto = true, g_bGrayScale = false;
static _tstring g_strDir;
static std::vector<_tstring> g_vLog;
/************************************************************************/

// CConvertor ��Ϣ�������
BOOL CConvertor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (m_pDlgManager)
	{
		//��ȡ����
		m_pDlgManager->GetIniConfig(m_cfg);
	}

	//���������ļ����ô���״̬
	//�Ƿ񱣳ֱ���
	if (m_cfg.con_bRemLastDir)
	{
		SetDlgItemText(IDC_EDIT_DIR, CString(m_cfg.con_strLastDir.c_str()));
	}
	SetDlgItemText(IDC_EDIT_SUFFIXS, CString(m_cfg.con_strSuffixs.c_str()));

	((CButton*)GetDlgItem(IDC_CHECK_BMP))->SetCheck(m_cfg.con_bPics[0]);
	((CButton*)GetDlgItem(IDC_CHECK_JPG))->SetCheck(m_cfg.con_bPics[1]);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->SetCheck(m_cfg.con_bPics[2]);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->SetCheck(m_cfg.con_bPics[3]);
	((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->SetCheck(m_cfg.con_bSets[2]);

	if (m_cfg.con_nSelIndex == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_NEWDIR))->SetCheck(TRUE);
	}
	else if(m_cfg.con_nSelIndex == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_SAMEDIR))->SetCheck(TRUE);
	}

	((CButton*)GetDlgItem(IDC_CHECK_KEEPMODIFYTIME))->SetCheck(m_cfg.con_bSets[0]);
	((CButton*)GetDlgItem(IDC_CHECK_SETPHOTOTIME))->SetCheck(m_cfg.con_bSets[1]);

	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY);

	if (pCombox != nullptr)
	{
		pCombox->AddString(_T("60"));
		pCombox->AddString(_T("80"));
		pCombox->AddString(_T("90"));
		pCombox->AddString(_T("99"));
		const int& nJpgQuality = m_cfg.con_nJpgQuality;
		if (nJpgQuality >= 0 && nJpgQuality <= 100)
		{
			_tstring sJpgQuality = CStdTpl::ConvertToString(nJpgQuality);
			int nJpgQualityIndex = pCombox->FindString(0, CString(sJpgQuality.c_str()));
			if (nJpgQualityIndex != -1)
			{
				pCombox->SetCurSel(nJpgQualityIndex);
			}
			else
			{
				pCombox->AddString(CString(sJpgQuality.c_str()));
				nJpgQualityIndex = pCombox->FindString(0, CString(sJpgQuality.c_str()));
				pCombox->SetCurSel(nJpgQualityIndex);
			}
		}
		else
		{
			int nJpgQualityIndex = pCombox->FindString(0, _T("99"));
			pCombox->SetCurSel(nJpgQualityIndex);
		}

		if (m_cfg.con_bPics[1])
		{
			pCombox->EnableWindow(TRUE);
		}
		else
		{
			pCombox->EnableWindow(FALSE);
		}
	}

	GetClientRect(m_rect);
	m_cfg.nCateGoryIndex = IMGTOOL_CATE_INDEX;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CConvertor::OnBnClickedButtonBrowsedir()
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

int CConvertor::TimesFilesSaves(const std::vector<_tstring>& vFiles, std::vector<_tstring>& vTimesFiles, std::vector<_tstring>& vTimesSaves)
{
	//�洢����Ŀ¼
	bool bNewDir = m_cfg.con_nSelIndex == 0;
	std::vector<_tstring> vSuffixs;
	//Ŀ�����ͣ�ע�ⶼ��Сд��������*
	if (((CButton*)GetDlgItem(IDC_CHECK_BMP))->GetCheck())
	{
		vSuffixs.push_back(_T("bmp"));
		m_cfg.con_bPics[0] = true;
	}
	else
	{
		m_cfg.con_bPics[0] = false;
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_JPG))->GetCheck())
	{
		vSuffixs.push_back(_T("jpg"));
		m_cfg.con_bPics[1] = true;
	}
	else
	{
		m_cfg.con_bPics[1] = false;
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_PNG))->GetCheck())
	{
		vSuffixs.push_back(_T("png"));
		m_cfg.con_bPics[2] = true;
	}
	else
	{
		m_cfg.con_bPics[2] = false;
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_GIF))->GetCheck())
	{
		vSuffixs.push_back(_T("gif"));
		m_cfg.con_bPics[3] = true;
	}
	else
	{
		m_cfg.con_bPics[3] = false;
	}

	//ÿ�ָ�ʽ�ֱ���
	if (bNewDir && g_strDir.length() > 0)
	{
		for (int i = 0; i < vSuffixs.size(); ++i)
		{
			const _tstring& strCurSuffix = vSuffixs[i];
			const _tstring& strDirName = CStdStr::ToUpperLower(strCurSuffix, false);
			for (int j = 0; j < vFiles.size(); ++j)
			{
				const _tstring& strCurFile = vFiles[j];
				//���ܺ�׺�Ƿ���ͬ������Ҫ�������Ҹ������ļ�
				_tstring strSubPath = strCurFile.substr(g_strDir.length());
				_tstring strNewTmpPath = CStdStr::AddSlashIfNeeded(g_strDir) + strDirName + strSubPath;
				_tstring strNewPath = CStdStr::ReplaceSuffix(strNewTmpPath, _T('.') + strCurSuffix);
				_tstring strDstDir = CStdStr::GetDirOfFile(strNewPath);
				if (_access(CStdStr::ws2s(strDstDir).c_str(), 0) != 0)
				{
					CStdDir::CreateDir(strDstDir);
				}

				int nNum = 0;
				//������ǰԤ�������ļ�
				if (CStdTpl::VectorContains(vTimesSaves, strNewPath) || CStdFile::IfAccessFile(strNewPath.c_str()))
				{
					do
					{
						++nNum;
						strNewPath = CStdStr::AddSlashIfNeeded(strDstDir) + CStdStr::GetNameOfFile(strNewTmpPath, false) + _T("_") +
							CStdTpl::ConvertToString(nNum) + CStdStr::GetSuffixOfFile(strNewPath);

					} while (CStdTpl::VectorContains(vTimesSaves, strNewPath) || CStdFile::IfAccessFile(strNewPath.c_str()));
				}
				
				//������
				vTimesFiles.push_back(strCurFile);
				vTimesSaves.push_back(strNewPath);
			}
		}
	}
	else
	{
		for (int i = 0; i < vSuffixs.size(); ++i)
		{
			const _tstring& strCurSuffix = _T('.') + vSuffixs[i];
			for (int j = 0; j < vFiles.size(); ++j)
			{
				const _tstring& strCurFile = vFiles[j];
				const _tstring& strOriSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(strCurFile));

				_tstring strNewTmpPath = CStdStr::ReplaceSuffix(strCurFile, strCurSuffix);
				_tstring strNewPath = CStdStr::ReplaceSuffix(strCurFile, strCurSuffix);
				_tstring strDstDir = CStdStr::GetDirOfFile(strNewTmpPath);

				//������
				int nNum = 0;
				//���ﱣ֤���Ḵд�Ѿ����ڵ��ļ���������ǰԤ�������ļ�
				if (CStdTpl::VectorContains(vTimesSaves, strNewPath) || CStdFile::IfAccessFile(strNewPath.c_str()))
				{
					do
					{
						++nNum;
						strNewPath = CStdStr::AddSlashIfNeeded(strDstDir) + CStdStr::GetNameOfFile(strNewTmpPath, false) + _T("_") +
							CStdTpl::ConvertToString(nNum) + CStdStr::GetSuffixOfFile(strNewPath);

					} while (CStdTpl::VectorContains(vTimesSaves, strNewPath) || CStdFile::IfAccessFile(strNewPath.c_str()));
				}

				vTimesFiles.push_back(strCurFile);
				vTimesSaves.push_back(strNewPath);
			}
		}
	}

	return int(vTimesFiles.size());
}

int CConvertor::ConvertPics(const _tstring& sInputPath, const _tstring& sSavePath, const size_t& nProcessIndex)
{
	//��ȡexif��Ϣ
	_tstring srcExtension = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sInputPath));
	_tstring dstExtension = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sSavePath));

	if (CxImageUser::pic2pic(sInputPath, sSavePath, -1, -1, g_bGrayScale))
	{
		g_vLog[nProcessIndex] = (sInputPath + _T("->") + sSavePath + _T('\n'));
	}
	else
	{
		//����ʧ��
		g_vLog[nProcessIndex] = (_T("Convert") +  sInputPath + _T(" Failed!") + _T('\n'));

		return -1;
	}

	if (!CloneExif(sInputPath.c_str(), sSavePath))
	{
		if (g_bSetPhoto)
		{
			SetPhotoTime(sInputPath, sSavePath);
		}
	}

	//����ʱ��
	if (g_bKeepTime)
	{
		CloneFileTime(CString(sInputPath.c_str()), CString(sSavePath.c_str()));
	}

	return 0;
}

void CConvertor::OnBnClickedOk()
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

	GetDlgItemText(IDC_EDIT_SUFFIXS, strSuffixs);
	m_cfg.con_strLastDir = strDirFiles;
	m_cfg.con_strSuffixs = strSuffixs;
	g_bKeepTime = ((CButton*)GetDlgItem(IDC_CHECK_KEEPMODIFYTIME))->GetCheck() == TRUE;
	g_bSetPhoto = ((CButton*)GetDlgItem(IDC_CHECK_SETPHOTOTIME))->GetCheck() == TRUE;
	g_bGrayScale = ((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->GetCheck() == TRUE;
	bool bNewDir = ((CButton*)GetDlgItem(IDC_RADIO_NEWDIR))->GetCheck() == TRUE;
	m_cfg.con_bSets[0] = g_bKeepTime;
	m_cfg.con_bSets[1] = g_bSetPhoto;
	m_cfg.con_bSets[2] = g_bGrayScale;
	CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY);
	if (pComBox != nullptr)
	{
		CString strJpgQuality;
		pComBox->GetWindowText(strJpgQuality);
		int nJpgQuality = 90;
		CStdTpl::ConvertFromString(nJpgQuality, CMfcStrFile::CString2string(strJpgQuality));
		if (nJpgQuality >= 0 && nJpgQuality <= 100)
		{
			m_cfg.con_nJpgQuality = nJpgQuality;
			CxImageUser::SetJpgQuality(nJpgQuality);
		}
	}

	//������浽���ļ��ж�����ͬһ�ļ���
	if (bNewDir)
	{
		//��ʱ���浽���ļ���
		m_cfg.con_nSelIndex = 0;
	}
	else
	{
		//��ʱ���浽ͬһ�ļ���
		m_cfg.con_nSelIndex = 1;
	}

	_tstring sSuffixs = CMfcStrFile::CString2string(strSuffixs);
	std::vector<_tstring> vSuffixs = CStdStr::Split(sSuffixs, _T("|"));

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
	if (nFileNum > 0)
	{
		std::vector<_tstring> vFiles, vSaves;
		vFiles = vFilesRead;
		//���Ŀ¼���Ƿ����gif�ļ�
		bool bGif = false;
		for (int i = 0; i < vFiles.size(); ++i)
		{
			_tstring sSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(vFiles[i]));
			if (sSuffix == _T(".gif"))
			{
				bGif = true;
				break;
			}
		}

		//������浽ͬһĿ¼
		std::vector<_tstring> vTimesFiles;
		std::vector<_tstring> vTimesSaves;

		//��������·���õ����·��
		g_strDir = CStdStr::GetCommonDir(vFiles);
		TimesFilesSaves(vFiles, vTimesFiles, vTimesSaves);

		//��ʼ��ʾ����
		ppi->Start();
		if(m_pDlgManager)
		{
			m_pDlgManager->MiddleMainProgress();
			if (bGif)
			{
				m_pDlgManager->SetBarInfo(_T("GIF����������Ժ�ʱ�����Ժ�..."));
			}
			else
			{
				m_pDlgManager->SetBarInfo(PROCESSING_INFO);
			}
			m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
		}

		size_t nProFileNum = vTimesFiles.size();
		g_vLog = std::vector<_tstring>(nProFileNum);

		/************������****************/
		ProcessVector(vTimesFiles, vTimesSaves, ConvertPics, ppi, m_cfg.con_nThreadNum);
		/**********************************/
		if (m_pDlgManager)
		{
			m_pDlgManager->FlashWindow(TRUE);
			m_pDlgManager->SetBarInfo(_T("������ɣ�"));
			m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
			m_pDlgManager->EndMainProgress();
		}
		ppi->End();
	}

	//������ļ���
	const int nDirNum = (int)vDirsRead.size();
	for (int m = 0; m < nDirNum; ++m)
	{
		g_strDir = vDirsRead[m];
		if (_access(CStdStr::ws2s(g_strDir).c_str(), 0) != 0)
		{
			//�ļ��в����������
			continue;
		}
		g_vLog.clear();
		std::vector<_tstring> vFiles;
		std::vector<_tstring> vTimesFiles, vTimesSaves;
		getFiles(g_strDir, vFiles, vSuffixs, true);

		//���Ŀ¼���Ƿ����gif�ļ�
		bool bGif = false;
		for (int i = 0; i < vFiles.size(); ++i)
		{
			_tstring sSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(vFiles[i]));
			if (sSuffix == _T(".gif"))
			{
				bGif = true;
				break;
			}
		}

		TimesFilesSaves(vFiles, vTimesFiles, vTimesSaves);

		//��ʼ��ʾ����
		ppi->Start();
		m_pDlgManager->MiddleMainProgress(m * 100 / nDirFileNum + 1);
		if (bGif)
		{
			m_pDlgManager->SetBarInfo(_T("GIF������Ժ�ʱ�����Ժ�..."));
		}
		else
		{
			m_pDlgManager->SetBarInfo(PROCESSING_INFO);
		}
		m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);

		size_t nProFileNum = vTimesFiles.size();
		g_vLog = std::vector<_tstring>(nProFileNum);

		/************������****************/
		ProcessVector(vTimesFiles, vTimesSaves, ConvertPics, ppi, m_cfg.con_nThreadNum);
		/**********************************/

		m_pDlgManager->FlashWindow(TRUE);
		m_pDlgManager->SetBarInfo(_T("������ɣ�"));
		m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		ppi->End();

		//����log����ǰʱ���Լ���ʱ
		_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(g_strDir) + _T("log.log");
		CStdFile::SaveTXTFile(strMoveLogFile, g_vLog);
	}

	m_pDlgManager->EndMainProgress();

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

	CString strSaveLine = strTips + _T("\t\t") + strTime + _T("\t\t") + strDirFiles + '\n';

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
		m_cfg.nImgToolIndex = 3;
		m_pDlgManager->SetIniConfig(m_cfg);
	}

	MessageBox(_T("��ʱ:") + strTips + "\n\n" + _T("�����������"), _T("�K��꣡"));
	SetFocus();

	//CDialogEx::OnOK();
}


void CConvertor::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT_DIR, _T(""));
	SetDlgItemText(IDC_EDIT_SUFFIXS, INI_SUFFIXS);

	((CButton*)GetDlgItem(IDC_CHECK_JPG))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_PNG))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_BMP))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_GIF))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_NEWDIR))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_SAMEDIR))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_KEEPMODIFYTIME))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_SETPHOTOTIME))->SetCheck(TRUE);

	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY);
	_tstring sJpgQuality = CStdTpl::ConvertToString(90);
	int nJpgQualityIndex = pCombox->FindString(0, CString(sJpgQuality.c_str()));
	if (nJpgQualityIndex != -1)
	{
		pCombox->SetCurSel(nJpgQualityIndex);
	}
	else
	{
		pCombox->AddString(CString(sJpgQuality.c_str()));
		nJpgQualityIndex = pCombox->FindString(0, CString(sJpgQuality.c_str()));
		pCombox->SetCurSel(nJpgQualityIndex);
	}

	//CDialog::OnCancel();
}

void CConvertor::OnDropFiles(HDROP hDropInfo)
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
				_tstring strDirName = CMfcStrFile::CString2string(strDirFiles);
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

void CConvertor::OnBnClickedCheckJpg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bRes = ((CButton*)GetDlgItem(IDC_CHECK_JPG))->GetCheck();

	if (bRes == TRUE)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY))->EnableWindow(TRUE);
	}
	else
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY))->EnableWindow(FALSE);
	}
}


BOOL CConvertor::OnEraseBkgnd(CDC* pDC)
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


HBRUSH CConvertor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);//���ñ���͸��
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CConvertor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

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
