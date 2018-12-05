#pragma once
#include "Kmean.h"

Kmean::Kmean() { this->_numClusters = 2; }
Kmean::Kmean(int k) { this->_numClusters = k; }
Kmean::~Kmean() {}
int Kmean::Apply(const Mat& srcImage, Mat &dstImage)
{
	Mat grayImg;
	//đổi sang ảnh xám
	cvtColor(srcImage, grayImg, cv::COLOR_RGB2GRAY);
	if (grayImg.step[1] == 3 || !grayImg.data)
		return false;

	int height = grayImg.rows;// Chiều cao của ảnh gốc
	int width = grayImg.cols;
	int WdithStep = grayImg.step[0];

	/* initialize random means*/
	srand(time(NULL));
	vector<int> means;
	
	for (int i = 0; i < _numClusters; i++)
		means.push_back(rand() % 255 + 1);
	
	vector<int> old_means;
	vector<vector<clusterPoint>> clusters(_numClusters);

	/*compute distances to means*/
	float E = 1;

	while (E >= CONVERGE)
	{
		uchar *pData = grayImg.data;
		for (int y = 0; y < height; y++, pData += WdithStep)
		{
			uchar *pdata_row = pData;
			for (int x = 0; x < width; x++, pdata_row += 1)
			{
				vector<int> distances;
				distances.empty();

				for (int i = 0; i < _numClusters; i++)
					distances.push_back(abs(*pdata_row - means[i]));

				int minIndex = min_element(distances.begin(), distances.end()) - distances.begin();
				clusters[minIndex].push_back(clusterPoint(y, x, *pdata_row));
			}
		}

		old_means = means;

		E = 0;
		for (int i = 0; i < _numClusters; i++)
		{
			int SUM_ = 0;
			if (clusters[i].size() > 0)
			{
				for (int j = 0; j < clusters[i].size(); j++)
					SUM_ += (int)clusters[i][j]._value;
				means[i] = (int)(SUM_ / clusters[i].size());
			}
			E += abs(means[i] - old_means[i]);
		}
	}
	cout << "clustering OK" << endl;

	dstImage = Mat(height, width, CV_8UC1);//Ảnh xám

	for (int i = 0; i < _numClusters; i++)
	{
		for (int j = 0; j < clusters[i].size(); j++)
		{
			uchar *pDst = dstImage.data;
			for (int y = 0; y < clusters[i][j]._y; y++, pDst += WdithStep) {}
			for (int x = 0; x < clusters[i][j]._x; x++, pDst += 1) {}
			*pDst = (uchar)means[i];
		}
	}
	return true;
}