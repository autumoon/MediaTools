#include "cxImageUser.h"
#include "..\src\StdStrFile.h"

int CxImageUser::m_nJpgQuality = 90;

bool CxImageUser::GetExifTime(_tstring fileName, _tstring& strTime)
{
	EXIFINFO* pExifInfo;

	CxImage  image;

	_tstring stSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(fileName, false));

	if (stSuffix == _T("jpg") || stSuffix == _T("jpeg"))
	{
		image.Load(fileName.c_str(), CXIMAGE_FORMAT_JPG); 
	}
	else if (stSuffix == _T("tif") || stSuffix == _T("tiff"))
	{
		image.Load(fileName.c_str(), CXIMAGE_FORMAT_TIF); 
	}
	else
	{
		return false;
	}

	if (image.IsValid())
	{
		pExifInfo = image.GetExifInfo();

		if (pExifInfo->IsExif)
		{
			//��ȡʱ�����Ϣ
			if (pExifInfo->DateTime[0])
			{
				strTime = CStdStr::Trim(CStdStr::s2ws(pExifInfo->DateTime));
				//���Ϊ��Ҳ���ؼ�
				return strTime.length() != 0;
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}
	}

	return true;    
}

bool CxImageUser::MergeImages(const std::vector<_tstring> vImgsToMerge, const _tstring& strDstFile, const bool& bVertically/* = false*/,
	const int& nLimiteLength /*= -1*/, const bool& bGrayScale /*= false*/, CProgressInterface* ppi /*= nullptr*/)
{
	CxImage img1,img2;
	ENUM_CXIMAGE_FORMATS format2;
	ENUM_CXIMAGE_FORMATS formatDst = GetFormatBySuffix(strDstFile);
	const size_t nFileNum = vImgsToMerge.size();
	if (nFileNum  == 0 || formatDst == CXIMAGE_FORMAT_UNKNOWN)
	{
		return false;
	}
	else
	{
		//Ϊ�˽�����������⣬��һ��ͳһ����Ȼ���ȡ
		_tstring strFirstFile = CStdStr::ReplaceSuffix(strDstFile, _T(".tif"));
		if (!pic2pic(vImgsToMerge[0], strFirstFile))
		{
			return false;
		}
		img1.Load(strFirstFile.c_str());
		img1.RotateExif();
		//ɾ����ʱ�ļ�
		std::string sFirstFile = CStdStr::ws2s(strFirstFile);
		remove(sFirstFile.c_str());

		//�趨��ʼ��Ȼ��߸߶�
		if (nLimiteLength > 0)
		{
			if (bVertically)
			{
				//�趨���
				int nNewWidth = nLimiteLength;
				int nNewHeight = GetPropLength(img1.GetWidth(), img1.GetHeight(), nLimiteLength);

				img1.Resample(nNewWidth, nNewHeight);
			}
			else
			{
				//�趨�߶�
				int nNewWidth = GetPropLength(img1.GetHeight(), img1.GetWidth(), nLimiteLength);
				int nNewHeight = nLimiteLength;
				img1.Resample(nNewWidth, nNewHeight);
			}
		}

		//��ʼ�ϲ�
		for (size_t i = 1; i < nFileNum; ++i)
		{
			format2 = GetFormatBySuffix(vImgsToMerge[i]);

			if (format2 == CXIMAGE_FORMAT_UNKNOWN)
			{
				continue;
			}

			img2.Load(vImgsToMerge[i].c_str());
			img2.RotateExif();
			if (format2 == CXIMAGE_FORMAT_ICO)
			{
				int nBigFrameIndex = 0, nBigWidth = 0, nBigHeight = 0;
				if (!GetIcoInfo(vImgsToMerge[i], nBigFrameIndex, nBigWidth, nBigHeight))
				{
					continue;
				}
				img2.SetFrame(nBigFrameIndex);
				img2.Load(vImgsToMerge[i].c_str());
				img2.RotateExif();
			}
			CxImage imgTmp;
			if (bVertically)
			{
				if (!MergeImagesVertially(&img1, &img2, &imgTmp))
				{
					continue;
				}
			}
			else
			{
				if (!MergeImagesHorizontally(&img1, &img2, &imgTmp))
				{
					continue;
				}
			}

			img1 = imgTmp;
			if (ppi)
			{
				ppi->SetProgressValue(i ,nFileNum);
			}
		}
	}

	if (formatDst == CXIMAGE_FORMAT_GIF)
	{
		img1.DecreaseBpp(8, true);
	}

	if (bGrayScale)
	{
		img1.GrayScale();
	}

	img1.Save(strDstFile.c_str(), formatDst);

	return true;
}

bool CxImageUser::MergeImagesHorizontally(CxImage* img1, CxImage* img2, CxImage* imgDst)
{
	if (img1 == nullptr || img2 == nullptr || imgDst == nullptr)
	{
		return false;
	}

	int h1=img1->GetHeight();
	int w1=img1->GetWidth();
	int h2=img2->GetHeight();
	int w2=img2->GetWidth();

	int h3 = h1;

	int bpp1=img1->GetBpp();
	int bpp2=img2->GetBpp();

	if (bpp1 != bpp2)
	{
		if (bpp1 > bpp2)
		{
			img2->IncreaseBpp(bpp1);
		}
		else
		{
			img2->DecreaseBpp(bpp1, false);
		}
	}

	if (h1 > 0 && h2 > 0)
	{
		//�ز������̶��߶ȣ����ֱ���
		int nNewWidth = GetPropLength(h2, w2, h1);
		int nNewHeight = h1;

		img2->Resample(nNewWidth, nNewHeight);
		//�����µĿ��
		w2 = nNewWidth;
	}
	int w3=w1+w2;
	imgDst->Create(w3,h3,bpp1);
	imgDst->MixFrom(*img1,0,0);
	imgDst->MixFrom(*img2,w1,0);

	return true;
}

bool CxImageUser::MergeImagesVertially(CxImage* img1, CxImage* img2, CxImage* imgDst)
{
	if (img1 == nullptr || img2 == nullptr || imgDst == nullptr)
	{
		return false;
	}

	int h1=img1->GetHeight();
	int w1=img1->GetWidth();
	int h2=img2->GetHeight();
	int w2=img2->GetWidth();

	int w3 = w1;

	int bpp1=img1->GetBpp();
	int bpp2=img2->GetBpp();

	if (bpp1 != bpp2)
	{
		if (bpp1 > bpp2)
		{
			img2->IncreaseBpp(bpp1);
		}
		else
		{
			img2->DecreaseBpp(bpp1, false);
		}
	}

	if (h1 > 0 && h2 > 0)
	{
		//�ز������̶���ȣ����ֱ���
		int nNewWidth = w1;
		int nNewHeight = GetPropLength(w2, h2, w1);

		img2->Resample(nNewWidth, nNewHeight);
		//�����µĿ��
		h2 = nNewHeight;
	}
	int h3=h1+h2;
	imgDst->Create(w3,h3,bpp1);
	imgDst->MixFrom(*img1,0,h2);
	imgDst->MixFrom(*img2,0,0);

	return true;
}

bool CxImageUser::ico2pic(const _tstring& strSrcFile, const _tstring& strDstFile, const int& nWidth /*= -1*/, const int& nHeight /*= -1*/, const bool& bGrayScale /*= false*/)
{
	ENUM_CXIMAGE_FORMATS dstFormat = GetFormatBySuffix(strDstFile);

	CxImage  image; 
	image.Load(strSrcFile.c_str(), CXIMAGE_FORMAT_ICO); 
	image.RotateExif();
	if (image.IsValid())
	{
		int nBigFrameIndex = 0, nBigWidth = 0, nBigHeight = 0;
		if (!GetIcoInfo(strSrcFile, nBigFrameIndex, nBigWidth, nBigHeight))
		{
			return false;
		}

		image.SetFrame(nBigFrameIndex);
		image.Load(strSrcFile.c_str());
		image.RotateExif();

		if(!image.IsGrayScale() && dstFormat != CXIMAGE_FORMAT_GIF)
			image.IncreaseBpp(24); 

		if (dstFormat == CXIMAGE_FORMAT_JPG)
		{
			//�����jpg������ѹ������
			image.SetJpegQuality(m_nJpgQuality);
		}

		if (dstFormat == CXIMAGE_FORMAT_GIF)
		{
			image.DecreaseBpp(8, true);
		}

		if (nWidth != -1 && nHeight != -1 && nWidth > 0 && nHeight > 0)
		{
			image.Resample(nWidth, nHeight);
		}

		if (bGrayScale)
		{
			image.GrayScale();
		}

		image.Save(strDstFile.c_str(), dstFormat); 
	}

	return true;
}

bool CxImageUser::GetIcoInfo(const _tstring& strSrcFile, int& nBigFrameIndex, int& nBigWidth, int& nBigHeight)
{
	CxImage  image; 
	image.Load(strSrcFile.c_str(), CXIMAGE_FORMAT_ICO); 

	if (image.IsValid())
	{ 
		int nPageCount = image.GetNumFrames();//�õ�ico�ļ���ҳ��
		nBigFrameIndex = 0;
		nBigWidth = -1;
		nBigHeight = -1;
		for(int i = 0; i < nPageCount; i++)
		{
			if ( 0 != i)
			{
				image.SetFrame(i);			//���õ�ǰ֡Ϊi���ͼ��
				image.Load(strSrcFile.c_str());
				int w_tmp = image.GetWidth();
				int h_tmp = image.GetHeight();
				if (w_tmp >= nBigWidth && h_tmp >= h_tmp)
				{
					nBigFrameIndex = i;
					nBigWidth = w_tmp;
					nBigHeight = h_tmp;
				}
			}
			else
			{
				image.SetFrame(i);			//���õ�ǰ֡Ϊi���ͼ��
				image.Load(strSrcFile.c_str());
				nBigWidth = image.GetWidth();
				nBigHeight = image.GetHeight();
			}
		}

		return true;
	}

	return false;
}

bool CxImageUser::pic2pic(const _tstring& strSrcFile, const _tstring& strDstFile, const int& nWidth /*= -1*/, const int& nHeight /*= -1*/, const bool& bGrayScale /*= false*/)
{
	//�����ĸ�ʽ����ת��
	ENUM_CXIMAGE_FORMATS srcFormat = GetFormatBySuffix(strSrcFile);
	ENUM_CXIMAGE_FORMATS dstFormat = GetFormatBySuffix(strDstFile);

	//gif�����ߴ磬Ϊ�˱��ֶ�����Ҫ���⴦��
	if (srcFormat == CXIMAGE_FORMAT_GIF && dstFormat == CXIMAGE_FORMAT_GIF)
	{
		return ResizeGif(strSrcFile, strDstFile, nWidth, nHeight, bGrayScale);
	}

	//ico��ʽת������Ϊ��ߴ��ԭ����Ҫ���⴦��
	if (srcFormat == CXIMAGE_FORMAT_ICO)
	{
		return ico2pic(strSrcFile, strDstFile, nWidth, nHeight, bGrayScale);
	}

	CxImage  image; 
	image.Load(strSrcFile.c_str(), srcFormat); 
	if (srcFormat != dstFormat)
	{
		image.RotateExif();
	}
	if (image.IsValid())
	{ 
		if(!image.IsGrayScale() && dstFormat != CXIMAGE_FORMAT_GIF)
			image.IncreaseBpp(24); 

		if (dstFormat == CXIMAGE_FORMAT_JPG)
		{
			//�����jpg������ѹ������
			image.SetJpegQuality(m_nJpgQuality);
		}

		if (dstFormat == CXIMAGE_FORMAT_GIF)
		{
			image.DecreaseBpp(8, true);
		}

		if (nWidth > 0 && nHeight > 0)
		{
			image.Resample(nWidth, nHeight);
		}

		if (bGrayScale)
		{
			image.GrayScale();
		}

		bool bRes = image.Save(strDstFile.c_str(), dstFormat); 

		return bRes;
	}

	return true;
}

bool CxImageUser::ResizeGif(const _tstring& strSrcFile, const _tstring& strDstFile, const int& nWidth, const int& nHeight,
	const bool& bGrayScale /*= false*/, CProgressInterface* ppi /*= nullptr*/)
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_ENCODE && CXIMAGE_SUPPORT_GIF
	CxImage img;
	img.Load(strSrcFile.c_str(), CXIMAGE_FORMAT_GIF);

	if (!img.IsValid())
	{
		return false;
	}

	//������ֻ�ܱ�����һ֡
	//img.Resample(nWidth, nHeight);
	//img.Save(strDstFile.c_str(), CXIMAGE_FORMAT_GIF);

	int iNumFrames = img.GetNumFrames();
	CxImage** imgSave = new CxImage*[iNumFrames];

	for (int i = 0; i < iNumFrames; i++)
	{
		CxImage* newImage = new CxImage();
		newImage->SetFrame(i);
		newImage->Load(strSrcFile.c_str(), CXIMAGE_FORMAT_GIF);
		newImage->Resample(nWidth, nHeight);
		if (0 == newImage->GetNumColors())
		{
			imgSave[i]->DecreaseBpp(8, true);
		}

		if (bGrayScale)
		{
			newImage->GrayScale();
		}

		imgSave[i] = newImage;
		if (ppi)
		{
			ppi->SetProgressValue(i, iNumFrames);
		}
	}

	CxIOFile hFile;
	std::string Method = "wb";
	_tstring  stempmd = CStdStr::s2ws(Method);
	LPCWSTR wMethod = stempmd.c_str();
	bool BFlag = hFile.Open(strDstFile.c_str(), wMethod);

	CxImageGIF multiimage;

	multiimage.SetLoops(-1);
	multiimage.SetFrameDelay(img.GetFrameDelay());
	multiimage.SetDisposalMethod(img.GetDisposalMethod());
	multiimage.Encode(&hFile, imgSave, iNumFrames, false, false);

	hFile.Close();

	//�ͷ���Դ
	for (int i = 0; i < iNumFrames; ++i)
	{
		if (imgSave[i])
		{
			delete imgSave[i];
			imgSave[i] = nullptr;
		}
	}
	if (imgSave)
	{
		delete[] imgSave;
		imgSave = nullptr;
	}

#endif

	return true;
}

inline ENUM_CXIMAGE_FORMATS CxImageUser::GetFormatBySuffix(const _tstring& strFileName)
{
	const _tstring strSrcSuffix = CStdStr::ToUpperLower(CStdStr::GetSuffixOfFile(strFileName, false));

	return (ENUM_CXIMAGE_FORMATS)CxImage::GetTypeIdFromName(strSrcSuffix.c_str());
}

int CxImageUser::GetPropLength(const int& nOldLength1, const int& nOldLength2, const int& nNewLength1)
{
	//�����
	double dPropScale = (double)nOldLength2 / nOldLength1;

	int nNewLength2 = int(nNewLength1 * dPropScale + 0.5);

	//�õ�ż��
	nNewLength2 = nNewLength2 % 2 == 0 ? nNewLength2 : nNewLength2 + 1;

	//����0
	if (nNewLength2 <= 0)
	{
		nNewLength2 = 1;
	}

	return nNewLength2;
}

