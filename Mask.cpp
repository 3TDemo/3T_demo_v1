#include "stdafx.h"
#include "Mask.h"


void Mask::maskout_points(vector<cv::Point2f>& p1, cv::Mat& mask)
{
	vector<cv::Point2f> p1_copy = p1;
	p1.clear();

	for (int i = 0; i < mask.rows; ++i)
	{
		if (mask.at<uchar>(i) > 0)
			p1.push_back(p1_copy[i]);
	}
}

void Mask::maskout_colors(vector<cv::Vec3b>& p1, cv::Mat& mask)
{
	vector<cv::Vec3b> p1_copy = p1;
	p1.clear();

	for (int i = 0; i < mask.rows; ++i)
	{
		if (mask.at<uchar>(i) > 0)
			p1.push_back(p1_copy[i]);
	}
}
