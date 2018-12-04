#include "included.h"
#include "Threshold.h"

int main(int argc, char *argv[]) {
	Mat srcImg;
	Mat desImg;

	int type = 3;

	int _lowThreshold = 80;
	int _highThreshold = 150;

	string filePath = "./images/cafe.jpg";
	string dstname;

	//đọc ảnh lên
	srcImg = imread(filePath, CV_LOAD_IMAGE_UNCHANGED);
	cout << "READ OK";

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
		Size winsize = Size(9, 9);
		thresholder->Apply(srcImg, desImg, winsize);
		dstname = "AverageLocalThreshold";
		delete(thresholder);
	}break;
	case 2:
	{
		MedianLocalThreshold* thresholder;
		thresholder = new MedianLocalThreshold();
		Size winsize = Size(9, 9);
		thresholder->Apply(srcImg, desImg, winsize);
		dstname = "MedianLocalThreshold";
		delete(thresholder);
	}break;
	case 3:
	{
		SauvolaLocalThreshold* thresholder;
		thresholder = new SauvolaLocalThreshold(0.2, 128);
		Size winsize = Size(5, 5);
		thresholder->Apply(srcImg, desImg, winsize);
		dstname = "SauvolaLocalThreshold";
		delete(thresholder);
	}break;

	default:
		break;
	}

	imshow("SrcImage", srcImg);
	imshow(dstname, desImg);
	waitKey(0);
	return 1;
}
