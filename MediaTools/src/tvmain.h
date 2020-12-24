//author:autumoon
//mail:autumoon@vip.qq.com

#pragma once

#ifdef DLLProvider

#define DLL_API __declspec(dllexport)
#define DLL_API_C extern "C" __declspec(dllexport)

#else

#define DLL_API __declspec(dllimport)

#ifdef __cplusplus
#define DLL_API_C extern "C" __declspec(dllimport)
#else
#define DLL_API_C __declspec(dllimport)

#endif // __cplusplus

#endif // DLLProvider

#ifdef __cplusplus
#include <vector>
//进度接口基类
#include "ProgressInterface.h"	//使用dll时，不用进度条可以注释这个头文件
#endif // __cplusplus

#ifdef _UNICODE

#ifndef _tstring
#define _tstring std::wstring
#endif // _tstring

#else

#ifndef _tstring
#define _tstring std::string
#endif // _tstring

#endif

#ifndef DLLProvider

#ifdef _DEBUG
#pragma comment(lib, "src\\threadvectord.lib")
#else
#pragma comment(lib, "src\\threadvector.lib")
#endif

#endif // DLLProvider

#ifdef __cplusplus

//本函数只适用于c++方式的调用，处理_tstring的vector
DLL_API int ProcessVector(const std::vector<_tstring>& vInputs, const std::vector<_tstring>& vOutputs,
	int(*pFun)(const _tstring&, const _tstring&, const size_t&), CProgressInterface* pProgress = nullptr, const int& nThreadNum = 4);

//本函数只适用于c++方式的调用，处理const void*的vector
DLL_API int ProcessVector(const std::vector<const void*>& vInputs, const std::vector<const void*>& vOutputs,
	int(*pFun)(const void*, const void*, const size_t&), CProgressInterface* pProgress = nullptr, const int& nThreadNum = 4);

#endif // __cplusplus

//本函数为c语言接口，处理二维数组
DLL_API_C int ProcessVector_C(void** vInputs, void** vOutputs, const int nNum, int(*pFun)(const void*, const void*, const int), const int nThreadNum);