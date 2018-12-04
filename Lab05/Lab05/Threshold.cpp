#pragma once
#include "Threshold.h"
#include "Integral.h"

//==============================================StaticThreshold==============================================
//============================================Ngưỡng Phân Ngưỡng Tĩnh============================================
StaticThreshold::StaticThreshold() {}
StaticThreshold::~StaticThreshold() {}
StaticThreshold::StaticThreshold(int lowThreshold, int highThreshold)
{
	_lowThreshold = lowThreshold;
	_highThreshold = highThreshold;
}
int StaticThreshold::Apply(const Mat& srcImage, Mat &dstImage)
{
	Mat grayImg;
	//đổi sang ảnh xám
	cvtColor(srcImage, grayImg, cv::COLOR_RGB2GRAY);
	//equalizeHist(grayImg, grayImg);

	if (grayImg.step[1] == 3 || !grayImg.data)
		return false;	

	int height = grayImg.rows;// Chiều cao của ảnh gốc
	int width = grayImg.cols;
	int WdithStep = grayImg.step[0];

	dstImage = Mat(height, width, CV_8UC1);//Ảnh xám

	uchar *pData = grayImg.data;
	uchar *pDst = dstImage.data;

	for (int y = 0; y < height; y++, pData += WdithStep, pDst += WdithStep)
	{
		uchar *pDst_row = pDst; //pointer đầu dòng thứ y
		uchar *pData_row = pData;
		for (int x = 0; x < width; x++, pData_row += 1, pDst_row += 1)
			*pDst_row = (((_lowThreshold > *pData_row) && (*pData_row < _highThreshold)) ? 0 : 255);
	}
	return true;
}


//==================================================AverageLocalThreshold===================================================
//============================================Ngưỡng Phân Ngưỡng Trung Bình Cục Bộ============================================
AverageLocalThreshold::AverageLocalThreshold() { _C = 0;}
AverageLocalThreshold::AverageLocalThreshold(int C) { _C = C; }
AverageLocalThreshold::~AverageLocalThreshold() {}
int AverageLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize)
{
	Mat grayImg;
	//đổi sang ảnh xám
	cvtColor(srcImage, grayImg, cv::COLOR_RGB2GRAY);
	//equalizeHist(grayImg, grayImg);
	if (grayImg.step[1] == 3 || !grayImg.data)
		return false;

	int height = grayImg.rows;// Chiều cao của ảnh gốc
	int width = grayImg.cols;
	int WdithStep = grayImg.step[0];

	dstImage = Mat(height, width, CV_8UC1);//Ảnh xám
	//blur(grayImg, dstImage, winSize, Point(-1, -1));

	Integral *integral_ = new Integral();
	integral_->fit(srcImage);
	
	

	uchar *pData = grayImg.data;
	uchar *pDst = dstImage.data;

	for (int y = 0; y < height; y++, pData += WdithStep, pDst += WdithStep)
	{
		uchar *pdst_row = pDst; //pointer đầu dòng thứ y
		uchar *pdata_row = pData;
		for (int x = 0; x < width; x++, pdata_row += 1, pdst_row += 1)
		{
			uchar mean = integral_->compute(y, x, winSize)/(winSize.height*winSize.width);
			*pdst_row = (*pdata_row > (mean - _C)) ? 0 : 255;
		}
	}
	return true;
}


//==================================================MedianLocalThreshold===================================================
//============================================Ngưỡng Phân Ngưỡng Trung Vị Cục Bộ============================================
MedianLocalThreshold::MedianLocalThreshold() { _C = 0; }
MedianLocalThreshold::MedianLocalThreshold(int C) { _C = C; }
MedianLocalThreshold::~MedianLocalThreshold() {}
int MedianLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize)
{
	Mat grayImg;
	//đổi sang ảnh xám
	cvtColor(srcImage, grayImg, cv::COLOR_RGB2GRAY);
	//equalizeHist(grayImg, grayImg);
	if (grayImg.step[1] == 3 || !grayImg.data)
		return false;

	int height = grayImg.rows;// Chiều cao của ảnh gốc
	int width = grayImg.cols;
	int WdithStep = grayImg.step[0];

	dstImage = Mat(height, width, CV_8UC1);//Ảnh xám
	medianBlur(grayImg, dstImage, winSize.height);

	uchar *pData = grayImg.data;
	uchar *pDst = dstImage.data;

	for (int y = 0; y < height; y++, pData += WdithStep, pDst += WdithStep)
	{
		uchar *pdst_row = pDst; //pointer đầu dòng thứ y
		uchar *pdata_row = pData;
		for (int x = 0; x < width; x++, pdata_row += 1, pdst_row += 1)
		{
			uchar* pWin = pdata_row; //pointer tại vị trí y,x
			*pdst_row = ((int)*pdata_row >((int)*pdst_row - (int)_C)) ? 0 : 255;
		}
	}
	return true;
}


//==================================================SauvolaLocalThreshold===================================================
//============================================Ngưỡng Phân Ngưỡng Sauvola Cục Bộ============================================
SauvolaLocalThreshold::SauvolaLocalThreshold() { _k = 0.5; _r = 128; }
SauvolaLocalThreshold::SauvolaLocalThreshold(double k, int r) { _k = k; _r = r; }
SauvolaLocalThreshold::~SauvolaLocalThreshold(){}
int SauvolaLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize)
{
	Mat grayImg;
	//đổi sang ảnh xám
	cvtColor(srcImage, grayImg, cv::COLOR_RGB2GRAY);
	if (grayImg.step[1] == 3 || !grayImg.data)
		return false;

	int height = grayImg.rows;// Chiều cao của ảnh gốc
	int width = grayImg.cols;
	int WdithStep = grayImg.step[0];

	dstImage = Mat(height, width, CV_8UC1);//Ảnh xám

	Integral *integral_mean = new Integral();
	integral_mean->fit(srcImage);
	
	Integral *integral_std = new Integral(2);
	integral_std->fit(srcImage);


	uchar *pData = grayImg.data;
	uchar *pDst = dstImage.data;

	for (int y = 0; y < height; y++, pData += WdithStep, pDst += WdithStep)
	{
		uchar *pdst_row = pDst; //pointer đầu dòng thứ y
		uchar *pdata_row = pData;
		for (int x = 0; x < width; x++, pdata_row += 1, pdst_row += 1)
		{
			int n = (winSize.height*winSize.width);
			uchar mean = integral_mean->compute(y, x, winSize) / n;
			uchar std = (1 / sqrt(n - 1))*sqrt(integral_std->compute(y, x, winSize) - n*pow(mean, 2.0));
			uchar T = mean*(1 + _k*(std / (_r - 1)));
			*pdst_row = (*pdata_row > T) ? 0 : 255;
		}
	}
	return true;
}