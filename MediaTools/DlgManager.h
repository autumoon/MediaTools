#pragma once
#include "src\StdStrFile.h"
#include "MediaToolsView.h"
#include "ClassifyFiles.h"
#include "ResizePictures.h"
#include "CloneExif.h"
#include "Convertor.h"
#include "MergeImgs.h"

class CMainFrame;

class CDlgManager
{
public:
	CDlgManager(void);
	~CDlgManager(void);

public:
	CMainFrame* m_pMainFrame;
	CMediaToolsView* m_pMTView;

	//×Ó´°¿Ú
	CClassifyFiles* m_pClassifyDlg;
	CResizePictures* m_pResizeDlg;
	CCloneExif* m_pCloneExifDlg;
	CConvertor* m_pConvertorDlg;
	CMergeImgs* m_pMergeImgsDlg;

public:
	bool IsReady();
	bool Initialize(CMainFrame* pMainFrame, CMediaToolsView* pMTView);
	BOOL FlashWindow(BOOL bInvert);

	int GetIniConfig(config_s& cfg);
	int HideAllDlgs();
	int SetBarInfo(const _tstring& strInfo, size_t nIndex = 0);
	int SetIniConfig(const config_s& cfg);
	int ShowClassifyDlg(bool bShow = true);
	int ShowCloneExifDlg(bool bShow = true);
	int ShowConvertorfDlg(bool bShow = true);
	int ShowResizeDlg(bool bShow = true);
	int ShowMergeImgsDlg(bool bShow = true);

	//MainProgress
	int StartMainProgress();
	int MiddleMainProgress(const int& nMiddle = 50);
	int EndMainProgress();

	_tstring GetIniPath();
};

