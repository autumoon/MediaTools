#pragma once
#include "src\Configure.h"
#include "src\MfcStrFile.h"


class CDlgManager;
// CMergeImgs �Ի���

class CMergeImgs : public CDialogEx
{
	DECLARE_DYNAMIC(CMergeImgs)

public:
	CMergeImgs(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMergeImgs();

// �Ի�������
	enum { IDD = IDD_DIALOG_MERGEIMGS };

public:
	CDlgManager* m_pDlgManager;

protected:
	config_s m_cfg;
	CRect m_rect;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowsedir();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioHorizontal();
	afx_msg void OnBnClickedRadioVertical();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
