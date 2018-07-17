#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
//using namespace cv;


class Mask
{
public:

	static void maskout_points(vector<cv::Point2f>& p1, cv::Mat & mask);

	static void maskout_colors(vector<cv::Vec3b>& p1, cv::Mat & mask);

};

