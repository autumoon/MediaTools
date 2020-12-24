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

// MediaToolsDoc.cpp : CMediaToolsDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MediaTools.h"
#endif

#include "MediaToolsDoc.h"
#include "MediaToolsView.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMediaToolsDoc

IMPLEMENT_DYNCREATE(CMediaToolsDoc, CDocument)

BEGIN_MESSAGE_MAP(CMediaToolsDoc, CDocument)
	ON_COMMAND(ID_BUTTON_CVT2EIGHT, &CMediaToolsDoc::OnButtonCvt2eight)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CVT2EIGHT, &CMediaToolsDoc::OnUpdateButtonCvt2eight)
	ON_COMMAND(ID_BUTTON_OPEN, &CMediaToolsDoc::OnButtonOpen)
	ON_COMMAND(ID_BUTTON_CxImageOpen, &CMediaToolsDoc::OnButtonCxImageOpen)
END_MESSAGE_MAP()


// CMediaToolsDoc 构造/析构

CMediaToolsDoc::CMediaToolsDoc()
{
	// TODO: 在此添加一次性构造代码
	ResetAllParameters();
}

CMediaToolsDoc::~CMediaToolsDoc()
{
	ReleaseMemory();
}

DWORD CMediaToolsDoc::GetPropMemSizeOneTime()
{
	DWORD MB = 1024 * 1024;
	DWORD GB = 1024 * MB;

	//最大一次申请1GB
	DWORD dwPropOneTime = GB;

	//获取可用内存的80%作为缓存
	MEMORYSTATUS stat;
	GlobalMemoryStatus(&stat);
	SIZE_T dCurMemAvail = SIZE_T(stat.dwAvailPhys * 0.8);

	if (dCurMemAvail > 32 * GB)
	{
		dwPropOneTime = GB;
	}
	else if (dCurMemAvail > 16 * GB)
	{
		dwPropOneTime = 512 * MB;
	}
	else if (dCurMemAvail > 8 * GB)
	{
		dwPropOneTime = 256 * MB;
	}
	else if (dCurMemAvail > 4 * GB)
	{
		dwPropOneTime = 128 * MB;
	}
	else if (dCurMemAvail > 2 * GB)
	{
		dwPropOneTime = 64 * MB;
	}
	else
	{
		//最小一次申请16MB
		dwPropOneTime = 16 * MB;
	}

	return dwPropOneTime;
}

int CMediaToolsDoc::AllocateMemory(bool bWriteAlone /*= false*/)
{
	if (m_gur == nullptr || !m_gur->IsImgReady())
	{
		return -1;
	}

	//获取参数
	m_nBlockSize = 256;
	m_nCols = m_gur->m_nImgWidth;
	m_nRows = m_gur->m_nImgHeight;
	m_nBandNum = m_gur->m_nBandNum;
	m_lfxstart = m_gur->m_LBX;
	m_lfystart = m_gur->m_LBY;
	m_lfcellsize = m_gur->m_lfGSD;
	m_nbpp = m_gur->m_nBPP;
	m_nbpb = m_gur->m_nBPB;

	//初始化
	m_nBlockSize = m_nRows;

	//获取可用内存的80%作为缓存
	MEMORYSTATUS stat;
	GlobalMemoryStatus(&stat);
	SIZE_T dCurMemAvail = SIZE_T(stat.dwAvailPhys * 0.8);

	//这个值最小为0.5
	double dBandProp = 0.50;
	m_nBlockSize = int(dCurMemAvail * dBandProp / m_nCols / m_nbpp);

	//如果块大小超过1G，则取较小的值1G
	INT64 nMemNeed = (INT64)m_nBlockSize * m_nCols * m_nbpp;
	DWORD dwPropOnetime = GetPropMemSizeOneTime();
	nMemNeed = nMemNeed > dwPropOnetime ? dwPropOnetime : nMemNeed;

	//内存对齐
	nMemNeed = (nMemNeed / 512 + 1) * 512;

	int nbppNeed = m_nbpp != 0 ? m_nbpp : 1;
	m_nBlockSize = int(nMemNeed / m_nCols / nbppNeed);
	m_nBlockSize = m_nRows < m_nBlockSize ? m_nRows : m_nBlockSize;

	//假设一次只能读取15行，用于测试分块
	//m_nBlockSize = 15;

	if (m_nBlockSize == 0)
	{
		//可用内存太少，影像的一行都无法正常读取
		return -1;
	}

	INT64 nAllocateMem = (INT64)m_nBlockSize*m_nCols*m_nbpp / 1024 / 1024; //MB,调试用

	//allocate mem
	m_nMemBlock = int(dCurMemAvail / (m_nBlockSize*m_nCols*m_nbpp));
	int nNeedMemBlockNum = m_nRows % m_nBlockSize == 0 ? m_nRows / m_nBlockSize : m_nRows / m_nBlockSize + 1;;
	m_nMemBlock = nNeedMemBlockNum < m_nMemBlock ? nNeedMemBlockNum : m_nMemBlock;

	//因为有读和写
	if (bWriteAlone && m_nMemBlock >= 2)
	{
		m_nMemBlock /= 2;
	}

	if (m_nMemBlock == 0)
	{
		//可用内存太少，影像的一行都无法正常读取
		return -1;
	}

	m_buf = new unsigned char*[m_nMemBlock];

	if (bWriteAlone)
	{
		m_wbuf = new unsigned char*[m_nMemBlock];
	}
	for (int i = 0; i < m_nMemBlock; ++i)
	{
		try
		{
			m_buf[i] = new unsigned char[m_nBlockSize*m_nCols*m_nbpp];

			if (bWriteAlone)
			{
				m_wbuf[i] = new unsigned char[m_nBlockSize*m_nCols*m_nbpp];
			}
		}
		catch (const std::bad_alloc& e)
		{
			for (int t = 0; t < i; ++t)
			{
				delete[] m_buf[t];
				m_buf[t] = nullptr;
				if (bWriteAlone)
				{
					delete[] m_wbuf[t];
					m_wbuf[t] = nullptr;
				}

			}
			delete[] m_buf;
			m_buf = nullptr;
			if (bWriteAlone)
			{
				delete[] m_wbuf;
				m_wbuf = nullptr;
			}


			return -1;
		}
		memset(m_buf[i], 0, m_nbpp*m_nCols*m_nBlockSize);

		if (bWriteAlone)
		{
			memset(m_wbuf[i], 0, m_nbpp*m_nCols*m_nBlockSize);
		}
	}

	return 0;
}

int CMediaToolsDoc::ReleaseMemory()
{
	CStdTpl::DelPointerSafely(m_gur);
	CStdTpl::DelPointerSafely(m_guw);
	
	//free mem
	for (int i = 0; i < m_nMemBlock; ++i)
	{
		if (m_buf)
		{
			CStdTpl::DelPointerSafely(m_buf[i], true);
		}
		if (m_wbuf)
		{
			CStdTpl::DelPointerSafely(m_wbuf[i], true);
		}
	}

	CStdTpl::DelPointerSafely(m_buf, true);
	CStdTpl::DelPointerSafely(m_wbuf, true);

	return 0;
}

int CMediaToolsDoc::ResetAllParameters()
{
	m_strFilePath = _T("");

	m_nBlockSize = 256;
	m_nMemBlock = 0;
	m_nCols = 0;
	m_nRows = 0;
	m_nBandNum = 0;
	m_lfxstart = 0.0;
	m_lfystart = 0.0;
	m_lfcellsize = 1.0;
	m_nbpp = 0;
	m_nbpb = 0;

	m_buf = nullptr;
	m_wbuf = nullptr;

	m_gur = nullptr;
	m_guw = nullptr;

	return 0;
}

BOOL CMediaToolsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

BOOL CMediaToolsDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->EndProgress();
	pMainFrame->InitTaskBarProgress();
	pMainFrame->m_pTb->Start();

	POSITION pos = CMediaToolsDoc::GetFirstViewPosition();
	CMediaToolsView* pView = (CMediaToolsView*)GetNextView(pos); //第一个view

	m_strFilePath = CMfcStrFile::CString2string(lpszPathName);

	if (pView)
	{
		pView->InitializeFile(m_strFilePath);
	}

	pMainFrame->m_pTb->End();

	return TRUE;
}

// CShowInViewDoc 序列化

void CMediaToolsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CMediaToolsDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CMediaToolsDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CMediaToolsDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMediaToolsDoc 诊断

#ifdef _DEBUG
void CMediaToolsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMediaToolsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMediaToolsDoc 命令

void CMediaToolsDoc::OnButtonCvt2eight()
{
	// TODO: 在此添加命令处理程序代码
	CString strSavePath = CMfcStrFile::SaveSuffixFile(/*CStdStr::GetSuffixOfFile(m_strFilePath, false)*/
		_T(".tif"), CStdStr::GetNameOfFile(m_strFilePath, false) + _T("_8"));

	if (!strSavePath.GetLength())
	{
		return;
	}

	std::string sSavePath = CStdStr::ws2s(CMfcStrFile::CString2string(strSavePath));

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->InitTaskBarProgress();
	pMainFrame->m_pTb->Start();
	//先释放上一次的内存
	ReleaseMemory();

	m_gur = new CGdalUser();
	m_guw = new CGdalUser();

	std::string sFilePath = CStdStr::ws2s(m_strFilePath);
	//注意这里要给true直接转8位
	if (!m_gur->InitializeRead(sFilePath.c_str(), true))
	{
		ReleaseMemory();
		pMainFrame->m_pTb->End();
		return;
	}

	//保存成8位
	if (!m_guw->InitializeCreate(sSavePath.c_str(), GDT_Byte, m_gur->m_nImgWidth, m_gur->m_nImgHeight, m_gur->m_nBandNum))
	{
		return;
	}
	AllocateMemory();
	//分块读取和写入
	pMainFrame->StartProgress(0, m_nRows);

	for (int y1 = 0; y1 < m_nRows;)
	{
		//read to mem
		int y = y1;
		if (y + m_nBlockSize < m_nRows)
		{
			for (int j = 0; j < m_nMemBlock && y + m_nBlockSize < m_nRows; ++j)
			{
				m_gur->ReadImg(0, y, m_nCols, y + m_nBlockSize, m_buf[j],
					m_nCols, m_nBlockSize, m_nBandNum, 0, 0, m_nCols, m_nBlockSize, -1, 0);
				y += m_nBlockSize;
			}
			y = y1;
			for (int j = 0; j < m_nMemBlock && y + m_nBlockSize < m_nRows; ++j)
			{
				m_guw->WriteImg(0, y, m_nCols, y + m_nBlockSize, m_buf[j],
					m_nCols, m_nBlockSize, m_nBandNum, 0, 0, m_nCols, m_nBlockSize, -1, 0);
				memset(m_buf[j], 0, m_nbpp*m_nCols*m_nBlockSize);
				y += m_nBlockSize;
			}
			pMainFrame->m_pTb->SetProgressValue(y, m_nRows);
			pMainFrame->SetProgressPos(y);
		}
		else
		{
			m_gur->ReadImg(0, y, m_nCols, m_nRows, m_buf[0],
				m_nCols, m_nBlockSize, m_nBandNum, 0, 0, m_nCols, m_nRows - y, -1, 0);
			//计算write的值
			m_guw->WriteImg(0, y, m_nCols, m_nRows, m_buf[0],
				m_nCols, m_nRows - y, m_nBandNum, 0, 0, m_nCols, m_nRows - y, -1, 0);
			memset(m_buf[0], 0, m_nbpp*m_nCols*(m_nRows - y));
			y = m_nRows;
			pMainFrame->m_pTb->SetProgressValue(y, m_nRows);
			pMainFrame->SetProgressPos(y);
		}
		//用于结束循环
		y1 = y;
	}

	//析构函数也会释放，或者下次载加载会释放
	ReleaseMemory();
	pMainFrame->m_pTb->End();

	AfxMessageBox(IDS_STRING_TASKOVER);
}

void CMediaToolsDoc::OnUpdateButtonCvt2eight(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_strFilePath.length() > 0)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMediaToolsDoc::OnButtonOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString strImgPath = CMfcStrFile::OpenFile();
	if (strImgPath.GetLength())
	{
		OnOpenDocument(strImgPath);
	}
}

void CMediaToolsDoc::OnButtonCxImageOpen()
{
	//后续开发
	AfxMessageBox(_T("功能开发中！"));
	OnButtonOpen();
}

void CMediaToolsDoc::OnButtonOpendir()
{
	// TODO: 在此添加命令处理程序代码
	CString strImgPath = CMfcStrFile::BrowseDir();
	if (strImgPath.GetLength())
	{
		OnOpenDocument(strImgPath);
	}
}
