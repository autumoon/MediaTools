// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MediaToolsDoc.h : CMediaToolsDoc ��Ľӿ�
//

#pragma once
#include "gdal\GdalUser.h"
#include "src\MfcStrFile.h"
#include "src\StdStrFile.h"

class CMediaToolsView;

class CMediaToolsDoc : public CDocument
{
protected: // �������л�����
	CMediaToolsDoc();
	DECLARE_DYNCREATE(CMediaToolsDoc)

// ����
public:
	_tstring m_strFilePath;

	int m_nBlockSize;
	int m_nMemBlock;
	int m_nCols;
	int m_nRows;
	int m_nBandNum;
	double m_lfxstart;
	double m_lfystart;
	double m_lfcellsize;
	int m_nbpp;
	int m_nbpb;

	unsigned char** m_buf;
	unsigned char** m_wbuf;

	CGdalUser* m_gur;
	CGdalUser* m_guw;
// ����
public:
	DWORD GetPropMemSizeOneTime();

	int AllocateMemory(bool bWriteAlone = false);
	int ReleaseMemory();
	int ResetAllParameters();
	
// ��д
public:
	virtual BOOL OnNewDocument();
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CMediaToolsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnButtonCvt2eight();
	afx_msg void OnUpdateButtonCvt2eight(CCmdUI *pCmdUI);
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonCxImageOpen();
	afx_msg void OnButtonOpendir();
};
