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

	float E = 1;

	while (E >= CONVERGE)
	{
		uchar *pData = grayImg.data;
		
		//after a loop we clean all the point in cluster to save memories
		//we mainpoint is after some loop enought to converge, the loop function will stop 
		//and we not clean the last clusters
		clusters.empty();

		//for every pixel in image we will compute distance to every mean represented to clusters
		//and then for a pixel, it will join the cluster have the minimum distance 
		//from that pixel to cluster's mean
		for (int y = 0; y < height; y++, pData += WdithStep)
		{
			uchar *pdata_row = pData;
			for (int x = 0; x < width; x++, pdata_row += 1)
			{
				vector<int> distances;
				distances.empty();
				//compute distances from a pixel to every mean
				for (int i = 0; i < _numClusters; i++)
					distances.push_back(abs(*pdata_row - means[i]));

				//find the nearest mean and join that mean
				int minIndex = min_element(distances.begin(), distances.end()) - distances.begin();
				clusters[minIndex].push_back(clusterPoint(y, x, *pdata_row));
			}
		}

		//we save current means before re-compute means with new clusters
		old_means = means;

		//it's Abs Error
		E = 0;

		//after that we re-compute all the mean
		for (int i = 0; i < _numClusters; i++)
		{
			int SUM_ = 0;
			if (clusters[i].size() > 0)
			{
				//re-compute means for all the mean with new cluster
				for (int j = 0; j < clusters[i].size(); j++)
					SUM_ += (int)clusters[i][j]._value;
				means[i] = (int)(SUM_ / clusters[i].size());
			}

			//compute the error
			E += abs(means[i] - old_means[i]);
		}
	} //we repeate until Error change very small
	//it's mean all the mean have change not thing

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