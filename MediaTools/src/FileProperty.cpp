#include "FileProperty.h"

//----------- Error Handling Function -------------------
void error(LPSTR lpszFunction)
{
	CHAR szBuf[80];
	DWORD dw = GetLastError();

	sprintf(szBuf, "%s failed: GetLastError returned %u\n",
		lpszFunction, dw);

	MessageBox(NULL, CString(szBuf), _T("Error"), MB_OK);
	ExitProcess(dw);
}
//--------------------------------------------------------

bool CloneExif(const _tstring& strSrcFile, const _tstring& strDstFile)
{
	//获取exif信息
	ExifPathsTags pathsTags;
	ExifImageFile               inImageFile;
	ExifStatus                  status(EXIF_INVALID_FORMAT_ERROR);
	ExifStatus					openStatus(EXIF_INVALID_FORMAT_ERROR);
	if (!IsFileExifSuffix(strSrcFile) || !IsFileExifSuffix(strDstFile))
	{
		return false;
	}
	openStatus = inImageFile.open(CStdStr::ws2s(strSrcFile).c_str(), "r");
	if (openStatus != EXIF_OK)	//不能用w打开照片，打开后会重新创建一张照片并将原来的照片覆盖掉
	{
		inImageFile.close();

		return false;
	}
	status = inImageFile.getAllTags(0xFFE1, "Exif", pathsTags) ;
	if (status != EXIF_OK)
	{
		inImageFile.close();

		return false;
	}

	ExifImageFile               outImageFile;
	ExifStatus                  out_status;
	std::string sDstFile = CStdStr::ws2s(strDstFile);
	const char* chPath = sDstFile.c_str();
	ExifStatus openout_status = outImageFile.open(chPath, "r+");
	if (openout_status != EXIF_OK)			// 不能用w打开照片，打开后会重新创建一张照片并将原来的照片覆盖掉
	{
		outImageFile.close();
		return false;
	}
	out_status = outImageFile.setAllTags(0xFFE1, "Exif", pathsTags);
	if (out_status != EXIF_OK)
	{
		outImageFile.close();
		return false;
	}

	return true;
}

bool CloneFileTime(LPCWSTR strSrcFile, LPCWSTR strDstFile)
{
	if (!CStdFile::IfAccessFile(strSrcFile) || !CStdFile::IfAccessFile(strDstFile))
	{
		return FALSE;
	}

	FILETIME ftCreate, ftAccess, ftWrite;

	// -------->获取 FileTime
	HANDLE hSrcFile = CreateFile(strSrcFile, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSrcFile == INVALID_HANDLE_VALUE || !GetFileTime(hSrcFile, &ftCreate, &ftAccess, &ftWrite))
	{
		//error("GetFileTime()");
		return FALSE;
	}

	HANDLE hDstFile = CreateFile(strDstFile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (hDstFile == INVALID_HANDLE_VALUE || !SetFileTime(hDstFile, &ftCreate, &ftAccess, &ftWrite))
	{
		//error("GetFileTime()");
		return FALSE;
	}

	CloseHandle(hSrcFile);
	CloseHandle(hDstFile);

	return TRUE;
}

bool GetFileTime(HANDLE hFile, LPWSTR lpszCreationTime, LPWSTR lpszLastAccessTime, LPWSTR lpszLastWriteTime)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC1, stLocal1, stUTC2, stLocal2, stUTC3, stLocal3;

	// -------->获取 FileTime
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		//error("GetFileTime()");
		return FALSE;
	}
	//---------> 转换: FileTime --> LocalTime
	FileTimeToSystemTime(&ftCreate, &stUTC1);
	FileTimeToSystemTime(&ftAccess, &stUTC2);
	FileTimeToSystemTime(&ftWrite, &stUTC3);

	SystemTimeToTzSpecificLocalTime(NULL, &stUTC1, &stLocal1);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC2, &stLocal2);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC3, &stLocal3);

	// ---------> Show the  date and time.
	wsprintf(lpszCreationTime, _T("%d%02d%02d-%02d%02d"),
		stLocal1.wYear, stLocal1.wMonth, stLocal1.wDay,
		stLocal1.wHour, stLocal1.wMinute);
	wsprintf(lpszLastAccessTime, _T("%d%02d%02d-%02d%02d"),
		stLocal2.wYear, stLocal2.wMonth, stLocal2.wDay,
		stLocal2.wHour, stLocal2.wMinute);
	wsprintf(lpszLastWriteTime, _T("%d%02d%02d-%02d%02d"),
		stLocal3.wYear, stLocal3.wMonth, stLocal3.wDay,
		stLocal3.wHour, stLocal3.wMinute);

	return TRUE;
}

bool IsFileExifSuffix(const _tstring& strFilePath)
{
	std::string dstExtension = CStdStr::ws2s(CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(strFilePath)));

	return 0 == strcmp(dstExtension.c_str(), ".jpg") || 0 == strcmp(dstExtension.c_str(), ".jpeg")/* || 0 == strcmp(dstExtension.c_str(), ".tif")*/;
}

bool RemoveReadOnlyAttrib(const _tstring& strFilePath)
{
	//-------------------将只读文件的只读属性去掉-----------------------
	CFileStatus rStatus;
	//获得文件的属性设置
	if (CFile::GetStatus(strFilePath.c_str(),rStatus) == FALSE)
	{
		return false;
	}
	rStatus.m_attribute=rStatus.m_attribute & 0x3E ;
	CFile::SetStatus(strFilePath.c_str(), rStatus );//更改文件的属性设置

	return true;
}

CString GetModifyYearMonth(LPCWSTR strFilePath)
{
	HANDLE hFile;
	TCHAR szCreationTime[30], szLastAccessTime[30], szLastWriteTime[30];
	hFile = CreateFile(strFilePath, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileTime(hFile, szCreationTime, szLastAccessTime, szLastWriteTime);
	CString strWriteTime(szLastWriteTime);
	int nLength = strWriteTime.GetLength();
	if (hFile == INVALID_HANDLE_VALUE || nLength == 0)
	{
		//error("GetLastWriteTime()");
		return CString();
	}
	CloseHandle(hFile);

	return nLength >= 6 ? strWriteTime.Mid(0, 6) : strWriteTime;
}

bool SetPhotoTime(const _tstring& strInputTimeFile, const _tstring& strDstFile)
{
	if (!IsFileExifSuffix(strDstFile))
	{
		return false;
	}

	//获取时间，转换为EXIF格式
	HANDLE hFile;
	TCHAR szLastWriteTime[30];
	hFile = CreateFile(CString(strInputTimeFile.c_str()), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC3, stLocal3;

	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		//error("GetFileTime()");
		return false;
	}
	FileTimeToSystemTime(&ftWrite, &stUTC3);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC3, &stLocal3);
	wsprintf(szLastWriteTime, _T("%d:%02d:%02d %02d:%02d:%02d"),
		stLocal3.wYear, stLocal3.wMonth, stLocal3.wDay,
		stLocal3.wHour, stLocal3.wMinute, stLocal3.wSecond);

	_tstring strTime = CMfcStrFile::CString2string(szLastWriteTime);
	CloseHandle(hFile);

	ExifImageFile               outImageFile;
	const char* chPath = CStdStr::ws2s(strDstFile).c_str();
	ExifStatus openout_status = outImageFile.open(chPath, "r+");
	if (openout_status != EXIF_OK)			// 不能用w打开照片，打开后会重新创建一张照片并将原来的照片覆盖掉
	{
		outImageFile.close();
		return false;
	}

	ExifTagEntry* genericTagEntry = new ExifTagEntryT<std::string>(EXIFTAG_DATETIMEORIGINAL, EXIF_ASCII, 20, CStdStr::ws2s(strTime).c_str());
	bool bRes = outImageFile.setGenericTag(*genericTagEntry, EXIF_APP1_EXIFIFD) == EXIF_OK;
	outImageFile.close();

	return bRes;
}


