#include "included.h"
#include "Integral.h"
#include "Threshold.h"

int main(int argc, char *argv[]) {
	Mat srcImg;
	Mat desImg;

	int type = 3;

	int _lowThreshold = 80;
	int _highThreshold = 150;

	string filePath = "./images/debug.jpg";
	string dstname;

	//đọc ảnh lên
	srcImg = imread(filePath, CV_LOAD_IMAGE_UNCHANGED);


	Integral *test;
	test = new Integral(1);
	test->fit(srcImg);
	cout << test->compute(5, 5, Size(3, 3)) << endl;

	switch (type)
	{
	case 0:
	{
		StaticThreshold* thresholder;
		thresholder = new StaticThreshold(_lowThreshold, _highThreshold);
		thresholder->Apply(srcImg, desImg);
		dstname = "StaticThreshold";
		delete(thresholder);
	}break;
	case 1:
	{
		AverageLocalThreshold* thresholder;
		thresholder = new AverageLocalThreshold();
		Size winsize = Size(5, 5);
		thresholder->Apply(srcImg, desImg, winsize);
		dstname = "AverageLocalThreshold";
		delete(thresholder);
	}break;
	case 2:
	{
		MedianLocalThreshold* thresholder;
		thresholder = new MedianLocalThreshold();
		Size winsize = Size(5, 5);
		thresholder->Apply(srcImg, desImg, winsize);
		dstname = "MedianLocalThreshold";
		delete(thresholder);
	}break;

	default:
		break;
	}
/*
	imshow("SrcImage", srcImg);
	imshow(dstname, desImg);*/
	waitKey(0);
	return 1;
}
