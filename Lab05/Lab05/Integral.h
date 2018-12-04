#pragma once
#include "included.h"


class Integral
{
	Mat_<int> integralImg;
	int exp;
public:
	Integral(int exp);
	Integral();
	~Integral();
	bool fit(const Mat srcImg);
	
	int compute(int r, int c, Size winSize);
};