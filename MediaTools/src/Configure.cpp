//author:autumoon
//mail:autumoon@vip.qq.com
#include "Configure.h"

inline _tstring VectorToString(const std::vector<_tstring>& vStrings)
{
	_tstring strRes;

	const size_t num = vStrings.size();

	if (num > 0)
	{
		for (size_t i = 0; i < num - 1; ++i)
		{
			const _tstring& strCurItems = vStrings[i];
			strRes += strCurItems + _T('|');
		}

		strRes += vStrings[vStrings.size() - 1];
	}

	return strRes;
}

int ReadIniFile(const _tstring& strIniPath, config_s& _cfg)
{
	bool bRes = CStdFile::IfAccessFile(strIniPath.c_str());

	CSimpleIni Ini;
	Ini.SetUnicode();
	if (bRes)
	{
		//¶ÁÈ¡ÐÅÏ¢
		Ini.LoadFile(strIniPath.c_str());
		_cfg.nCateGoryIndex = Ini.GetLongValue(		INI_SEL_INDEX, INI_CAT_INDEX, _cfg.nImgToolIndex);
		_cfg.nImgToolIndex = Ini.GetLongValue(		INI_SEL_INDEX, INI_SEL_INDEX, _cfg.nImgToolIndex);

		//ClassifyFiles
		_cfg.cf_bRemLastDir = Ini.GetBoolValue(		INI_CLASSIFY_FILES, INI_CF_REMEMBERDIR	,_cfg.cf_bRemLastDir);
		_cfg.cf_strLastDir = Ini.GetValue(			INI_CLASSIFY_FILES, INI_CF_DIR_VALUE	,_cfg.cf_strLastDir.c_str());
		_cfg.cf_nSelIndex = Ini.GetLongValue(		INI_CLASSIFY_FILES, INI_CF_INDEX		,_cfg.cf_nSelIndex);
		_cfg.cf_nMethod = Ini.GetLongValue(			INI_CLASSIFY_FILES, INI_CF_METHOD		,_cfg.cf_nMethod);
		_cfg.cf_bPics[0] = Ini.GetBoolValue(		INI_CLASSIFY_FILES, INI_BMP				,_cfg.cf_bPics[0]);
		_cfg.cf_bPics[1] = Ini.GetBoolValue(		INI_CLASSIFY_FILES, INI_JPG				,_cfg.cf_bPics[1]);
		_cfg.cf_bPics[2] = Ini.GetBoolValue(		INI_CLASSIFY_FILES, INI_PNG				,_cfg.cf_bPics[2]);
		_cfg.cf_bPics[3] = Ini.GetBoolValue(		INI_CLASSIFY_FILES, INI_GIF				,_cfg.cf_bPics[3]);
		_cfg.cf_strCustom = Ini.GetValue(			INI_CLASSIFY_FILES, INI_CF_CUSTOM		,_cfg.cf_strCustom.c_str());

		//ResizePics
		_cfg.rp_bRemLastDir = Ini.GetBoolValue(		INI_RESIZE_PICS, INI_RP_REMEMBERDIR		, _cfg.rp_bRemLastDir);
		_cfg.rp_strLastDir = Ini.GetValue(			INI_RESIZE_PICS, INI_RP_DIR_VALUE		, _cfg.rp_strLastDir.c_str());
		_cfg.rp_strSuffixs = Ini.GetValue(			INI_RESIZE_PICS, INI_RP_PIC_SUFFIXS		, _cfg.rp_strSuffixs.c_str());
		_cfg.rp_nSelIndex = Ini.GetLongValue(		INI_RESIZE_PICS, INI_RP_INDEX			, _cfg.rp_nSelIndex);
		_cfg.rp_nWidth = Ini.GetLongValue(			INI_RESIZE_PICS, INI_RP_WIDTH			, _cfg.rp_nWidth);
		_cfg.rp_nHeight = Ini.GetLongValue(			INI_RESIZE_PICS, INI_RP_HEIGHT			, _cfg.rp_nHeight);
		_cfg.rp_bParameters[0] = Ini.GetBoolValue(	INI_RESIZE_PICS, INI_RP_KEEPWIDTH		, _cfg.rp_bParameters[0]);
		_cfg.rp_bParameters[1] = Ini.GetBoolValue(	INI_RESIZE_PICS, INI_RP_KEEPSCALE		, _cfg.rp_bParameters[1]);
		_cfg.rp_bParameters[2] = Ini.GetBoolValue(	INI_RESIZE_PICS, INI_RP_KEEPHEIGHT		, _cfg.rp_bParameters[2]);
		_cfg.rp_bParameters[3] = Ini.GetBoolValue(	INI_RESIZE_PICS, INI_RP_GRAYSCALE		, _cfg.rp_bParameters[3]);
		_cfg.rp_bParameters[4] = Ini.GetBoolValue(	INI_RESIZE_PICS, INI_RP_IGNORE_MIN		, _cfg.rp_bParameters[4]);
		_cfg.rp_nThreadNum = Ini.GetLongValue(		INI_RESIZE_PICS, INI_RP_THREAD_NUM		, _cfg.rp_nThreadNum);
		_cfg.rp_nJpgQuality = Ini.GetLongValue(		INI_RESIZE_PICS, INI_RP_JPG_QUALITY		, _cfg.rp_nJpgQuality);

		//CloneExif
		_cfg.ce_nSelIndex = Ini.GetLongValue(		INI_CLONE_EXIF, INI_CE_INDEX		, _cfg.ce_nSelIndex);
		_cfg.ce_bRemLast = Ini.GetBoolValue(		INI_CLONE_EXIF, INI_CE_REMEMBERFILE	, _cfg.ce_bRemLast);
		_cfg.ce_strLast[0] = Ini.GetValue(			INI_CLONE_EXIF, INI_CE_FILE_VALUE	, _cfg.ce_strLast[0].c_str());
		_cfg.ce_strLast[1] = Ini.GetValue(			INI_CLONE_EXIF, INI_CE_FILE_DST		, _cfg.ce_strLast[1].c_str());
		_cfg.ce_strLast[2] = Ini.GetValue(			INI_CLONE_EXIF, INI_CE_DIR_VALUE	, _cfg.ce_strLast[2].c_str());
		_cfg.ce_strLast[3] = Ini.GetValue(			INI_CLONE_EXIF, INI_CE_DIR_DST		, _cfg.ce_strLast[3].c_str());
		_cfg.ce_strSuffixs = Ini.GetValue(			INI_CLONE_EXIF, INI_CE_PIC_SUFFIXS	, _cfg.ce_strSuffixs.c_str());
		_cfg.ce_nThreadNum = Ini.GetLongValue(		INI_CLONE_EXIF, INI_CE_THREAD_NUM	, _cfg.ce_nThreadNum);

		//Convertor
		_cfg.con_bRemLastDir = Ini.GetBoolValue(	INI_CONVERTOR, INI_CON_REMEMBERDIR		, _cfg.con_bRemLastDir);
		_cfg.con_strLastDir = Ini.GetValue(			INI_CONVERTOR, INI_CON_DIR_VALUE		, _cfg.con_strLastDir.c_str());
		_cfg.con_strSuffixs = Ini.GetValue(			INI_CONVERTOR, INI_CON_PIC_SUFFIXS		, _cfg.con_strSuffixs.c_str());
		_cfg.con_bPics[0] = Ini.GetBoolValue(		INI_CONVERTOR, INI_BMP					, _cfg.con_bPics[0]);
		_cfg.con_bPics[1] = Ini.GetBoolValue(		INI_CONVERTOR, INI_JPG					, _cfg.con_bPics[1]);
		_cfg.con_bPics[2] = Ini.GetBoolValue(		INI_CONVERTOR, INI_PNG					, _cfg.con_bPics[2]);
		_cfg.con_bPics[3] = Ini.GetBoolValue(		INI_CONVERTOR, INI_GIF					, _cfg.con_bPics[3]);
		_cfg.con_nSelIndex = Ini.GetLongValue(		INI_CONVERTOR, INI_CON_INDEX			, _cfg.con_nSelIndex);
		_cfg.con_bSets[0] = Ini.GetBoolValue(		INI_CONVERTOR, INI_CON_KEEP_MODIFYTIME	, _cfg.con_bSets[0]);
		_cfg.con_bSets[1] = Ini.GetBoolValue(		INI_CONVERTOR, INI_CON_SET_PHOTOTIME	, _cfg.con_bSets[1]);
		_cfg.con_bSets[2] = Ini.GetBoolValue(		INI_CONVERTOR, INI_CON_SET_GRAYSCALE	, _cfg.con_bSets[2]);
		_cfg.con_nThreadNum = Ini.GetLongValue(		INI_CONVERTOR, INI_CON_THREAD_NUM		, _cfg.con_nThreadNum);
		_cfg.con_nJpgQuality = Ini.GetLongValue(	INI_CONVERTOR, INI_CON_JPG_QUALITY		, _cfg.con_nJpgQuality);

		//MergeImgs
		_cfg.mi_bRemLastDir = Ini.GetBoolValue(		INI_MERGE_IMGS, INI_MI_REMEMBERDIR		, _cfg.mi_bRemLastDir);
		_cfg.mi_strLastDir = Ini.GetValue(			INI_MERGE_IMGS, INI_MI_DIR_VALUE		, _cfg.mi_strLastDir.c_str());
		_cfg.mi_strSuffixs = Ini.GetValue(			INI_MERGE_IMGS, INI_MI_PIC_SUFFIXS		, _cfg.mi_strSuffixs.c_str());
		_cfg.mi_nSelIndex = Ini.GetLongValue(		INI_MERGE_IMGS, INI_MI_INDEX			, _cfg.mi_nSelIndex);
		_cfg.mi_nWidth = Ini.GetLongValue(			INI_MERGE_IMGS, INI_MI_WIDTH			, _cfg.mi_nWidth);
		_cfg.mi_nHeight = Ini.GetLongValue(			INI_MERGE_IMGS, INI_MI_HEIGHT			, _cfg.mi_nHeight);
		_cfg.mi_bParameters[0] = Ini.GetBoolValue(	INI_MERGE_IMGS, INI_MI_KEEPWIDTH		, _cfg.mi_bParameters[0]);
		_cfg.mi_bParameters[1] = Ini.GetBoolValue(	INI_MERGE_IMGS, INI_MI_KEEPHEIGHT		, _cfg.mi_bParameters[1]);
		_cfg.mi_bParameters[2] = Ini.GetBoolValue(	INI_MERGE_IMGS, INI_MI_GRAYSCALE		, _cfg.mi_bParameters[2]);
		_cfg.mi_nJpgQuality = Ini.GetLongValue(		INI_MERGE_IMGS, INI_MI_JPG_QUALITY		, _cfg.mi_nJpgQuality);
	}
	else
	{
		WriteIniFile(strIniPath, _cfg);
	}
	Ini.Reset();

	return 0;
}

int WriteIniFile(const _tstring& strIniPath, const config_s& _cfg)
{
	CSimpleIni Ini;
	Ini.SetUnicode();

	Ini.SetLongValue(	INI_SEL_INDEX, INI_CAT_INDEX, _cfg.nCateGoryIndex);
	Ini.SetLongValue(	INI_SEL_INDEX, INI_SEL_INDEX, _cfg.nImgToolIndex);

	//ClassifyFiles
	Ini.SetBoolValue(	INI_CLASSIFY_FILES, INI_CF_REMEMBERDIR	, _cfg.cf_bRemLastDir);
	Ini.SetValue(		INI_CLASSIFY_FILES, INI_CF_DIR_VALUE	, _cfg.cf_strLastDir.c_str());
	Ini.SetLongValue(	INI_CLASSIFY_FILES, INI_CF_INDEX		, _cfg.cf_nSelIndex);
	Ini.SetLongValue(	INI_CLASSIFY_FILES, INI_CF_METHOD		, _cfg.cf_nMethod);
	Ini.SetBoolValue(	INI_CLASSIFY_FILES, INI_BMP				, _cfg.cf_bPics[0]);
	Ini.SetBoolValue(	INI_CLASSIFY_FILES, INI_JPG				, _cfg.cf_bPics[1]);
	Ini.SetBoolValue(	INI_CLASSIFY_FILES, INI_PNG				, _cfg.cf_bPics[2]);
	Ini.SetBoolValue(	INI_CLASSIFY_FILES, INI_GIF				, _cfg.cf_bPics[3]);
	Ini.SetValue(		INI_CLASSIFY_FILES, INI_CF_CUSTOM		, _cfg.cf_strCustom.c_str());

	//ResizePics
	Ini.SetBoolValue(	INI_RESIZE_PICS, INI_RP_REMEMBERDIR		, _cfg.rp_bRemLastDir);
	Ini.SetValue(		INI_RESIZE_PICS, INI_RP_DIR_VALUE		, _cfg.rp_strLastDir.c_str());
	Ini.SetValue(		INI_RESIZE_PICS, INI_RP_PIC_SUFFIXS		, _cfg.rp_strSuffixs.c_str());
	Ini.SetLongValue(	INI_RESIZE_PICS, INI_RP_INDEX			, _cfg.rp_nSelIndex);
	Ini.SetLongValue(	INI_RESIZE_PICS, INI_RP_WIDTH			, _cfg.rp_nWidth);
	Ini.SetLongValue(	INI_RESIZE_PICS, INI_RP_HEIGHT			, _cfg.rp_nHeight);
	Ini.SetBoolValue(	INI_RESIZE_PICS, INI_RP_KEEPWIDTH		, _cfg.rp_bParameters[0]);
	Ini.SetBoolValue(	INI_RESIZE_PICS, INI_RP_KEEPSCALE		, _cfg.rp_bParameters[1]);
	Ini.SetBoolValue(	INI_RESIZE_PICS, INI_RP_KEEPHEIGHT		, _cfg.rp_bParameters[2]);
	Ini.SetBoolValue(	INI_RESIZE_PICS, INI_RP_GRAYSCALE		, _cfg.rp_bParameters[3]);
	Ini.SetBoolValue(	INI_RESIZE_PICS, INI_RP_IGNORE_MIN		, _cfg.rp_bParameters[4]);
	Ini.SetLongValue(	INI_RESIZE_PICS, INI_RP_THREAD_NUM		, _cfg.rp_nThreadNum);
	Ini.SetLongValue(	INI_RESIZE_PICS, INI_RP_JPG_QUALITY		, _cfg.rp_nJpgQuality);

	//CloneExif
	Ini.SetLongValue(	INI_CLONE_EXIF, INI_CE_INDEX		, _cfg.ce_nSelIndex);
	Ini.SetBoolValue(	INI_CLONE_EXIF, INI_CE_REMEMBERFILE	, _cfg.ce_bRemLast);
	Ini.SetValue(		INI_CLONE_EXIF, INI_CE_FILE_VALUE	, _cfg.ce_strLast[0].c_str());
	Ini.SetValue(		INI_CLONE_EXIF, INI_CE_FILE_DST		, _cfg.ce_strLast[1].c_str());
	Ini.SetValue(		INI_CLONE_EXIF, INI_CE_DIR_VALUE	, _cfg.ce_strLast[2].c_str());
	Ini.SetValue(		INI_CLONE_EXIF, INI_CE_DIR_DST		, _cfg.ce_strLast[3].c_str());
	Ini.SetValue(		INI_CLONE_EXIF, INI_CE_PIC_SUFFIXS	, _cfg.ce_strSuffixs.c_str());
	Ini.SetLongValue(	INI_CLONE_EXIF, INI_CE_THREAD_NUM	, _cfg.ce_nThreadNum);

	//Convertor
	Ini.SetBoolValue(	INI_CONVERTOR, INI_CON_REMEMBERDIR		, _cfg.con_bRemLastDir);
	Ini.SetValue(		INI_CONVERTOR, INI_CON_DIR_VALUE		, _cfg.con_strLastDir.c_str());
	Ini.SetValue(		INI_CONVERTOR, INI_CON_PIC_SUFFIXS		, _cfg.con_strSuffixs.c_str());
	Ini.SetBoolValue(	INI_CONVERTOR, INI_BMP					, _cfg.con_bPics[0]);
	Ini.SetBoolValue(	INI_CONVERTOR, INI_JPG					, _cfg.con_bPics[1]);
	Ini.SetBoolValue(	INI_CONVERTOR, INI_PNG					, _cfg.con_bPics[2]);
	Ini.SetBoolValue(	INI_CONVERTOR, INI_GIF					, _cfg.con_bPics[3]);
	Ini.SetLongValue(	INI_CONVERTOR, INI_CON_INDEX			, _cfg.con_nSelIndex);
	Ini.SetBoolValue(	INI_CONVERTOR, INI_CON_KEEP_MODIFYTIME	, _cfg.con_bSets[0]);
	Ini.SetBoolValue(	INI_CONVERTOR, INI_CON_SET_PHOTOTIME	, _cfg.con_bSets[1]);
	Ini.SetBoolValue(	INI_CONVERTOR, INI_CON_SET_GRAYSCALE	, _cfg.con_bSets[2]);
	Ini.SetLongValue(	INI_CONVERTOR, INI_CON_THREAD_NUM		, _cfg.con_nThreadNum);
	Ini.SetLongValue(	INI_CONVERTOR, INI_CON_JPG_QUALITY		, _cfg.con_nJpgQuality);

	//MergeImgs
	Ini.SetBoolValue(	INI_MERGE_IMGS, INI_MI_REMEMBERDIR		, _cfg.mi_bRemLastDir);
	Ini.SetValue(		INI_MERGE_IMGS, INI_MI_DIR_VALUE		, _cfg.mi_strLastDir.c_str());
	Ini.SetValue(		INI_MERGE_IMGS, INI_MI_PIC_SUFFIXS		, _cfg.mi_strSuffixs.c_str());
	Ini.SetLongValue(	INI_MERGE_IMGS, INI_MI_INDEX			, _cfg.mi_nSelIndex);
	Ini.SetLongValue(	INI_MERGE_IMGS, INI_MI_WIDTH			, _cfg.mi_nWidth);
	Ini.SetLongValue(	INI_MERGE_IMGS, INI_MI_HEIGHT			, _cfg.mi_nHeight);
	Ini.SetBoolValue(	INI_MERGE_IMGS, INI_MI_KEEPWIDTH		, _cfg.mi_bParameters[0]);
	Ini.SetBoolValue(	INI_MERGE_IMGS, INI_MI_KEEPHEIGHT		, _cfg.mi_bParameters[1]);
	Ini.SetBoolValue(	INI_MERGE_IMGS, INI_MI_GRAYSCALE		, _cfg.mi_bParameters[2]);
	Ini.SetLongValue(	INI_MERGE_IMGS, INI_MI_JPG_QUALITY		, _cfg.mi_nJpgQuality);

	Ini.SaveFile(strIniPath.c_str());
	Ini.Reset();

	return 0;
}
