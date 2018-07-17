#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Transform.h"
#include "Mask.h"
#include "Reconstruct.h"

using namespace std;

class Structure
{
public:
	
	//static void init_structure(Mat K, vector<vector<KeyPoint>>& key_points_for_all, vector<vector<Vec3b>>& colors_for_all, vector<vector<DMatch>>& matches_for_all, vector<Point3f>& structure, vector<vector<int>>& correspond_struct_idx, vector<Vec3b>& colors, vector<Mat>& rotations, vector<Mat>& motions);

	static void init_structure(vector<vector<cv::KeyPoint>>& key_points_for_all, vector<cv::Mat>& colors_for_all, vector<vector<cv::DMatch>>& matches_for_all, vector<cv::Point3f>& structure, vector<vector<int>>& correspond_struct_idx, vector<cv::Vec3b>& colors, vector<cv::Mat>& rotations, vector<cv::Mat>& motions);

};

