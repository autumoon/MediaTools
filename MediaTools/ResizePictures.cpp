// ResizePictures.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ResizePictures.h"
#include "afxdialogex.h"
#include "src\Configure.h"
#include "DlgManager.h"
#include "MainFrm.h"

// CResizePictures �Ի���

IMPLEMENT_DYNAMIC(CResizePictures, CDialog)

CResizePictures::CResizePictures(CWnd* pParent /*=NULL*/)
	: CDialog(CResizePictures::IDD, pParent)
{
	m_pDlgManager = nullptr;
}

CResizePictures::~CResizePictures()
{
}

void CResizePictures::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CResizePictures, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEDIR, &CResizePictures::OnBnClickedButtonBrowsedir)
	ON_BN_CLICKED(IDOK_RP, &CResizePictures::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL_RP, &CResizePictures::OnBnClickedCancel)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK_DESWIDTH, &CResizePictures::OnBnClickedCheckDeswidth)
	ON_BN_CLICKED(IDC_CHECK_DESHEIGHT, &CResizePictures::OnBnClickedCheckDesheight)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_KEEPSCALE, &CResizePictures::OnBnClickedCheckKeepscale)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CResizePictures::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CResizePictures::OnEnChangeEditHeight)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CResizePictures::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_CHAR)
	{

		//���ؼ��Ƿ��ǵ�ǰ����
		if(GetDlgItem(IDC_EDIT_WIDTH)==GetFocus()|| GetDlgItem(IDC_EDIT_HEIGHT)==GetFocus())
		{

			//���Ctrl��ϼ�
			short nks = GetKeyState(VK_CONTROL);

			if (nks & 0x8000)
			{
				return CDialog::PreTranslateMessage(pMsg);
			}

			//������������
			//ֻ�����������ֺ��˸���������������
			if ((pMsg->wParam >= 0x30 && pMsg->wParam <=  0x39) || (pMsg->wParam  == 0x08))
			{
				return CDialog::PreTranslateMessage(pMsg);
			} 
			else
			{
				MessageBeep(-1);
				pMsg->wParam=NULL;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/******************************qȫ�ֱ���**********************************/
static bool g_bParameters[5] = {0};
static int g_nImgWidth = 0;
static int g_nImgHeight = 0;
static double g_dWHProp = 0.0;
static std::vector<_tstring> g_vLog;
/************************************************************************/

// CResizePictures ��Ϣ�������
BOOL CResizePictures::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	if (m_cfg.rp_bRemLastDir)
	{
		SetDlgItemText(IDC_EDIT_DIR, CString(m_cfg.rp_strLastDir.c_str()));
	}
	SetDlgItemText(IDC_EDIT_SUFFIXS, CString(m_cfg.rp_strSuffixs.c_str()));

	if (m_cfg.rp_nSelIndex == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_RENAME))->SetCheck(TRUE);
	}
	else if(m_cfg.rp_nSelIndex == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_OVERWRITE))->SetCheck(TRUE);
	}

	((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->SetCheck(m_cfg.rp_bParameters[0]);
	((CButton*)GetDlgItem(IDC_CHECK_KEEPSCALE))->SetCheck(m_cfg.rp_bParameters[1]);
	((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->SetCheck(m_cfg.rp_bParameters[2]);
	((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->SetCheck(m_cfg.rp_bParameters[3]);
	((CButton*)GetDlgItem(IDC_CHECK_IGNORE_MIN))->SetCheck(m_cfg.rp_bParameters[4]);

	SetDlgItemText(IDC_EDIT_WIDTH, CString(CStdTpl::ConvertToString(m_cfg.rp_nWidth).c_str()));
	SetDlgItemText(IDC_EDIT_HEIGHT, CString(CStdTpl::ConvertToString(m_cfg.rp_nHeight).c_str()));

	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY);

	if (pCombox != nullptr)
	{
		pCombox->AddString(_T("60"));
		pCombox->AddString(_T("80"));
		pCombox->AddString(_T("90"));
		pCombox->AddString(_T("99"));
		const int& nJpgQuality = m_cfg.rp_nJpgQuality;
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CResizePictures::OnBnClickedButtonBrowsedir()
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

int CResizePictures::ResizePics(const _tstring& sInputPath, const _tstring& sSavePath, const size_t& nProcessIndex)
{
	//Ŀ���ļ�������ֻ���ģ���Ҫȥ��
	if (CStdFile::IfAccessFile(sSavePath))
	{
		RemoveReadOnlyAttrib(sSavePath);
	}

	//��ȡ�ļ�ʱ����Ϣ
	FILETIME ftCreate, ftAccess, ftWrite;

	// -------->��ȡ FileTime
	HANDLE hSrcFile = CreateFile(sInputPath.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSrcFile == INVALID_HANDLE_VALUE || !GetFileTime(hSrcFile, &ftCreate, &ftAccess, &ftWrite))
	{
		return -1;
	}

	//��ȡexif��Ϣ
	ExifPathsTags pathsTags;
	ExifImageFile               inImageFile;
	ExifStatus                  status(EXIF_INVALID_FORMAT_ERROR);
	ExifStatus					openStatus(EXIF_INVALID_FORMAT_ERROR);
	_tstring dstExtension = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sInputPath));
	bool bExifSuffix = IsFileExifSuffix(sInputPath);
	if (bExifSuffix)
	{
#ifdef _UNICODE
		openStatus = inImageFile.open(CStdStr::ws2s(sInputPath).c_str(), "r");
#else
		openStatus = inImageFile.open(sInputPath, "r");
#endif // _UNICODE
		if (openStatus != EXIF_OK)	//������w����Ƭ���򿪺�����´���һ����Ƭ����ԭ������Ƭ���ǵ�
		{
			inImageFile.close();
		}
		status = inImageFile.getAllTags(0xFFE1, "Exif", pathsTags);
		if (status != EXIF_OK)
		{
			inImageFile.close();
		}
	}

	const _tstring& strSrcSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sInputPath, false));
	ENUM_CXIMAGE_FORMATS srcFormat = (ENUM_CXIMAGE_FORMATS)CxImage::GetTypeIdFromName(strSrcSuffix.c_str());
	CxImage image;
	image.Load(sInputPath.c_str(), srcFormat);
	if (!image.IsValid())
	{
		inImageFile.close();

		return -1;
	}

	const uint32_t nImgWidth = image.GetWidth();
	const uint32_t nImgHeight = image.GetHeight();
	
	double dPropWH = (double)image.GetWidth() / image.GetHeight();

	int nNewWidth(g_nImgWidth), nNewHeight(g_nImgHeight);

	//���˷ֱ��ʽ�С��ͼƬ��������
	if (g_bParameters[4])
	{
		if ((int)nImgWidth <= g_nImgWidth && (int)nImgHeight <= g_nImgHeight)
		{
			//�������
			g_vLog[nProcessIndex] = (_T("Ignore ") +  sInputPath + _T(" !") + _T('\n'));
			inImageFile.close();

			return -1;
		}
	}

	double dPropScale = 1.0;
	if (g_bParameters[0])
	{
		//�̶���ȣ����ֱ���
		dPropScale = (double)nNewWidth / nImgWidth;
		nNewHeight = int(nImgHeight * dPropScale + 0.5);
		nNewHeight = nNewHeight % 2 == 0 ? nNewHeight : nNewHeight + 1; 
		if (nNewHeight <= 0)
		{
			nNewHeight = 1;
		}
	}
	else if (g_bParameters[2])
	{
		//�̶��߶ȣ����ֱ���
		dPropScale = (double)nNewHeight / nImgHeight;
		nNewWidth = int(nImgWidth * dPropScale + 0.5);
		nNewWidth = nNewWidth % 2 == 0 ? nNewWidth : nNewWidth + 1;
		if (nNewWidth <= 0)
		{
			nNewWidth = 1;
		}
	}
	else if(g_bParameters[1] && nImgWidth > nNewWidth && nImgHeight > nNewHeight)
	{
		//ֻ������С
		//�Զ�ѡ���Ⱥ͸߶��н�С��ֵ����������
		if (nImgWidth < nImgHeight)
		{
			dPropScale = (double)nNewWidth / nImgWidth;
			if (dPropScale < 1.0)
			{
				nNewHeight = int(nImgHeight * dPropScale + 0.5);
				nNewHeight = nNewHeight % 2 == 0 ? nNewHeight : nNewHeight + 1; 
				if (nNewHeight <= 0)
				{
					nNewHeight = 1;
				}
			}
		}
		else
		{
			dPropScale = (double)nNewHeight / nImgHeight;
			nNewWidth = int(nImgWidth * dPropScale + 0.5);
			nNewWidth = nNewWidth % 2 == 0 ? nNewWidth : nNewWidth + 1;
			if (nNewWidth <= 0)
			{
				nNewWidth = 1;
			}
		}
	}
	else
	{
		nNewWidth = nImgWidth;
		nNewHeight = nImgHeight;
	}

	if (CxImageUser::pic2pic(sInputPath, sSavePath, nNewWidth, nNewHeight, g_bParameters[3]))
	{
		if (status == EXIF_OK)
		{
			//����exif��Ϣ
			SetPicExifInfo(sSavePath, pathsTags);
		}

		//�����ļ�ʱ����Ϣ
		HANDLE hDstFile = CreateFile(sSavePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hDstFile == INVALID_HANDLE_VALUE || !SetFileTime(hDstFile, &ftCreate, &ftAccess, &ftWrite))
		{
			inImageFile.close();

			return -1;
		}
		CloseHandle(hSrcFile);
		CloseHandle(hDstFile);
		g_vLog[nProcessIndex] = (sInputPath + _T("->") + sSavePath + _T('\n'));
	}
	else
	{
		//����ʧ��
		g_vLog[nProcessIndex] = (_T("Resize") +  sInputPath + _T(" Failed!") + _T('\n'));
		inImageFile.close();

		return -1;
	}

	inImageFile.close();

	return 0;
}


int CResizePictures::ResizeOneGif(const _tstring& sInputPath, const _tstring& sSavePath, CProgressInterface* ppi /*= nullptr*/)
{
	const _tstring& strSrcSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sInputPath, false));
	const _tstring& strDstSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sSavePath, false));

	if (strSrcSuffix != _T("gif") || strDstSuffix != _T("gif"))
	{
		return -1;
	}

	//Ŀ���ļ�������ֻ���ģ���Ҫȥ��
	if (CStdFile::IfAccessFile(sSavePath))
	{
		RemoveReadOnlyAttrib(sSavePath);
	}

	CxImage image;
	image.Load(sInputPath.c_str(), CXIMAGE_FORMAT_GIF);

	if (!image.IsValid())
	{
		return -1;
	}

	const uint32_t m_nImgWidth = image.GetWidth();
	const uint32_t m_nImgHeight = image.GetHeight();

	double dPropWH = (double)image.GetWidth() / image.GetHeight();

	int nNewWidthInput = g_nImgWidth;
	int nNewHeightInput = g_nImgHeight;
	int nNewWidth(nNewWidthInput), nNewHeight(nNewHeightInput);

	double dPropScale = 1.0;
	if (g_bParameters[0])
	{
		//�̶���ȣ����ֱ���
		dPropScale = (double)nNewWidth / m_nImgWidth;
		nNewHeight = int(m_nImgHeight * dPropScale + 0.5);
		nNewHeight = nNewHeight % 2 == 0 ? nNewHeight : nNewHeight + 1; 
		if (nNewHeight <= 0)
		{
			nNewHeight = 1;
		}
	}
	else if (g_bParameters[2])
	{
		//�̶��߶ȣ����ֱ���
		dPropScale = (double)nNewHeight / m_nImgHeight;
		nNewWidth = int(m_nImgWidth * dPropScale + 0.5);
		nNewWidth = nNewWidth % 2 == 0 ? nNewWidth : nNewWidth + 1;
		if (nNewWidth <= 0)
		{
			nNewWidth = 1;
		}
	}
	else if(g_bParameters[1])
	{
		//�Զ�ѡ���Ⱥ͸߶��н�С��ֵ����������
		if (m_nImgWidth < m_nImgHeight)
		{
			dPropScale = (double)nNewWidth / m_nImgWidth;
			nNewHeight = int(m_nImgHeight * dPropScale + 0.5);
			nNewHeight = nNewHeight % 2 == 0 ? nNewHeight : nNewHeight + 1; 
			if (nNewHeight <= 0)
			{
				nNewHeight = 1;
			}
		}
		else
		{
			dPropScale = (double)nNewHeight / m_nImgHeight;
			nNewWidth = int(m_nImgWidth * dPropScale + 0.5);
			nNewWidth = nNewWidth % 2 == 0 ? nNewWidth : nNewWidth + 1;
			if (nNewWidth <= 0)
			{
				nNewWidth = 1;
			}
		}
	}

	return CxImageUser::ResizeGif(sInputPath, sSavePath, nNewWidth, nNewHeight, g_bParameters[3], ppi);
}


void CResizePictures::OnBnClickedOk()
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
	g_bParameters[0] = ((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->GetCheck() == TRUE;
	g_bParameters[1] = ((CButton*)GetDlgItem(IDC_CHECK_KEEPSCALE))->GetCheck() == TRUE;
	g_bParameters[2] = ((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->GetCheck() == TRUE;
	g_bParameters[3] = ((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->GetCheck() == TRUE;
	g_bParameters[4] = ((CButton*)GetDlgItem(IDC_CHECK_IGNORE_MIN))->GetCheck() == TRUE;
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

	if (g_nImgWidth <= 0 || g_nImgHeight <= 0)
	{
		MessageBox(_T("��߲��Ϸ���"), _T("�����Ƿ���"), MB_ICONINFORMATION);
		SetFocus();
		return;
	}

	m_cfg.rp_nWidth = g_nImgWidth;
	m_cfg.rp_nHeight = g_nImgHeight;
	for (int i = 0; i < 5; ++i)
	{
		m_cfg.rp_bParameters[i] = g_bParameters[i];
	}
	m_cfg.rp_strLastDir = strDirFiles;
	m_cfg.rp_strSuffixs = strSuffixs;
	CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_JPGQUALITY);
	if (pComBox != nullptr)
	{
		CString strJpgQuality;
		pComBox->GetWindowText(strJpgQuality);
		int nJpgQuality = 90;
		CStdTpl::ConvertFromString(nJpgQuality, CMfcStrFile::CString2string(strJpgQuality));
		if (nJpgQuality >= 0 && nJpgQuality <= 100)
		{
			m_cfg.rp_nJpgQuality = nJpgQuality;
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

	//��������������Ǹ���
	BOOL nOverWrite = ((CButton*)GetDlgItem(IDC_RADIO_OVERWRITE))->GetCheck();
	if (nOverWrite == TRUE)
	{
		//��ʱ�Ǹ���
		if (ShowWarnnings() == FALSE)
		{
			SetFocus();
			return;
		}
		m_cfg.rp_nSelIndex = 1;
	}
	else
	{
		//��ʱ��������
		m_cfg.rp_nSelIndex = 0;
	}

	//����ʱ��
	clock_t start, finish;
	double duration;
	start = clock();

	m_pDlgManager->m_pMainFrame->InitTaskBarProgress();
	CProgressInterface* ppi = m_pDlgManager->m_pMainFrame->m_pTb;
	m_pDlgManager->StartMainProgress();

	//������ļ��������ļ�
	const size_t nFileNum = vFilesRead.size();
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

		//��������������Ǹ���
		if (((CButton*)GetDlgItem(IDC_RADIO_RENAME))->GetCheck() == TRUE)
		{
			for (int i = 0; i < vFiles.size(); ++i)
			{
				_tstring strCurFile = CStdStr::ReplaceSuffix(vFiles[i], _T("autumoon") + CStdStr::GetSuffixOfFile(vFiles[i]));
				vSaves.push_back(strCurFile);
			}
		}
		else
		{
			vSaves = vFiles;
		}

		//��ʼ��ʾ����
		ppi->Start();
		m_pDlgManager->MiddleMainProgress();

		if (m_pDlgManager)
		{
			if (bGif)
			{
				m_pDlgManager->SetBarInfo(_T("GIF����ǳ���ʱ�����Ժ�..."));
			}
			else
			{
				m_pDlgManager->SetBarInfo(PROCESSING_INFO);
			}
			m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
		}

		size_t nProFileNum = vFiles.size();
		g_vLog = std::vector<_tstring>(vFiles.size());

		if (nProFileNum == 1 && bGif)
		{
			//����ǵ���gifͼƬ���ţ��򵥶�����Ϊ����ʾ������ȣ�
			ResizeOneGif(vFiles[0], vSaves[0], ppi);
		}
		else
		{
			/************������****************/
			ProcessVector(vFiles, vSaves, ResizePics, ppi, m_cfg.rp_nThreadNum);
			/**********************************/
		}

		if (m_pDlgManager)
		{
			m_pDlgManager->FlashWindow(TRUE);
			m_pDlgManager->SetBarInfo(_T("������ɣ�"));
			m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
			ppi->End();
		}
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

		//���Ŀ¼���Ƿ����gif�ļ�
		bool bGif = false;
		for (int i = 0; i < vFiles.size(); ++i)
		{
			_tstring strCurFile = vFiles[i];
			_tstring sSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(strCurFile));
			if (sSuffix == _T(".gif"))
			{
				bGif = true;
				break;
			}
		}
		//��������������Ǹ���
		if (nOverWrite == FALSE)
		{
			for (int i = 0; i < vFiles.size(); ++i)
			{
				_tstring strCurFile = vFiles[i];
				_tstring strSaveFile = CStdStr::ReplaceSuffix(strCurFile, _T("autumoon") + CStdStr::GetSuffixOfFile(strCurFile));
				vSaves.push_back(strSaveFile);
			}
		}
		else
		{
			vSaves = vFiles;
		}

		//��ʼ��ʾ����
		ppi->Start();
		m_pDlgManager->MiddleMainProgress(m * 100 / nDirFileNum + 1);
		if (bGif)
		{
			m_pDlgManager->SetBarInfo(_T("GIF����ǳ���ʱ�����Ժ�..."));
		}
		else
		{
			m_pDlgManager->SetBarInfo(PROCESSING_INFO);
		}
		m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);

		size_t nProFileNum = vFiles.size();
		g_vLog = std::vector<_tstring>(nProFileNum);

		/************������****************/
		ProcessVector(vFiles, vSaves, ResizePics, ppi, m_cfg.rp_nThreadNum);
		/**********************************/

		if (m_pDlgManager)
		{
			m_pDlgManager->FlashWindow(TRUE);
			m_pDlgManager->SetBarInfo(_T("������ɣ�"));
			m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		}
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
		m_cfg.nImgToolIndex = 1;
		m_pDlgManager->SetIniConfig(m_cfg);
	}

	MessageBox(_T("��ʱ:") + strTips + "\n\n" + _T("�����������"), _T("�K��꣡"));
	SetFocus();

	//CDialog::OnOK();
}


void CResizePictures::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT_DIR, _T(""));
	SetDlgItemText(IDC_EDIT_SUFFIXS, INI_SUFFIXS);

	((CButton*)GetDlgItem(IDC_RADIO_RENAME))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_OVERWRITE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_KEEPSCALE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_GRAYSCALE))->SetCheck(FALSE);

	SetDlgItemText(IDC_EDIT_WIDTH, _T("1920"));
	SetDlgItemText(IDC_EDIT_HEIGHT, _T("1920"));

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


void CResizePictures::OnDropFiles(HDROP hDropInfo)
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

	if(strDirFiles.GetLength())
	{
		if (nNum == 1)
		{
			_tstring strItemPath = CMfcStrFile::CString2string(strDirFiles);
			_tstring strDirName = CStdStr::GetNameOfFile(strItemPath);
			//����ǵ����ļ��Ļ������ȡ��߱�
			if (CStdFile::IfAccessFile(strItemPath))
			{
				CxImage image;
				image.Load(strItemPath.c_str());
				image.RotateExif();
				if (image.IsValid())
				{
					uint32_t w = image.GetWidth();
					uint32_t h = image.GetHeight();
					g_dWHProp = (double)w / h;
					CString strTips;
					strTips.Format(_T("%d x %d"), w, h);
					m_pDlgManager->SetBarInfo(_T("ԭʼ�ߴ磺") + CMfcStrFile::CString2string(strTips));
				}
				else
				{
					g_dWHProp = 0.0;
					m_pDlgManager->SetBarInfo(strDirName + _T(" ����ʧ�ܣ�"));
				}
			}
			else
			{
				g_dWHProp = 0.0;
				m_pDlgManager->SetBarInfo(strDirName + _T(" ���سɹ���"));
			}
		}
		else
		{
			g_dWHProp = 0.0;
			m_pDlgManager->SetBarInfo(_T("����ļ�(��)���سɹ���"));
		}
	}
	else
	{
		g_dWHProp = 0.0;
		m_pDlgManager->SetBarInfo(_T("ʲôҲû�з�����"));
	}

	// �����˴���ק���������ͷŷ������Դ
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

BOOL CResizePictures::ShowWarnnings()
{
	return MessageBox(_T("1.�Ѿ�ѡ�񸲸�ģʽ����;�벻Ҫ�˳����򣬷������ݿ��ܶ�ʧ��\n2.��ȷ�������Ѿ����ݹ�����ȷ����ѡ�ļ���Ŀ¼��ȷ��\n3.������GIF����������ǳ���ʱ�������ĵȴ���\n\n�Ƿ������"), 
		_T("����ģʽ��ʾ"), MB_YESNO) == IDYES;

}

bool CResizePictures::SetPicExifInfo(_tstring strFilePath, const ExifPathsTags& pathsTags)
{
	ExifImageFile               inImageFile;
	ExifStatus                  status;

	std::string sPath = CStdStr::ws2s(strFilePath);

	ExifStatus openStatus = inImageFile.open(sPath.c_str(), "r+");
	if (openStatus != EXIF_OK)			// ������w����Ƭ���򿪺�����´���һ����Ƭ����ԭ������Ƭ���ǵ�
	{
		inImageFile.close();
		return false;
	}

	status = inImageFile.setAllTags(0xFFE1, "Exif", pathsTags);
	if (status != EXIF_OK)				// exif��Ϣ����,���¸�ֵ
	{
		inImageFile.close();
		return false;
	}

	return true;
}


void CResizePictures::OnBnClickedCheckDeswidth()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bDesWidth = ((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->GetCheck() == TRUE;
	if (bDesWidth)
	{
		//���ֻ�̶ܹ�һ�������Ҵ�ʱ���뱣�ֱ���
		((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_KEEPSCALE))->SetCheck(TRUE);
	}
}


void CResizePictures::OnBnClickedCheckKeepscale()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bKeepScale = ((CButton*)GetDlgItem(IDC_CHECK_KEEPSCALE))->GetCheck() == TRUE;
	if (!bKeepScale)
	{
		//���ֻ�̶ܹ�һ�������Ҵ�ʱ���뱣�ֱ���
		((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->SetCheck(FALSE);
	}
}

void CResizePictures::OnBnClickedCheckDesheight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bDesHeight = ((CButton*)GetDlgItem(IDC_CHECK_DESHEIGHT))->GetCheck() == TRUE;
	if (bDesHeight)
	{
		//���ֻ�̶ܹ�һ��
		((CButton*)GetDlgItem(IDC_CHECK_DESWIDTH))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_KEEPSCALE))->SetCheck(TRUE);
	}
}

BOOL CResizePictures::OnEraseBkgnd(CDC* pDC)
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


HBRUSH CResizePictures::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CResizePictures::OnEnChangeEditWidth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	if (GetDlgItem(IDC_EDIT_WIDTH) != GetFocus()) 
	{
		return;
	}

	g_bParameters[1] = ((CButton*)GetDlgItem(IDC_CHECK_KEEPSCALE))->GetCheck() == TRUE;
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strWidth, strHeight;
	GetDlgItemText(IDC_EDIT_WIDTH, strWidth);
	_tstring sTmp[2];
	sTmp[0] = CMfcStrFile::CString2string(strWidth);
	_tstring sWidth = CStdStr::RepalceAll(sTmp[0], _T(","));
	CStdTpl::ConvertFromString(g_nImgWidth, sWidth);
	//����Ѿ������˵�����Ƭ��������Ҫ���ֱ�������ͬ�������Ӧ��ֵ
	if (g_bParameters[1] && g_dWHProp > 1e-5 && g_nImgWidth > 0)
	{
		//��ʱ���������һ��ֵ
		int nTmpH = int(g_nImgWidth / g_dWHProp + 0.5);
		if (nTmpH != g_nImgHeight)
		{
			g_nImgHeight = nTmpH % 2 == 0 ? nTmpH : nTmpH + 1;
			strHeight.Format(_T("%d"), g_nImgHeight);
			SetDlgItemText(IDC_EDIT_HEIGHT, strHeight);
		}
	}
}

void CResizePictures::OnEnChangeEditHeight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	if (GetDlgItem(IDC_EDIT_HEIGHT) != GetFocus()) 
	{
		return;
	}

	g_bParameters[1] = ((CButton*)GetDlgItem(IDC_CHECK_KEEPSCALE))->GetCheck() == TRUE;
 	// TODO:  �ڴ���ӿؼ�֪ͨ����������
 	CString strWidth, strHeight;
 	GetDlgItemText(IDC_EDIT_HEIGHT, strHeight);
 	_tstring sTmp[2];
 	sTmp[1] = CMfcStrFile::CString2string(strHeight);
 	_tstring sHeight = CStdStr::RepalceAll(sTmp[1], _T(","));
 	CStdTpl::ConvertFromString(g_nImgHeight, sHeight);
 	//����Ѿ������˵�����Ƭ��������Ҫ���ֱ�������ͬ�������Ӧ��ֵ
 	if (g_bParameters[1] && g_dWHProp > 1e-5 && g_nImgHeight > 0)
 	{
 		//��ʱ���������һ��ֵ
 		int nTmpW = int(g_nImgHeight * g_dWHProp + 0.5);
		if (nTmpW != g_nImgWidth)
		{
			g_nImgWidth = nTmpW % 2 == 0 ? nTmpW : nTmpW + 1;
			strWidth.Format(_T("%d"), g_nImgWidth);
			SetDlgItemText(IDC_EDIT_WIDTH, strWidth);
		}
 	}
}


void CResizePictures::OnSize(UINT nType, int cx, int cy)
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
