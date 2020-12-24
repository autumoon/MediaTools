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

// MediaToolsDoc.h : CMediaToolsDoc 类的接口
//

#pragma once
#include "gdal\GdalUser.h"
#include "src\MfcStrFile.h"
#include "src\StdStrFile.h"

class CMediaToolsView;

class CMediaToolsDoc : public CDocument
{
protected: // 仅从序列化创建
	CMediaToolsDoc();
	DECLARE_DYNCREATE(CMediaToolsDoc)

// 特性
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
// 操作
public:
	DWORD GetPropMemSizeOneTime();

	int AllocateMemory(bool bWriteAlone = false);
	int ReleaseMemory();
	int ResetAllParameters();
	
// 重写
public:
	virtual BOOL OnNewDocument();
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CMediaToolsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnButtonCvt2eight();
	afx_msg void OnUpdateButtonCvt2eight(CCmdUI *pCmdUI);
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonCxImageOpen();
	afx_msg void OnButtonOpendir();
};
