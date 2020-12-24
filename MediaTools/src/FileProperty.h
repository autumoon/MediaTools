#pragma once

#include "StdStrFile.h"
#include "MfcStrFile.h"
#include "GetDirFiles.h"

//EXIFœ‡πÿ
#include "..\openExif\ExifImageDescUtils.h"
#include "..\openExif\ExifImageFile.h"
#include "..\openExif\ExifMisc.h"

bool CloneExif(const _tstring& strSrcFile, const _tstring& strDstFile);
bool CloneFileTime(LPCWSTR strSrcFile, LPCWSTR strDstFile);
bool GetFileTime(HANDLE hFile, LPWSTR lpszCreationTime, LPWSTR lpszLastAccessTime, LPWSTR lpszLastWriteTime);
bool IsFileExifSuffix(const _tstring& strFilePath);
bool RemoveReadOnlyAttrib(const _tstring& strFilePath);
bool SetPhotoTime(const _tstring& strInputTimeFile, const _tstring& strDstFile);

CString GetModifyYearMonth(LPCWSTR strFilePath);