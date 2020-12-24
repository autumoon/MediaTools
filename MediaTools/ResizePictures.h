#pragma once
#include "src/StdStrFile.h"
#include "src/MfcStrFile.h"
#include "src/tvmain.h"
#include "src/Configure.h"
#include "cximage/cxImageUser.h"
#include "src/ProgressInterface.h"
#include "resource.h"

//EXIF相关
#include "openExif/ExifImageDescUtils.h"
#include "openExif/ExifImageFile.h"
#include "openExif/ExifMisc.h"

class CDlgManager;

// CResizePictures 对话框

class CResizePictures : public CDialog
{
	DECLARE_DYNAMIC(CResizePictures)

public:
	CResizePictures(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResizePictures();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 对话框数据
	enum { IDD = IDD_DIALOG_RESIZEPICS };

public:
	CDlgManager* m_pDlgManager;

protected:
	CRect m_rect;
	config_s m_cfg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL ShowWarnnings();

	static bool SetPicExifInfo(_tstring strFilePath, const ExifPathsTags& pathsTags);
	static int ResizePics(const _tstring& sInputPath, const _tstring& sSavePath, const size_t& nProcessIndex);
	static int ResizeOneGif(const _tstring& sInputPath, const _tstring& sSavePath, CProgressInterface* ppi = nullptr);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowsedir();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedCheckDeswidth();
	afx_msg void OnBnClickedCheckDesheight();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckKeepscale();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
