// Convertor.cpp : 实现文件
//

#include "stdafx.h"
#include "Convertor.h"
#include "afxdialogex.h"
#include "DlgManager.h"
#include "MainFrm.h"


// CConvertor 对话框

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

/******************************q全局变量**********************************/
static bool g_bKeepTime = true, g_bSetPhoto = true, g_bGrayScale = false;
static _tstring g_strDir;
static std::vector<_tstring> g_vLog;
/************************************************************************/

// CConvertor 消息处理程序
BOOL CConvertor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_pDlgManager)
	{
		//读取配置
		m_pDlgManager->GetIniConfig(m_cfg);
	}

	//根据配置文件设置窗口状态
	//是否保持比例
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
	// 异常: OCX 属性页应返回 FALSE
}

void CConvertor::OnBnClickedButtonBrowsedir()
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

int CConvertor::TimesFilesSaves(const std::vector<_tstring>& vFiles, std::vector<_tstring>& vTimesFiles, std::vector<_tstring>& vTimesSaves)
{
	//存储到新目录
	bool bNewDir = m_cfg.con_nSelIndex == 0;
	std::vector<_tstring> vSuffixs;
	//目标类型，注意都是小写，不包含*
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

	//每种格式分别处理
	if (bNewDir && g_strDir.length() > 0)
	{
		for (int i = 0; i < vSuffixs.size(); ++i)
		{
			const _tstring& strCurSuffix = vSuffixs[i];
			const _tstring& strDirName = CStdStr::ToUpperLower(strCurSuffix, false);
			for (int j = 0; j < vFiles.size(); ++j)
			{
				const _tstring& strCurFile = vFiles[j];
				//不管后缀是否相同，都需要创建并且更新新文件
				_tstring strSubPath = strCurFile.substr(g_strDir.length());
				_tstring strNewTmpPath = CStdStr::AddSlashIfNeeded(g_strDir) + strDirName + strSubPath;
				_tstring strNewPath = CStdStr::ReplaceSuffix(strNewTmpPath, _T('.') + strCurSuffix);
				_tstring strDstDir = CStdStr::GetDirOfFile(strNewPath);
				if (_access(CStdStr::ws2s(strDstDir).c_str(), 0) != 0)
				{
					CStdDir::CreateDir(strDstDir);
				}

				int nNum = 0;
				//这里提前预防重名文件
				if (CStdTpl::VectorContains(vTimesSaves, strNewPath) || CStdFile::IfAccessFile(strNewPath.c_str()))
				{
					do
					{
						++nNum;
						strNewPath = CStdStr::AddSlashIfNeeded(strDstDir) + CStdStr::GetNameOfFile(strNewTmpPath, false) + _T("_") +
							CStdTpl::ConvertToString(nNum) + CStdStr::GetSuffixOfFile(strNewPath);

					} while (CStdTpl::VectorContains(vTimesSaves, strNewPath) || CStdFile::IfAccessFile(strNewPath.c_str()));
				}
				
				//配对输出
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

				//配对输出
				int nNum = 0;
				//这里保证不会复写已经存在的文件，并且提前预防重名文件
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
	//获取exif信息
	_tstring srcExtension = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sInputPath));
	_tstring dstExtension = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sSavePath));

	if (CxImageUser::pic2pic(sInputPath, sSavePath, -1, -1, g_bGrayScale))
	{
		g_vLog[nProcessIndex] = (sInputPath + _T("->") + sSavePath + _T('\n'));
	}
	else
	{
		//保存失败
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

	//设置时间
	if (g_bKeepTime)
	{
		CloneFileTime(CString(sInputPath.c_str()), CString(sSavePath.c_str()));
	}

	return 0;
}

void CConvertor::OnBnClickedOk()
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

	//如果保存到新文件夹而不是同一文件夹
	if (bNewDir)
	{
		//此时保存到新文件夹
		m_cfg.con_nSelIndex = 0;
	}
	else
	{
		//此时保存到同一文件夹
		m_cfg.con_nSelIndex = 1;
	}

	_tstring sSuffixs = CMfcStrFile::CString2string(strSuffixs);
	std::vector<_tstring> vSuffixs = CStdStr::Split(sSuffixs, _T("|"));

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
	if (nFileNum > 0)
	{
		std::vector<_tstring> vFiles, vSaves;
		vFiles = vFilesRead;
		//检测目录中是否包含gif文件
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

		//如果保存到同一目录
		std::vector<_tstring> vTimesFiles;
		std::vector<_tstring> vTimesSaves;

		//根据输入路径得到输出路径
		g_strDir = CStdStr::GetCommonDir(vFiles);
		TimesFilesSaves(vFiles, vTimesFiles, vTimesSaves);

		//开始显示进度
		ppi->Start();
		if(m_pDlgManager)
		{
			m_pDlgManager->MiddleMainProgress();
			if (bGif)
			{
				m_pDlgManager->SetBarInfo(_T("GIF动画处理相对耗时，请稍后..."));
			}
			else
			{
				m_pDlgManager->SetBarInfo(PROCESSING_INFO);
			}
			m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);
		}

		size_t nProFileNum = vTimesFiles.size();
		g_vLog = std::vector<_tstring>(nProFileNum);

		/************主程序****************/
		ProcessVector(vTimesFiles, vTimesSaves, ConvertPics, ppi, m_cfg.con_nThreadNum);
		/**********************************/
		if (m_pDlgManager)
		{
			m_pDlgManager->FlashWindow(TRUE);
			m_pDlgManager->SetBarInfo(_T("处理完成！"));
			m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
			m_pDlgManager->EndMainProgress();
		}
		ppi->End();
	}

	//如果是文件夹
	const int nDirNum = (int)vDirsRead.size();
	for (int m = 0; m < nDirNum; ++m)
	{
		g_strDir = vDirsRead[m];
		if (_access(CStdStr::ws2s(g_strDir).c_str(), 0) != 0)
		{
			//文件夹不存在则继续
			continue;
		}
		g_vLog.clear();
		std::vector<_tstring> vFiles;
		std::vector<_tstring> vTimesFiles, vTimesSaves;
		getFiles(g_strDir, vFiles, vSuffixs, true);

		//检测目录中是否包含gif文件
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

		//开始显示进度
		ppi->Start();
		m_pDlgManager->MiddleMainProgress(m * 100 / nDirFileNum + 1);
		if (bGif)
		{
			m_pDlgManager->SetBarInfo(_T("GIF处理相对耗时，请稍后..."));
		}
		else
		{
			m_pDlgManager->SetBarInfo(PROCESSING_INFO);
		}
		m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);

		size_t nProFileNum = vTimesFiles.size();
		g_vLog = std::vector<_tstring>(nProFileNum);

		/************主程序****************/
		ProcessVector(vTimesFiles, vTimesSaves, ConvertPics, ppi, m_cfg.con_nThreadNum);
		/**********************************/

		m_pDlgManager->FlashWindow(TRUE);
		m_pDlgManager->SetBarInfo(_T("处理完成！"));
		m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		ppi->End();

		//保存log，当前时间以及耗时
		_tstring strMoveLogFile = CStdStr::AddSlashIfNeeded(g_strDir) + _T("log.log");
		CStdFile::SaveTXTFile(strMoveLogFile, g_vLog);
	}

	m_pDlgManager->EndMainProgress();

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

	CString strSaveLine = strTips + _T("\t\t") + strTime + _T("\t\t") + strDirFiles + '\n';

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
		m_cfg.nImgToolIndex = 3;
		m_pDlgManager->SetIniConfig(m_cfg);
	}

	MessageBox(_T("耗时:") + strTips + "\n\n" + _T("处理完成啦！"), _T("終わり！"));
	SetFocus();

	//CDialogEx::OnOK();
}


void CConvertor::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
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
				_tstring strDirName = CMfcStrFile::CString2string(strDirFiles);
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

void CConvertor::OnBnClickedCheckJpg()
{
	// TODO: 在此添加控件通知处理程序代码
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


HBRUSH CConvertor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);//设置背景透明
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CConvertor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

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
