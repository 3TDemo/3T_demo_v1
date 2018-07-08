#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


class Mask
{
public:

	static void maskout_points(vector<Point2f>& p1, Mat & mask);

	static void maskout_colors(vector<Vec3b>& p1, Mat & mask);

};

