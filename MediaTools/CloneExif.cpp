// CloneExif.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "CloneExif.h"
#include "afxdialogex.h"
#include "src\Configure.h"
#include "MainFrm.h"
#include "DlgManager.h"
// CCloneExif �Ի���

IMPLEMENT_DYNAMIC(CCloneExif, CDialog)

CCloneExif::CCloneExif(CWnd* pParent /*=NULL*/)
	: CDialog(CCloneExif::IDD, pParent)
{
	m_pDlgManager = nullptr;
}

CCloneExif::~CCloneExif()
{
}

BOOL CCloneExif::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//DragAcceptFiles(TRUE);
 	if (m_pDlgManager)
 	{
		//��ȡ����
		m_pDlgManager->GetIniConfig(m_cfg);
 	}

	SetDlgItemText(IDC_EDIT_FILEREAD, m_cfg.ce_strLast[0].c_str());
	SetDlgItemText(IDC_EDIT_FILEWRITE, m_cfg.ce_strLast[1].c_str());
	SetDlgItemText(IDC_EDIT_DIRREAD, m_cfg.ce_strLast[2].c_str());
	SetDlgItemText(IDC_EDIT_DIRWRITE, m_cfg.ce_strLast[3].c_str());
 
 	//���������ļ����ô���״̬
	if (m_cfg.ce_nSelIndex == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->SetCheck(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_FILEREAD))->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BUTTON_SELREAD)->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_FILEWRITE))->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BUTTON_SELWRITE)->EnableWindow(TRUE);

		((CEdit*)GetDlgItem(IDC_EDIT_DIRREAD))->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BUTTON_SELREADDIR)->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_DIRWRITE))->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BUTTON_SELWRITEDIR)->EnableWindow(FALSE);
	}
	else if (m_cfg.ce_nSelIndex == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_BATCH))->SetCheck(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_DIRREAD))->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BUTTON_SELREADDIR)->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_DIRWRITE))->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BUTTON_SELWRITEDIR)->EnableWindow(TRUE);

		((CEdit*)GetDlgItem(IDC_EDIT_FILEREAD))->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BUTTON_SELREAD)->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_FILEWRITE))->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BUTTON_SELWRITE)->EnableWindow(FALSE);
	}

	GetClientRect(m_rect);
	m_cfg.nCateGoryIndex = IMGTOOL_CATE_INDEX;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCloneExif::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCloneExif, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELREAD, &CCloneExif::OnBnClickedButtonSelread)
	ON_BN_CLICKED(IDC_BUTTON_SELWRITE, &CCloneExif::OnBnClickedButtonSelwrite)
	ON_BN_CLICKED(IDC_BUTTON_SELREADDIR, &CCloneExif::OnBnClickedButtonSelreaddir)
	ON_BN_CLICKED(IDC_BUTTON_SELWRITEDIR, &CCloneExif::OnBnClickedButtonSelwritedir)
	ON_BN_CLICKED(IDOK_CE, &CCloneExif::OnBnClickedOk)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDCANCEL_CE, &CCloneExif::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_SINGLE, &CCloneExif::OnBnClickedRadioSingle)
	ON_BN_CLICKED(IDC_RADIO_BATCH, &CCloneExif::OnBnClickedRadioBatch)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCloneExif ��Ϣ�������


void CCloneExif::OnBnClickedButtonSelread()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFileRead = CMfcStrFile::OpenSuffixFile(3, _T(".jpg"), _T(".jpeg"), _T(".tif"));

	if (strFileRead.GetLength())
	{
		SetDlgItemText(IDC_EDIT_FILEREAD, strFileRead);
	}
}


void CCloneExif::OnBnClickedButtonSelwrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFileWrite = CMfcStrFile::OpenSuffixFile(3, _T(".jpg"), _T(".jpeg"), _T(".tif"));

	if (strFileWrite.GetLength())
	{
		SetDlgItemText(IDC_EDIT_FILEWRITE, strFileWrite);
	}
}


void CCloneExif::OnBnClickedButtonSelreaddir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strDirRead = CMfcStrFile::BrowseDir();

	if (strDirRead.GetLength())
	{
		SetDlgItemText(IDC_EDIT_DIRREAD, strDirRead);
	}
	else
	{
		SetFocus();
	}
}


void CCloneExif::OnBnClickedButtonSelwritedir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strDirRead = CMfcStrFile::BrowseDir();

	if (strDirRead.GetLength())
	{
		SetDlgItemText(IDC_EDIT_DIRWRITE, strDirRead);
	}
	else
	{
		SetFocus();
	}
}

void CCloneExif::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ����һ������������Ŷ�ȡ���ļ�����Ϣ
	wchar_t szDirPath[MAX_PATH + 1] = { 0 };
	// ͨ������iFiles����Ϊ0xFFFFFFFF,����ȡ�õ�ǰ�϶����ļ�������
	// ������Ϊ0xFFFFFFFF,��������Ժ�������������
	UINT nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	// ͨ��ѭ������ȡ���϶��ļ���File Name��Ϣ����������ӵ�ListBox��
	DragQueryFile(hDropInfo, 0, szDirPath, MAX_PATH);

	BOOL bSingle = ((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->GetCheck();

	if (bSingle == TRUE && CStdFile::IfAccessFile(szDirPath))
	{
		SetDlgItemText(IDC_EDIT_FILEWRITE, szDirPath);
		_tstring strFileName = CStdStr::GetNameOfFile(CMfcStrFile::CString2string(szDirPath));
		if (m_pDlgManager)
		{
			m_pDlgManager->SetBarInfo(strFileName + _T(" ���سɹ���"));
		}
	}

	BOOL bBatch = ((CButton*)GetDlgItem(IDC_RADIO_BATCH))->GetCheck();

	if (bBatch == TRUE && _access(CStdStr::ws2s(szDirPath).c_str(), 0) == 0 &&
		!CStdFile::IfAccessFile(szDirPath))
	{
		SetDlgItemText(IDC_EDIT_DIRWRITE, szDirPath);
		_tstring strDirName = CStdStr::GetNameOfDir(CMfcStrFile::CString2string(szDirPath));
		if (m_pDlgManager)
		{
			m_pDlgManager->SetBarInfo(strDirName + _T(" ���سɹ���"));
		}
	}

	// �����˴���ק���������ͷŷ������Դ
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

/******************************��������**********************************/
std::vector<_tstring> g_vLog;
/************************************************************************/

int CCloneExif::ClonePicExif(const _tstring& sInputPath, const _tstring& sSavePath, const size_t& nProcessIndex)
{
	_tstring dstExtension = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sInputPath));

	//����exif��Ϣ������Ϊjpg
	_tstring sSavePathJpg = CStdStr::ReplaceSuffix(sSavePath, _T(".jpg"));

	bool bCloneExifResult = false;

	if (CStdFile::IfAccessFile(sSavePathJpg))
	{
		//Ҳ����ɾ����Ϊ���Է���һֻȥ��ֻ������
		RemoveReadOnlyAttrib(sSavePathJpg);
	}
	if (CxImageUser::pic2pic(sSavePath, sSavePathJpg))
	{
		bCloneExifResult = CloneExif(sInputPath, sSavePathJpg);
		CloneFileTime(sInputPath.c_str(), sSavePath.c_str());
	}
	
	if (bCloneExifResult)
	{
		g_vLog[nProcessIndex] = (sInputPath + _T("->") + sSavePathJpg + _T('\n'));
	}
	else
	{
		//����ʧ��
		g_vLog[nProcessIndex] = (_T("CloneExif") +  sInputPath + _T(" Failed!") + _T('\n'));

		return -1;
	}


	return 0;
}

BOOL CCloneExif::ShowWarnnings()
{
	return MessageBox(_T("1.��ȷ�����롢���Ŀ¼��ȷ��\n2.��ȷ�������ļ��е�Ŀ¼�ṹһ�£��ļ�����ȫһ�£�\n3.������߲����ڵ��ļ����Թ�����¼����־�С�\n\n�Ƿ������"), 
		_T("��ʾ"), MB_YESNO) == IDYES;

}

void CCloneExif::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialog::OnOK();
	if (((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->GetCheck())
	{
		m_cfg.ce_nSelIndex = 0;
		//��������Ƭ
		CString strFileSrc, strFileDst;
		GetDlgItemText(IDC_EDIT_FILEREAD, strFileSrc);
		GetDlgItemText(IDC_EDIT_FILEWRITE, strFileDst);

		if (strFileDst.GetLength() == 0)
		{
			MessageBox(_T("���·�����Ϸ���"), _T("��ʾ"), MB_ICONINFORMATION);
			SetFocus();
			return;
		}

		if (strFileSrc == strFileDst)
		{
			MessageBox(_T("���·�����ܵ�������·����"), _T("��ʾ"), MB_ICONINFORMATION);
			SetFocus();
			return;
		}

		_tstring sSrcPath = CMfcStrFile::CString2string(strFileSrc);
		_tstring sDstPath = CMfcStrFile::CString2string(strFileDst);
		if (m_cfg.ce_bRemLast)
		{

			m_cfg.ce_strLast[0] = sSrcPath;
			m_cfg.ce_strLast[1] = sDstPath;
		}

		//��ʼ�������ռ�
		g_vLog = std::vector<_tstring>(1);
		if (m_pDlgManager)
		{
			m_pDlgManager->SetBarInfo(PROCESSING_INFO);
			m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
		}
		if (ClonePicExif(sSrcPath, sDstPath, 0) < 0)
		{
			if (m_pDlgManager)
			{
				m_pDlgManager->SetBarInfo(_T("��¡EXIFʧ�ܣ�"));
				m_pDlgManager->SetBarInfo(_T("..."), SECOND_INFO_INDEX);
			}
			MessageBox(_T("��¡EXIFʧ�ܣ�"), _T("��ʾ"), MB_ICONERROR);
			SetFocus();

			return;
		}
		if (m_pDlgManager)
		{
			m_pDlgManager->SetBarInfo(_T("��¡EXIF��ɣ�"));
			m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);

			//д��ini�ļ�
			m_pDlgManager->EndMainProgress();
			m_cfg.nImgToolIndex = 2;
			m_pDlgManager->SetIniConfig(m_cfg);
		}

		MessageBox(_T("��¡EXIF��ɣ�"), _T("��ʾ"), MB_ICONINFORMATION);
		SetFocus();
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_BATCH))->GetCheck())
	{
		m_cfg.ce_nSelIndex = 1;
		//����Ƕ�����Ƭ
		CString strSrcDir, strDstDir;
		GetDlgItemText(IDC_EDIT_DIRREAD, strSrcDir);
		GetDlgItemText(IDC_EDIT_DIRWRITE, strDstDir);

		if (strDstDir.GetLength() == 0)
		{
			MessageBox(_T("���·�����Ϸ���"), _T("��ʾ"), MB_ICONINFORMATION);
			SetFocus();
			return;
		}

		if (strSrcDir == strDstDir)
		{
			MessageBox(_T("���·�����ܵ�������·����"), _T("��ʾ"), MB_ICONINFORMATION);
			SetFocus();
			return;
		}

		if (ShowWarnnings() == FALSE)
		{
			SetFocus();
			return;
		}

		_tstring sTmp[2];
		sTmp[0] = CMfcStrFile::CString2string(strSrcDir);
		sTmp[1] = CMfcStrFile::CString2string(strDstDir);

		if (m_cfg.ce_bRemLast)
		{
			m_cfg.ce_strLast[2] = sTmp[0];
			m_cfg.ce_strLast[3] = sTmp[1];
		}

		_tstring sSrcDir(sTmp[0]), sDstDir(sTmp[1]), sSuffixs;
		sSuffixs = m_cfg.ce_strSuffixs;

		std::vector<_tstring> vSaves, vFiles;
		std::vector<_tstring> vSuffixs = CStdStr::Split(sSuffixs, _T("|"));
		getFiles(sDstDir, vSaves, vSuffixs, true);

		for (int i = 0; i < vSaves.size(); ++i)
		{
			_tstring strSavePath = CStdStr::ReplaceAllDistinct(vSaves[i], CStdStr::AddSlashIfNeeded(sDstDir), CStdStr::AddSlashIfNeeded(sSrcDir));
			vFiles.push_back(strSavePath);
		}

		//��ʼ����ͼ�񣬼�¼ʱ��
		clock_t start, finish;
		double duration;
		start = clock();

		//��ʼ��ʾ����
		m_pDlgManager->m_pMainFrame->InitTaskBarProgress();
		CProgressInterface* ppi = m_pDlgManager->m_pMainFrame->m_pTb;
		m_pDlgManager->StartMainProgress();

		ppi->Start();
		m_pDlgManager->MiddleMainProgress();

		m_pDlgManager->SetBarInfo(PROCESSING_INFO);
		m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);

		size_t nProFileNum = vFiles.size();
		g_vLog = std::vector<_tstring>(vFiles.size());

		/************������****************/
		ProcessVector(vFiles, vSaves, ClonePicExif, ppi, m_cfg.ce_nThreadNum);
		/**********************************/


		m_pDlgManager->FlashWindow(TRUE);
		m_pDlgManager->SetBarInfo(_T("������ɣ�"));
		m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		ppi->End();

		finish = clock();
		duration = double(finish - start) / CLOCKS_PER_SEC;
		int nMinutes = int(duration / 60);
		int nSeconds = (int)duration % 60;
		CString strTips;
		strTips.Format(_T("%d��%d��"), nMinutes, nSeconds);

		//����log����ǰʱ���Լ���ʱ
		_tstring strCloneLogFile = CStdStr::AddSlashIfNeeded(sDstDir) + _T("log.log");
		CStdFile::SaveTXTFile(strCloneLogFile, g_vLog);

		CString strTime, strDate; //��ȡϵͳʱ�� ����
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y��%m��%d�� %X");
		strDate = tm.Format("%Y��%m��%d��");

		CString strSaveLine = strTips + _T("\t\t") + strTime + _T("\t\t") + CString(sSrcDir.c_str()) + '\n';

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
			m_pDlgManager->EndMainProgress();
			m_cfg.nImgToolIndex = 2;
			m_pDlgManager->SetIniConfig(m_cfg);
		}

		//������
		std::vector<_tstring>().swap(vSaves);
		MessageBox(_T("��ʱ:") + strTips + "\n\n" + _T("�����������"), _T("�K��꣡"));
		SetFocus();
	}
}

void CCloneExif::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT_FILEREAD, _T(""));
	SetDlgItemText(IDC_EDIT_FILEWRITE, _T(""));
	SetDlgItemText(IDC_EDIT_DIRREAD, _T(""));
	SetDlgItemText(IDC_EDIT_DIRWRITE, _T(""));

	((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_BATCH))->SetCheck(FALSE);

	OnBnClickedRadioSingle();
	//CDialog::OnCancel();
}

void CCloneExif::OnBnClickedRadioSingle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CEdit*)GetDlgItem(IDC_EDIT_FILEREAD))->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_BUTTON_SELREAD)->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_FILEWRITE))->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_BUTTON_SELWRITE)->EnableWindow(TRUE);

	((CEdit*)GetDlgItem(IDC_EDIT_DIRREAD))->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_BUTTON_SELREADDIR)->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_DIRWRITE))->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_BUTTON_SELWRITEDIR)->EnableWindow(FALSE);
}


void CCloneExif::OnBnClickedRadioBatch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CEdit*)GetDlgItem(IDC_EDIT_DIRREAD))->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_BUTTON_SELREADDIR)->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_DIRWRITE))->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_BUTTON_SELWRITEDIR)->EnableWindow(TRUE);

	((CEdit*)GetDlgItem(IDC_EDIT_FILEREAD))->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_BUTTON_SELREAD)->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_FILEWRITE))->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_BUTTON_SELWRITE)->EnableWindow(FALSE);
}


BOOL CCloneExif::OnEraseBkgnd(CDC* pDC)
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


HBRUSH CCloneExif::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CCloneExif::OnSize(UINT nType, int cx, int cy)
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
