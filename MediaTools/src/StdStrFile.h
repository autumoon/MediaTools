#pragma once
#include <list>
#include <string>
#include <vector>
#include <sstream>
#include <direct.h>		//_mkdir函数的头文件
#include <io.h>
#include <tchar.h>
#include <codecvt>		//utf8\16编码转换

#ifdef _UNICODE

#ifndef _tstring
#define _tstring std::wstring
#endif // _tstring

#else

#ifndef _tstring
#define _tstring std::string
#endif // _tstring

#endif

class CStdStr
{
public:
	//字符操作
	static _tstring AddSlashIfNeeded(const _tstring& strDir, const TCHAR& cDir = '\\');
	static _tstring GetDirOfDir(const _tstring& strDir, const TCHAR& cDir = '\\');
	static _tstring GetDirOfFile(const _tstring& strFile, const TCHAR& cDir = '\\');
	static _tstring GetCommonDir(const std::vector<_tstring>& vFullPaths);
	//在文件同目录新建一个子目录，返回子目录中的以当前文件为名的全路径，通常用于将文件保存到同目录中的某个新建子目录中
	static _tstring GetFileDirSavePath(const _tstring& strFilePath, const _tstring& strDirName, const _tstring& strAddSuffix = _T(""));
	static _tstring GetRootDir(const _tstring& strDirOrFile, const TCHAR& cDir = '\\');
	static _tstring GetSuffixOfFile(const _tstring& strFile, bool bWithDot = true);
	static _tstring GetNameOfDir(const _tstring& strDir, const TCHAR& cDir = '\\');
	static _tstring GetNameOfFile(const _tstring& strFile, const bool& bWithSuffix = true, const TCHAR& cDir = '\\');
	static _tstring RepalceAll(const _tstring& str,const _tstring& old_value,const _tstring& new_value = _T(""));
	static _tstring ReplaceAllDistinct(const _tstring& str,const _tstring& old_value,const _tstring& new_value = _T(""));
	static _tstring ReplaceSuffix(const _tstring& strFilePath, const _tstring& strNewSuffix);
	static _tstring ToUpperLower(const _tstring& strOri, const bool& bToLower = true);
	static _tstring Trim(const _tstring& strOri, const TCHAR& cToTrim = ' ');
	static _tstring TrimHead(const _tstring& strOri, const TCHAR& cToTrim = ' ');
	static _tstring TrimTail(const _tstring& strOri, const TCHAR& cToTrim = ' ');

	static bool IsDigital(const _tstring& strCstring, const bool& bRegardDotAsNum = true);
	static bool IsFileContainsDir(const _tstring& strFile, const _tstring& strDir);
	static bool IsFilesContainsDir(const std::vector<_tstring>& vFiles, const _tstring& strDir);

	static int SetDigitalTimes(const int& nInputNum, const int& nBaseNum = 2, const bool& bUpper = true);

	static std::string ws2s(const std::wstring& ws);
	static std::wstring s2ws(const std::string& s);

	//多字节非Unicode下重载
	static std::string ws2s(const std::string& s){return s;};
	static std::wstring s2ws(const std::wstring& ws){return ws;};

	static std::vector<_tstring> Split(const _tstring& str, const _tstring& pattern);
};

class CStdDir
{
public:
	static bool IfAccessDir(const _tstring& strDirPath);
	static bool CreateDir(const _tstring& strDir, const TCHAR& cDir = '\\');
};

class CStdFile
{
public:
	//文件操作
	static __int64 GetFileSize(const _tstring& strFilePath);

	static bool CompareFileDistinct(const _tstring strLeftFile, const _tstring strRightFile);
	static bool CopyAFile(const _tstring& strSrcFileName, const _tstring& strDstFileName, const bool& bFailIfExists);
	static bool IfAccessFile(const _tstring& strFilePath);
	
	static int SaveTXTFile(const _tstring& strTxtPath, std::list<_tstring>& lContent, bool bAppend = false);
	static int SaveTXTFile(const _tstring& strTxtPath, std::vector<_tstring>& vContent, bool bAppend = false);
	static int SaveTXTFile(const _tstring& strTxtPath, const _tstring& strLine, bool bAppend = false);

	static size_t ParseTXTFile(const _tstring& strFilePath, std::list<_tstring>& lContentInFile);
	static size_t ParseTXTFile(const _tstring& strFilePath, std::vector<_tstring>& vContentInFile);
	static size_t ParseTXTUtf8(const _tstring& strFilePath, std::vector<_tstring>& vContentInFile);

	static _tstring CStdFile::GetFileEncoding(const _tstring& strFile);
};

class CStdTpl
{
public:
	//nStrictSeq = -2环形逆向严格包含,-1逆向严格包含，0或者其他值不讲究顺序，1正向严格包含，2环形正向严格包含
	typedef enum _VectorContainFlag
	{
		eCircleReverseContain = -2,
		eReverseContain = -1,
		eNoSeq = 0,
		eSequenceContain = 1,
		eCircleSequenceContain = 2
	}VectorContainFlag;

public:
	template <class T>
	static bool DelPointerSafely(T*& pPointer, const bool& bArray = false);
	template <class T>
	//如果是结构体或者类，自带构造函数，则bIniZero必须为false，否则构造函数失效
	static bool NewSafely(T*& pPointer, const size_t& nSize = 1, const bool& bIniZero = false);

	//nStrictSeq = -2环形逆向严格包含,-1逆向严格包含，0或者其他值不讲究顺序，1正向严格包含，2环形正向严格包含
	template <class T>
	static bool VectorContains(const std::vector<T>& vTs, const T& value, bool bPositiveGoing = true);
	template <class T>
	static bool VectorContains(const std::vector<T>& vTsSum, const std::vector<T>& vTsPart, const VectorContainFlag& nStrictSeq = eNoSeq);
	template <class T>
	static int VectorFindIndex(const std::vector<T>& vTs, const T& value, bool bPositiveGoing = true);

	//返回第一个查找到的最大值的索引
	template <class T>
	static int FindVectorMaxMin(const std::vector<T>& vTs, bool bFindMax);

	//指针对象包含关系
	template <class T>
	static bool VectorPtrContains(const std::vector<T*>& vTs, const T* value, bool bPositiveGoing = true);
	template <class T>
	static int VectorPtrFindIndex(const std::vector<T*>& vTs, const T* value, bool bPositiveGoing = true);


	//l = 0, r = vTs.size() - 1
	template <class T>
	static void QuickSort(std::vector<T*>& vpTs, int l, int r);
	template <class T>

	static void* ConvertFromString(T &value, const _tstring &s);
	template <class T>
	static _tstring ConvertToString(T value);

private:
	template <class T>
	static bool VectorContainsStrict(const std::vector<T>& vTsSum, const std::vector<T>& vTsPart, bool bPositiveGoing = true);
	template <class T>
	static bool VectorContainsStrictCircle(const std::vector<T>& vTsSum, const std::vector<T>& vTsPart, bool bPositiveGoing = true);
};

/***********************内联或者模板实现***********************/
inline bool CStdDir::IfAccessDir(const _tstring& strDirPath)
{
#ifdef _UNICODE
	std::string sDirPath = CStdStr::ws2s(strDirPath);
	return _access(sDirPath.c_str(), 0) == 0 && !CStdFile::IfAccessFile(strDirPath);
#else
	return _access(strDirPath.c_str(), 0) == 0 && !CStdFile::IfAccessFile(strDirPath);
#endif // _UNICODE
}

template <class T>
inline bool CStdTpl::DelPointerSafely(T*& pPointer, const bool& bArray /*= false*/)
{
	if (pPointer != nullptr)
	{
		if (bArray)
		{
			delete[] pPointer;
		}
		else
		{
			delete pPointer;
		}
		pPointer = nullptr;

		return true;
	}

	return false;
}

template <class T>
bool CStdTpl::NewSafely(T*& pPointer, const size_t& nSize /*= 1*/, const bool& bIniZero /*= false*/)
{
	try
	{
		pPointer = new T[nSize];

		if (bIniZero)
		{
			memset(pPointer, 0, nSize * sizeof(T));
		}
	}
	catch ( const std::bad_alloc& e ) 
	{
		pPointer = nullptr;

		return false;
	}

	return true;
}

template <class T>
inline bool CStdTpl::VectorContains(const std::vector<T>& vTs, const T& value, bool bPositiveGoing /*= true*/)
{
	return VectorFindIndex(vTs, value, bPositiveGoing) != -1;
}

template <class T>
bool CStdTpl::VectorContains(const std::vector<T>& vTsSum, const std::vector<T>& vTsPart, const VectorContainFlag& nStrictSeq /*= NoSeq*/)
{
	if (vTsPart.size() == 0)
	{
		//空集是任何集合的子集
		return true;
	}

	switch (nStrictSeq)
	{
	case eCircleReverseContain:
		return VectorContainsStrictCircle(vTsSum, vTsPart, false);
		//break;
	case eReverseContain:
		//如果判断必须严格按照顺序包含
		return VectorContainsStrict(vTsSum, vTsPart, false);
		//break;
	case eSequenceContain:
		//如果判断必须严格按照顺序包含
		return VectorContainsStrict(vTsSum, vTsPart, true);
		//break;
	case eCircleSequenceContain:
		return VectorContainsStrictCircle(vTsSum, vTsPart, true);
		//break;
	default:
		{
			for (size_t i = 0; i < vTsPart.size(); ++i)
			{
				if (!VectorContains(vTsSum, vTsPart[i]))
				{
					return false;
				}
			}

			return true;
		}
		//break;
	}

	return false;
}

template <class T>
inline bool CStdTpl::VectorContainsStrict(const std::vector<T>& vTsSum, const std::vector<T>& vTsPart, bool bPositiveGoing /*= true*/)
{
	if (vTsPart.size() == 0)
	{
		//空集是任何集合的子集
		return true;
	}
	if (vTsPart.size() == 1)
	{
		//查找是否包含即可
		return VectorContains(vTsSum, vTsPart[0]);
	}
	else
	{
		//一一对应查找
		const size_t nSumSize = vTsSum.size();
		const size_t nPartSize = vTsPart.size();

		if (nSumSize < nPartSize)
		{
			return false;
		}

		std::vector<T> vTsPartBak(vTsPart);
		if (!bPositiveGoing)
		{
			std::reverse(vTsPartBak.begin(), vTsPartBak.end());
		}

		//找出第一个元素在原来合集中第一次出现的位置
		int nSIndex = VectorFindIndex(vTsSum, vTsPartBak[0]);
		//找出最后一个元素在原来合集中反向第一次出现的位置
		int nEIndex = VectorFindIndex(vTsSum, vTsPartBak[vTsPartBak.size() - 1], false);

		if (nSIndex == -1 || nEIndex == -1 || nSIndex >= nEIndex)
		{
			return false;
		}

		for (int i = nSIndex; i < nEIndex && i - nSIndex < nPartSize; ++i)
		{
			if (vTsSum[i] != vTsPartBak[i - nSIndex])
			{
				return false;
			}
		}

		return true;
	}
}

template <class T>
inline bool CStdTpl::VectorContainsStrictCircle(const std::vector<T>& vTsSum, const std::vector<T>& vTsPart, bool bPositiveGoing /*= true*/)
{
	if (vTsPart.size() == 0)
	{
		//空集是任何集合的子集
		return true;
	}
	if (vTsPart.size() == 1)
	{
		//查找是否包含即可
		return VectorContains(vTsSum, vTsPart[0]);
	}
	else
	{
		//一一对应查找
		const size_t nSumSize = vTsSum.size();
		const size_t nPartSize = vTsPart.size();

		if (nSumSize < nPartSize)
		{
			return false;
		}

		std::vector<T> vTsPartBak(vTsPart);
		if (!bPositiveGoing)
		{
			std::reverse(vTsPartBak.begin(), vTsPartBak.end());
		}

		//找出第一个元素在原来合集中第一次出现的位置
		int nSIndex = VectorFindIndex(vTsSum, vTsPartBak[0]);
		//找出最后一个元素在原来合集中反向第一次出现的位置
		int nEIndex = VectorFindIndex(vTsSum, vTsPartBak[vTsPartBak.size() - 1], false);

		if (nSIndex == -1 || nEIndex == -1 || nSIndex == nEIndex)
		{
			return false;
		}

		if (nSIndex < nEIndex)
		{
			for (int i = nSIndex; i <= nEIndex && i - nSIndex < nPartSize; ++i)
			{
				if (vTsSum[i] != vTsPartBak[i - nSIndex])
				{
					return false;
				}
			}
		}
		else
		{
			//公共边可能被首尾点截断
			int nTmpPos = 0;
			for (int i = nSIndex; i < nSumSize && i - nSIndex < nPartSize; ++i)
			{
				if (vTsSum[i] != vTsPartBak[i - nSIndex])
				{
					return false;
				}
				nTmpPos++;
			}

			for (int i = 0; i <= nEIndex && i + nTmpPos < nPartSize; ++i)
			{
				if (vTsSum[i] != vTsPartBak[i + nTmpPos])
				{
					return false;
				}
			}
		}

		return true;
	}
}

template <class T>
inline int CStdTpl::VectorFindIndex(const std::vector<T>& vTs, const T& value, bool bPositiveGoing /*= true*/)
{
	if (bPositiveGoing)
	{
		for (int i = 0; i < int(vTs.size()); ++i)
		{
			if (vTs[i] == value)
			{
				return i;
			}
		}
	}
	else
	{
		for (int i = int(vTs.size() - 1); i >= 0; --i)
		{
			if (vTs[i] == value)
			{
				return i;
			}
		}
	}

	return -1;
}

template <class T>
int CStdTpl::FindVectorMaxMin(const std::vector<T>& vTs, bool bFindMax)
{
	T Ttmp;
	int index = -1;

	if (bFindMax)
	{
		for (int i = 0; i < int(vTs.size()); ++i)
		{
			if (i != 0)
			{
				if (vTs[i] > Ttmp)
				{
					index = i;
					Ttmp = vTs[i];
				}
			}
			else
			{
				Ttmp = vTs[i];
				index = 0;
			}
		}
	}
	else
	{
		for (int i = 0; i < int(vTs.size()); ++i)
		{
			if (i != 0)
			{
				if (vTs[i] < Ttmp)
				{
					index = i;
					Ttmp = vTs[i];
				}
			}
			else
			{
				Ttmp = vTs[i];
				index = 0;
			}
		}
	}

	return index;
}

template <class T>
bool CStdTpl::VectorPtrContains(const std::vector<T*>& vTs, const T* value, bool bPositiveGoing /*= true*/)
{
	return VectorPtrFindIndex(vTs, value, bPositiveGoing) != -1;
}

template <class T>
int CStdTpl::VectorPtrFindIndex(const std::vector<T*>& vTs, const T* value, bool bPositiveGoing /*= true*/)
{
	if (bPositiveGoing)
	{
		for (int i = 0; i < int(vTs.size()); ++i)
		{
			if (*vTs[i] == *value)
			{
				return i;
			}
		}
	}
	else
	{
		for (int i = int(vTs.size() - 1); i >= 0; --i)
		{
			if (*vTs[i] == *value)
			{
				return i;
			}
		}
	}

	return -1;
}

template <class T>
void CStdTpl::QuickSort(std::vector<T*>& vpTs, int l, int r)
{
	if (l < r)
	{
		int i,j;
		i = l;
		j = r;
		T* x = vpTs[i];
		while (i < j)
		{
			while(i < j && *(vpTs[j]) > *x)
				j--; // 从右向左找第一个小于x的数
			if(i < j)
				vpTs[i++] = vpTs[j];
			while(i < j && *(vpTs[i]) < *x)
				i++; // 从左向右找第一个大于x的数
			if(i < j)
				vpTs[j--] = vpTs[i];
		}
		vpTs[i] = x;
		QuickSort(vpTs, l, i-1); /* 递归调用 */
		QuickSort(vpTs, i+1, r); /* 递归调用 */
	}
}

template <class T>
inline void* CStdTpl::ConvertFromString(T &value, const _tstring &s)
{
#ifdef _UNICODE
	std::wstringstream ss(s);
#else
	std::stringstream ss(s);
#endif

	return ss >> value;
}

template <class T>
inline _tstring CStdTpl::ConvertToString(T value)
{
#ifdef _UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	
	ss << value;

	return ss.str();
}

inline _tstring ToString(const double& dValue, const int& nLenDec = -1)
{
	int nLen = nLenDec;
	if (nLen < 0)
	{
		//自动计算保留位数
		double dAbsValue = fabs(dValue);
		double dGap = dAbsValue - std::floor(dAbsValue);
		_tstring strValue = CStdTpl::ConvertToString(dGap);
		strValue = CStdStr::Trim(strValue, _T('0'));
		strValue = CStdStr::Trim(strValue, _T('.'));
		nLen = (int)strValue.length();
	}

	char tmp[256];
	_tstring strFormat = _T("%.") + CStdTpl::ConvertToString(nLen) + _T("lf");

#ifdef _UNICODE
	std::string sFormat = CStdStr::ws2s(strFormat);
	sprintf_s(tmp, sFormat.c_str(), dValue);
	return CStdStr::s2ws(tmp);
#else
	sprintf_s(tmp, strFormat.c_str(), dValue);
	return _tstring(tmp);
#endif
}
