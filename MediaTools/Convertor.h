#pragma once

#include "src/FileProperty.h"
#include "src/StdStrFile.h"
#include "src/MfcStrFile.h"
#include "src/tvmain.h"
#include "src/Configure.h"
#include "cximage/cxImageUser.h"
#include "resource.h"

class CDlgManager;
// CConvertor 对话框

class CConvertor : public CDialogEx
{
	DECLARE_DYNAMIC(CConvertor)

public:
	CConvertor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConvertor();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONVERTOR };
public:
	CDlgManager* m_pDlgManager;
	

protected:
	CRect m_rect;
	config_s m_cfg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	static int ConvertPics(const _tstring& sInputPath, const _tstring& sSavePath, const size_t& nProcessIndex);
	int TimesFilesSaves(const std::vector<_tstring>& vFiles, std::vector<_tstring>& vTimesFiles, std::vector<_tstring>& vTimesSaves);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonBrowsedir();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckJpg();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
