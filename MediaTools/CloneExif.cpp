// CloneExif.cpp : 实现文件
//

#include "stdafx.h"

#include "CloneExif.h"
#include "afxdialogex.h"
#include "src\Configure.h"
#include "MainFrm.h"
#include "DlgManager.h"
// CCloneExif 对话框

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

	// TODO:  在此添加额外的初始化
	//DragAcceptFiles(TRUE);
 	if (m_pDlgManager)
 	{
		//读取配置
		m_pDlgManager->GetIniConfig(m_cfg);
 	}

	SetDlgItemText(IDC_EDIT_FILEREAD, m_cfg.ce_strLast[0].c_str());
	SetDlgItemText(IDC_EDIT_FILEWRITE, m_cfg.ce_strLast[1].c_str());
	SetDlgItemText(IDC_EDIT_DIRREAD, m_cfg.ce_strLast[2].c_str());
	SetDlgItemText(IDC_EDIT_DIRWRITE, m_cfg.ce_strLast[3].c_str());
 
 	//根据配置文件设置窗口状态
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
	// 异常: OCX 属性页应返回 FALSE
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


// CCloneExif 消息处理程序


void CCloneExif::OnBnClickedButtonSelread()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFileRead = CMfcStrFile::OpenSuffixFile(3, _T(".jpg"), _T(".jpeg"), _T(".tif"));

	if (strFileRead.GetLength())
	{
		SetDlgItemText(IDC_EDIT_FILEREAD, strFileRead);
	}
}


void CCloneExif::OnBnClickedButtonSelwrite()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFileWrite = CMfcStrFile::OpenSuffixFile(3, _T(".jpg"), _T(".jpeg"), _T(".tif"));

	if (strFileWrite.GetLength())
	{
		SetDlgItemText(IDC_EDIT_FILEWRITE, strFileWrite);
	}
}


void CCloneExif::OnBnClickedButtonSelreaddir()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 定义一个缓冲区来存放读取的文件名信息
	wchar_t szDirPath[MAX_PATH + 1] = { 0 };
	// 通过设置iFiles参数为0xFFFFFFFF,可以取得当前拖动的文件数量，
	// 当设置为0xFFFFFFFF,函数间忽略后面两个参数。
	UINT nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	// 通过循环依次取得拖动文件的File Name信息，并把它添加到ListBox中
	DragQueryFile(hDropInfo, 0, szDirPath, MAX_PATH);

	BOOL bSingle = ((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->GetCheck();

	if (bSingle == TRUE && CStdFile::IfAccessFile(szDirPath))
	{
		SetDlgItemText(IDC_EDIT_FILEWRITE, szDirPath);
		_tstring strFileName = CStdStr::GetNameOfFile(CMfcStrFile::CString2string(szDirPath));
		if (m_pDlgManager)
		{
			m_pDlgManager->SetBarInfo(strFileName + _T(" 加载成功！"));
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
			m_pDlgManager->SetBarInfo(strDirName + _T(" 加载成功！"));
		}
	}

	// 结束此次拖拽操作，并释放分配的资源
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

/******************************公共变量**********************************/
std::vector<_tstring> g_vLog;
/************************************************************************/

int CCloneExif::ClonePicExif(const _tstring& sInputPath, const _tstring& sSavePath, const size_t& nProcessIndex)
{
	_tstring dstExtension = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(sInputPath));

	//增加exif信息，保存为jpg
	_tstring sSavePathJpg = CStdStr::ReplaceSuffix(sSavePath, _T(".jpg"));

	bool bCloneExifResult = false;

	if (CStdFile::IfAccessFile(sSavePathJpg))
	{
		//也可以删除，为了以防万一只去掉只读属性
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
		//保存失败
		g_vLog[nProcessIndex] = (_T("CloneExif") +  sInputPath + _T(" Failed!") + _T('\n'));

		return -1;
	}


	return 0;
}

BOOL CCloneExif::ShowWarnnings()
{
	return MessageBox(_T("1.请确保输入、输出目录正确！\n2.请确保两个文件夹的目录结构一致，文件名完全一致！\n3.错误或者不存在的文件会略过并记录到日志中。\n\n是否继续？"), 
		_T("提示"), MB_YESNO) == IDYES;

}

void CCloneExif::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialog::OnOK();
	if (((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->GetCheck())
	{
		m_cfg.ce_nSelIndex = 0;
		//处理单张照片
		CString strFileSrc, strFileDst;
		GetDlgItemText(IDC_EDIT_FILEREAD, strFileSrc);
		GetDlgItemText(IDC_EDIT_FILEWRITE, strFileDst);

		if (strFileDst.GetLength() == 0)
		{
			MessageBox(_T("输出路径不合法！"), _T("提示"), MB_ICONINFORMATION);
			SetFocus();
			return;
		}

		if (strFileSrc == strFileDst)
		{
			MessageBox(_T("输出路径不能等于输入路径！"), _T("提示"), MB_ICONINFORMATION);
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

		//初始化保留空间
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
				m_pDlgManager->SetBarInfo(_T("克隆EXIF失败！"));
				m_pDlgManager->SetBarInfo(_T("..."), SECOND_INFO_INDEX);
			}
			MessageBox(_T("克隆EXIF失败！"), _T("提示"), MB_ICONERROR);
			SetFocus();

			return;
		}
		if (m_pDlgManager)
		{
			m_pDlgManager->SetBarInfo(_T("克隆EXIF完成！"));
			m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);

			//写入ini文件
			m_pDlgManager->EndMainProgress();
			m_cfg.nImgToolIndex = 2;
			m_pDlgManager->SetIniConfig(m_cfg);
		}

		MessageBox(_T("克隆EXIF完成！"), _T("提示"), MB_ICONINFORMATION);
		SetFocus();
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_BATCH))->GetCheck())
	{
		m_cfg.ce_nSelIndex = 1;
		//如果是多张照片
		CString strSrcDir, strDstDir;
		GetDlgItemText(IDC_EDIT_DIRREAD, strSrcDir);
		GetDlgItemText(IDC_EDIT_DIRWRITE, strDstDir);

		if (strDstDir.GetLength() == 0)
		{
			MessageBox(_T("输出路径不合法！"), _T("提示"), MB_ICONINFORMATION);
			SetFocus();
			return;
		}

		if (strSrcDir == strDstDir)
		{
			MessageBox(_T("输出路径不能等于输入路径！"), _T("提示"), MB_ICONINFORMATION);
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

		//开始处理图像，记录时间
		clock_t start, finish;
		double duration;
		start = clock();

		//开始显示进度
		m_pDlgManager->m_pMainFrame->InitTaskBarProgress();
		CProgressInterface* ppi = m_pDlgManager->m_pMainFrame->m_pTb;
		m_pDlgManager->StartMainProgress();

		ppi->Start();
		m_pDlgManager->MiddleMainProgress();

		m_pDlgManager->SetBarInfo(PROCESSING_INFO);
		m_pDlgManager->SetBarInfo(BUSY_INFO_TEXT, SECOND_INFO_INDEX);

		size_t nProFileNum = vFiles.size();
		g_vLog = std::vector<_tstring>(vFiles.size());

		/************主程序****************/
		ProcessVector(vFiles, vSaves, ClonePicExif, ppi, m_cfg.ce_nThreadNum);
		/**********************************/


		m_pDlgManager->FlashWindow(TRUE);
		m_pDlgManager->SetBarInfo(_T("处理完成！"));
		m_pDlgManager->SetBarInfo(OVER_INFO_TEXT, SECOND_INFO_INDEX);
		ppi->End();

		finish = clock();
		duration = double(finish - start) / CLOCKS_PER_SEC;
		int nMinutes = int(duration / 60);
		int nSeconds = (int)duration % 60;
		CString strTips;
		strTips.Format(_T("%d分%d秒"), nMinutes, nSeconds);

		//保存log，当前时间以及耗时
		_tstring strCloneLogFile = CStdStr::AddSlashIfNeeded(sDstDir) + _T("log.log");
		CStdFile::SaveTXTFile(strCloneLogFile, g_vLog);

		CString strTime, strDate; //获取系统时间 　　
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y年%m月%d日 %X");
		strDate = tm.Format("%Y年%m月%d日");

		CString strSaveLine = strTips + _T("\t\t") + strTime + _T("\t\t") + CString(sSrcDir.c_str()) + '\n';

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
			m_pDlgManager->EndMainProgress();
			m_cfg.nImgToolIndex = 2;
			m_pDlgManager->SetIniConfig(m_cfg);
		}

		//清理工作
		std::vector<_tstring>().swap(vSaves);
		MessageBox(_T("耗时:") + strTips + "\n\n" + _T("处理完成啦！"), _T("終わり！"));
		SetFocus();
	}
}

void CCloneExif::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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


HBRUSH CCloneExif::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CCloneExif::OnSize(UINT nType, int cx, int cy)
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
