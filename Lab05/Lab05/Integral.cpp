#include "Integral.h"

Integral::Integral(int exp) { this->exp = exp; };
Integral::Integral() { this->exp = 1; };
Integral::~Integral() {}

bool Integral::fit(const Mat srcImg)
{
	Mat grayImg;
	//đổi sang ảnh xám
	cvtColor(srcImg, grayImg, cv::COLOR_RGB2GRAY);
	//equalizeHist(grayImg, grayImg);
	if (grayImg.step[1] == 3 || !grayImg.data)
		return false;

	int height = grayImg.rows;// Chiều cao của ảnh gốc
	int width = grayImg.cols;
	int WdithStep = grayImg.step[0];

	this->integralImg = Mat_<int>::zeros(height + 1, width + 1);

	uchar *pData = grayImg.data;

	for (int y = 1; y <= height; y++, pData += WdithStep)
	{
		uchar *pdata_row = pData;
		int SUM_ = 0;
		for (int x = 1; x <= width; x++, pdata_row += 1)
		{
			this->integralImg.at<int>(y, x) = pow(pdata_row[0], (double)exp) + this->integralImg.at<int>(y - 1, x) + SUM_;
			SUM_ += pow(pdata_row[0], (double)exp);
		}
	}
	cout << grayImg << endl;
	cout << "==================================" << endl;
	cout << this->integralImg << endl;
}


int Integral::compute(int r, int c, Size winSize)
{
	if (r >= this->integralImg.rows - 1 || c >= this->integralImg.cols - 1)
		return -1;

	int wstep = winSize.width / 2;
	int hstep = winSize.height / 2;

	int minrow = (r + 1) - hstep - 1;
	int maxrow = (r + 1) + hstep;
	int mincol = (c + 1) - wstep - 1;
	int maxcol = (c + 1) + wstep;

	if (mincol <= 0)
		mincol = 0;
	if (minrow <= 0)
		minrow = 0;
	if (maxcol >= this->integralImg.cols - 1)
		maxcol = this->integralImg.cols - 1;
	if (maxrow >= this->integralImg.rows - 1)
		maxrow = this->integralImg.rows - 1;

	int C = integralImg.at<int>(maxrow, maxcol);
	int B = integralImg.at<int>(minrow, maxcol);
	int A = integralImg.at<int>(minrow, mincol);
	int D = integralImg.at<int>(maxrow, mincol);
	return C - B - D + A;
}