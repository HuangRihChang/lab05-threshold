#pragma once
#include "included.h"

class Kmean
{
	//số cụm K
	int _numClusters;
public:
	/*
	Hàm áp dụng thuật toán Kmeans để phân đoạn ảnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	Hàm trả về
	1: nếu phân đoạn thành công
	0: nếu phân đoạn không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage);

	Kmean();
	Kmean(int k);
	~Kmean();
};

struct clusterPoint
{
	int _y;
	int _x;
	uchar _value;

	clusterPoint(int y, int x, uchar value)
	{
		_y = y;
		_x = x;
		_value = value;
	}
};