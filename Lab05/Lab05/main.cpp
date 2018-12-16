#include "included.h"
#include "Threshold.h"
#include "Kmean.h"

void main(int argc, char *argv[]) {
	Mat srcImg;
	Mat desImg;
	int type = -1;
	string dstname;

	if (argc >= 4)
	{	
		srcImg = imread(argv[2], CV_LOAD_IMAGE_UNCHANGED);
	}
	else
	{
		cout << "Missing image path!!!" << endl;
		return;
	}

	if (!srcImg.data) {
		cout << "Image scource load fail, please try again" << endl;
		return;
	}

	if (strcmp(argv[1], "static") == 0) { type = 0; }
	if (strcmp(argv[1], "mean") == 0) { type = 1; }
	if (strcmp(argv[1], "median") == 0) { type = 2; }
	if (strcmp(argv[1], "sauvola") == 0) { type = 3; }
	if (strcmp(argv[1], "kmeans") == 0) { type = 4; }

	switch (type)
	{
	case 0:
	{
		int _lowThreshold = stoi(argv[3]);
		int _highThreshold = stoi(argv[4]);

		StaticThreshold* thresholder;
		thresholder = new StaticThreshold(_lowThreshold, _highThreshold);
		thresholder->Apply(srcImg, desImg);
		
		dstname = "StaticThreshold";
		
		delete(thresholder);
	}break;
	case 1:
	{
		int win_size = stoi(argv[3]);
		int C = stoi(argv[4]);

		AverageLocalThreshold* thresholder;
		thresholder = new AverageLocalThreshold(C);

		Size winsize = Size(win_size, win_size);
		
		thresholder->Apply(srcImg, desImg, winsize);
		dstname = "AverageLocalThreshold";
		delete(thresholder);
	}break;
	case 2:
	{
		int win_size = stoi(argv[3]);
		int C = stoi(argv[4]);

		MedianLocalThreshold* thresholder;
		thresholder = new MedianLocalThreshold(C);
		
		Size winsize = Size(win_size, win_size);

		thresholder->Apply(srcImg, desImg, winsize);
		dstname = "MedianLocalThreshold";
		delete(thresholder);
	}break;
	case 3:
	{
		int win_size = stoi(argv[3]); //51
		double k = stod(argv[4]); //0.8
		double r = stod(argv[5]); //110


		SauvolaLocalThreshold* thresholder;
		thresholder = new SauvolaLocalThreshold(k, r);
		
		Size winsize = Size(win_size, win_size);

		thresholder->Apply(srcImg, desImg, winsize);
		dstname = "SauvolaLocalThreshold";
		delete(thresholder);
	}break;
	case 4:
	{
		int K = stoi(argv[3]);

		Kmean* kmeans;
		kmeans = new Kmean(K);

		kmeans->Apply(srcImg, desImg);
		dstname = "Kmean";
		delete(kmeans);
	}break;
	default:
		break;
	}

	imshow("SrcImage", srcImg);
	imshow(dstname, desImg);

	waitKey(0);
	return;
}
