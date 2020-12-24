//author:autumoon
//mail:autumoon@vip.qq.com

#pragma once

#include "SimpleIni.h"
#include "StdStrFile.h"
#include <tchar.h>
#include "Defines.h"

typedef struct _config_s 
{
	//All
	int nCateGoryIndex;
	int nImgToolIndex;

	//ClassifyFiles
	bool cf_bRemLastDir;
	bool cf_bPics[4];
	int cf_nSelIndex;
	int cf_nMethod;
	_tstring cf_strLastDir;
	_tstring cf_strCustom;

	//ResizePics
	bool rp_bRemLastDir;
	bool rp_bParameters[5];
	int rp_nSelIndex;
	int rp_nWidth;
	int rp_nHeight;
	int rp_nThreadNum;
	int rp_nJpgQuality;
	_tstring rp_strLastDir;
	_tstring rp_strSuffixs;


	//CloneExif
	int ce_nSelIndex;
	bool ce_bRemLast;
	int ce_nThreadNum;
	_tstring ce_strLast[4];
	_tstring ce_strSuffixs;

	//Convertor
	bool con_bRemLastDir;
	bool con_bPics[4];
	bool con_bSets[3];
	int con_nSelIndex;
	int con_nThreadNum;
	int con_nJpgQuality;
	_tstring con_strLastDir;
	_tstring con_strSuffixs;

	//MergeImgs
	bool mi_bRemLastDir;
	bool mi_bParameters[3];
	int mi_nSelIndex;
	int mi_nWidth;
	int mi_nHeight;
	int mi_nJpgQuality;
	_tstring mi_strLastDir;
	_tstring mi_strSuffixs;

	_config_s()
	{
		//内部初始化
		//All
		nCateGoryIndex = 2;
		nImgToolIndex = 0;

		//ClassifyFiles
		cf_bRemLastDir = true;
		cf_nSelIndex = 0;
		cf_nMethod = 0;
		for (int i = 0; i < 4; ++i)
		{
			cf_bPics[i] = true;
		}
		cf_strLastDir = _T("");
		cf_strCustom = _T("*.*");

		//ResizePics
		rp_bRemLastDir = true;
		rp_strLastDir = _T("");
		rp_strSuffixs = INI_SUFFIXS;
		rp_nSelIndex = 0;
		rp_nWidth = 1920;
		rp_nHeight = 1920;
		rp_nThreadNum = 4;
		rp_nJpgQuality = 90;
		for (int i = 0; i < 5; ++i)
		{
			rp_bParameters[i] = false;
		}
		rp_bParameters[1] = true;

		//CloneExif
		ce_nSelIndex = 0;
		ce_bRemLast = true;
		for (int i = 0; i < 4; ++i)
		{
			ce_strLast[i] = _T("");
		}
		ce_strSuffixs = INI_SUFFIXS;
		ce_nThreadNum = 4;

		//Convertor
		con_bRemLastDir = true;
		con_strLastDir = _T("");
		con_strSuffixs = INI_SUFFIXS;
		for (int i = 0; i < 4; ++i)
		{
			con_bPics[i] = false;
		}
		con_bPics[1] = true;
		con_nSelIndex = 0;
		for (int i = 0; i < 2; ++i)
		{
			con_bSets[i] = true;
		}
		con_bSets[2] = false;
		con_nThreadNum = 4;
		con_nJpgQuality = 90;

		//MergeImgs
		mi_bRemLastDir = true;
		mi_strLastDir = _T("");
		mi_strSuffixs = INI_SUFFIXS;
		mi_nSelIndex = 0;
		mi_nWidth = 1080;
		mi_nHeight = 1080;
		mi_nJpgQuality = 90;
		for (int i = 0; i < 3; ++i)
		{
			mi_bParameters[i] = false;
		}
	}
}config_s;

int ReadIniFile(const _tstring& strIniPath, config_s& _cfg);
int WriteIniFile(const _tstring& strIniPath, const config_s& _cfg);
