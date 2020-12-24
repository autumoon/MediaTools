#pragma once

#include "src\StdStrFile.h"
#include "src\MfcStrFile.h"
#include "src\GetDirFiles.h"
#include "src\HiperLink.h"
#include "afxwin.h"
#include "src\tvmain.h"
#include "src\Configure.h"
#include "resource.h"

class CDlgManager;

// CCloneExif 对话框

class CCloneExif : public CDialog
{
	DECLARE_DYNAMIC(CCloneExif)

public:
	CCloneExif(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCloneExif();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CLONEEXIF };
public:
	CDlgManager* m_pDlgManager;

protected:
	CRect m_rect;
	config_s m_cfg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL ShowWarnnings();
	static int ClonePicExif(const _tstring& sInputPath, const _tstring& sSavePath, const size_t& nProcessIndex);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonSelread();
	afx_msg void OnBnClickedButtonSelwrite();
	afx_msg void OnBnClickedButtonSelreaddir();
	afx_msg void OnBnClickedButtonSelwritedir();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioSingle();
	afx_msg void OnBnClickedRadioBatch();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
