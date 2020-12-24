// MergeImgs.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MediaTools.h"
#include "MergeImgs.h"
#include "afxdialogex.h"
#include "DlgManager.h"
#include "MainFrm.h"


// CMergeImgs �Ի���

IMPLEMENT_DYNAMIC(CMergeImgs, CDialogEx)

CMergeImgs::CMergeImgs(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMergeImgs::IDD, pParent)
{
	m_pDlgManager = nullptr;
}

CMergeImgs::~CMergeImgs()
{
}

void CMergeImgs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMergeImgs, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEDIR, &CMergeImgs::OnBnClickedButtonBrowsedir)
	ON_BN_CLICKED(IDOK_MI, &CMergeImgs::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL_MI, &CMergeImgs::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_HORIZONTAL, &CMergeImgs::OnBnClickedRadioHorizontal)
	ON_BN_CLICKED(IDC_RADIO_VERTICAL, &CMergeImgs::OnBnClickedRadioVertical)
	ON_WM_ERASEBKGND()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/******************************qȫ�ֱ���**********************************/
static int g_nImgWidth = 0;
static int g_nImgHeight = 0;
static std::vector<_tstring> g_vLog;
/************************************************************************/

// CMergeImgs ��Ϣ�������
BOOL CMergeImgs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (m_pDlgManager)
	{
		//��ȡ����
		m_pDlgManager->GetIniConfig(m_cfg);
	}

	CSpinButtonCtrl* pSpinW = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WIDTH);
	CSpinButtonCtrl* pSpinH = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_HEIGHT);
	pSpinW->SetRange32(1, INT_MAX);
	pSpinH->SetRange32(1, INT_MAX);
	pSpinW->SetBase(10);
	pSpinH->SetBase(10);

	//���������ļ����ô���״̬
	//�Ƿ񱣳ֱ���
	if (m_cfg.mi_bRemLastDir)
	{
		SetDlgItemText(IDC_EDIT_DIR, CString(m_cfg.mi_strLastDir.c_str()));
	}
	SetDlgItemText(IDC_EDIT_SUFFIXS, CString(m_cfg.mi_strSuffixs.c_str()));

	if (m_cfg.mi_nSelIndex == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_VERTICAL))->SetCheck(TRUE);
		OnBnClickedRadioVertical();
	}
	else if(m_cfg.mi_nSelIndex == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_HORIZONTAL))->SetCheck(TRUE);
		OnBnClickedRadioHorizontal();
	}

	((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->SetCheck(m_cfg.mi_bParameters[0]);
	((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->SetCheck(m_cfg.mi_bParameters[1]);
	((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->SetCheck(m_cfg.mi_bParameters[2]);

	SetDlgItemText(IDC_EDIT_WIDTH, CString(CStdTpl::ConvertToString(m_cfg.mi_nWidth).c_str()));
	SetDlgItemText(IDC_EDIT_HEIGHT, CString(CStdTpl::ConvertToString(m_cfg.mi_nHeight).c_str()));

	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY);

	if (pCombox != nullptr)
	{
		pCombox->AddString(_T("60"));
		pCombox->AddString(_T("80"));
		pCombox->AddString(_T("90"));
		pCombox->AddString(_T("99"));
		const int& nJpgQuality = m_cfg.mi_nJpgQuality;
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
	}

	GetClientRect(m_rect);
	m_cfg.nCateGoryIndex = IMGTOOL_CATE_INDEX;

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CMergeImgs::OnBnClickedButtonBrowsedir()
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

void CMergeImgs::OnBnClickedOk()
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
	m_cfg.mi_bParameters[0] = ((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->GetCheck() == TRUE;
	m_cfg.mi_bParameters[1] = ((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->GetCheck() == TRUE;
	m_cfg.mi_bParameters[2] = ((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->GetCheck() == TRUE;
	CString strWidth, strHeight;
	GetDlgItemText(IDC_EDIT_WIDTH, strWidth);
	GetDlgItemText(IDC_EDIT_HEIGHT, strHeight);
	_tstring sTmp[2];
	sTmp[0] = CMfcStrFile::CString2string(strWidth);
	sTmp[1] = CMfcStrFile::CString2string(strHeight);
	_tstring sWidth = CStdStr::RepalceAll(sTmp[0], _T(","));
	_tstring sHeight = CStdStr::RepalceAll(sTmp[1], _T(","));
	CStdTpl::ConvertFromString(g_nImgWidth, sWidth);
	CStdTpl::ConvertFromString(g_nImgHeight, sHeight);

	if (g_nImgWidth <= 0 && g_nImgHeight <= 0)
	{
		MessageBox(_T("��߲��Ϸ���"), _T("�����Ƿ���"), MB_ICONINFORMATION);
		SetFocus();
		return;
	}

	m_cfg.mi_nWidth = g_nImgWidth;
	m_cfg.mi_nHeight = g_nImgHeight;
	m_cfg.mi_strLastDir = strDirFiles;
	m_cfg.mi_strSuffixs = strSuffixs;
	CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY);
	if (pComBox != nullptr)
	{
		CString strJpgQuality;
		pComBox->GetWindowText(strJpgQuality);
		int nJpgQuality = 90;
		CStdTpl::ConvertFromString(nJpgQuality, CMfcStrFile::CString2string(strJpgQuality));
		if (nJpgQuality >= 0 && nJpgQuality <= 100)
		{
			m_cfg.mi_nJpgQuality = nJpgQuality;
			CxImageUser::SetJpgQuality(nJpgQuality);
		}
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
		std::string sCurItem = CStdStr::ws2s(strCurItem);

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

	//����Ǵ�ֱ
	bool bVertical = ((CButton*)GetDlgItem(IDC_RADIO_VERTICAL))->GetCheck() == TRUE;
	int nLengthLimit = -1;
	_tstring strResSuffix = _T("_Horizontal");
	if (bVertical)
	{
		//��ֱ
		m_cfg.mi_nSelIndex = 1;
		if (m_cfg.mi_bParameters[0])
		{
			nLengthLimit = g_nImgWidth;
		}
		strResSuffix = _T("_Vertical");
	}
	else
	{
		//��ʱ��ˮƽ
		m_cfg.mi_nSelIndex = 0;
		if (m_cfg.mi_bParameters[1])
		{
			nLengthLimit = g_nImgHeight;
		}
	}

	//����ʱ��
	clock_t start, finish;
	double duration;
	start = clock();

	//��ʼ��ʾ����
	m_pDlgManager->m_pMainFrame->InitTaskBarProgress();
	CProgressInterface* ppi = m_pDlgManager->m_pMainFrame->m_pTb;

	//������ļ��������ļ�
	const size_t nFileNum = vFilesRead.size();
	if (nFileNum > 0)
	{
		ppi->Start();
		m_pDlgManager->MiddleMainProgress();
		m_pDlgManager->SetBarInfo(PROCESSING_INFO);
		m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);

		//��������·���õ����·��
		_tstring strCommonDir = CStdStr::GetCommonDir(vFilesRead);
		_tstring strMergeFilePath = CStdStr::AddSlashIfNeeded(strCommonDir) +
			CStdStr::GetNameOfDir(strCommonDir) + strResSuffix + CStdStr::GetSuffixOfFile(vFilesRead[0]);

		CxImageUser::MergeImages(vFilesRead, strMergeFilePath, bVertical, nLengthLimit, m_cfg.mi_bParameters[2], ppi);
		m_pDlgManager->FlashWindow(TRUE);
		m_pDlgManager->SetBarInfo(_T("������ɣ�"));
		m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		ppi->End();
	}

	//������ļ���
	const int nDirNum = (int)vDirsRead.size();
	for (int m = 0; m < nDirNum; ++m)
	{
		_tstring sDir = vDirsRead[m];
		if (_access(CStdStr::ws2s(sDir).c_str(), 0) != 0)
		{
			//�ļ��в����������
			continue;
		}
		g_vLog.clear();
		std::vector<_tstring> vFiles, vSaves;
		getFiles(sDir, vFiles, vSuffixs, true);
		if (vFiles.size() == 0)
		{
			continue;
		}

		_tstring strMergeFilePath = CStdStr::AddSlashIfNeeded(CStdStr::GetDirOfDir(sDir)) +
			CStdStr::GetNameOfDir(sDir) + strResSuffix + CStdStr::GetSuffixOfFile(vFiles[0]);

		ppi->Start();
		m_pDlgManager->MiddleMainProgress(m * 100 / nDirFileNum + 1);
		m_pDlgManager->SetBarInfo(PROCESSING_INFO);
		m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);

		size_t nProFileNum = vFiles.size();

		/************������****************/
		CxImageUser::MergeImages(vFiles, strMergeFilePath, bVertical, nLengthLimit, m_cfg.mi_bParameters[2], ppi);

		g_vLog.push_back(sDir + _T("�ϲ���") + strMergeFilePath + _T("\n"));
		/**********************************/

		m_pDlgManager->FlashWindow(TRUE);
		m_pDlgManager->SetBarInfo(_T("������ɣ�"));
		m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		ppi->End();

		//����log����ǰʱ���Լ���ʱ
		_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(sDir) + _T("log.log");
		CStdFile::SaveTXTFile(strMoveLogFile, g_vLog);
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
		m_cfg.nImgToolIndex = 4;
		m_pDlgManager->SetIniConfig(m_cfg);
	}

	MessageBox(_T("��ʱ:") + strTips + "\n\n" + _T("�����������"), _T("�K��꣡"));
	SetFocus();

	//CDialog::OnOK();
}


void CMergeImgs::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT_DIR, _T(""));
	SetDlgItemText(IDC_EDIT_SUFFIXS, INI_SUFFIXS);

	((CButton*)GetDlgItem(IDC_RADIO_VERTICAL))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_HORIZONTAL))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->SetCheck(FALSE);
	
	SetDlgItemText(IDC_EDIT_WIDTH, _T("1080"));
	SetDlgItemText(IDC_EDIT_HEIGHT, _T("1080"));

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

	OnBnClickedRadioVertical();
}

void CMergeImgs::OnSize(UINT nType, int cx, int cy)
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

void CMergeImgs::OnBnClickedRadioHorizontal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_CHECK_DESWIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_DESHEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN_HEIGHT)->EnableWindow(TRUE);
}

void CMergeImgs::OnBnClickedRadioVertical()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_CHECK_DESWIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_DESHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_HEIGHT)->EnableWindow(FALSE);
}



BOOL CMergeImgs::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnEraseBkgnd(pDC);
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


void CMergeImgs::OnDropFiles(HDROP hDropInfo)
{
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
	CDialogEx::OnDropFiles(hDropInfo);
}
