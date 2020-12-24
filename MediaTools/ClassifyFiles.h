#pragma once
#include "src\FileProperty.h"
#include "src\StdStrFile.h"
#include "src\MfcStrFile.h"
#include "src\GetDirFiles.h"
#include "src\tvmain.h"
#include "src\Configure.h"
#include "resource.h"
#include "cximage\cxImageUser.h"

// ClassifyFiles �Ի���
class CDlgManager;

class CClassifyFiles : public CDialog
{
	DECLARE_DYNAMIC(CClassifyFiles)

public:
	CClassifyFiles(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClassifyFiles();
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_CLASSIFYFILES };

public:
	CDlgManager* m_pDlgManager;
	

protected:
	CRect m_rect;
	config_s m_cfg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

	BOOL ShowWarnnings();
	int ProcessSingleDir(const CString& strSingleDir);
	static int MoveFilesByTime(const _tstring& pStr, const _tstring& pSavePath, const size_t& nProcessIndex);
	static int MovePicsByResolution(const _tstring& pStr, const _tstring& pSavePath, const size_t& nProcessIndex);
	static int MovePicsByTime(const _tstring& pStr, const _tstring& pSavePath, const size_t& nProcessIndex);
	static int RestoreFile(const _tstring& pStr, const _tstring& pSavePath, const size_t& nProcessIndex);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonDir();
	afx_msg void OnBnClickedRadioPics();
	afx_msg void OnBnClickedRadioFiles();
	afx_msg void OnBnClickedRadioRec();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
