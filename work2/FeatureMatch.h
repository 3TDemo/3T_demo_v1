#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class FeatureMatch {

public:

	static void FeatureObtatinAndMatchForAll(vector<Mat>& images, vector<vector<KeyPoint>>& key_points_for_all, vector<Mat>& descriptor_for_all, vector<vector<Vec3b>>& colors_for_all, vector<vector<DMatch>>& matches_for_all);

	static void SiftFeatureMatch(vector<Mat>& images, vector<vector<KeyPoint>>& key_points_for_all, vector<Mat>& descriptor_for_all, vector<vector<Vec3b>>& colors_for_all);

	static void OrbFeatureMatch(vector<Mat>& image_names, vector<vector<KeyPoint>>& key_points_for_all, vector<Mat>& descriptor_for_all, vector<vector<Vec3b>>& colors_for_all);

	static void matchFeatures(Mat & query, Mat & train, vector<DMatch>& matches);

	static void matchFeatures(vector<Mat>& descriptor_for_all, vector<vector<DMatch>>& matches_for_all);

//	static void matchFeatures(Mat & query, Mat & train, Mat & descriptors1, Mat & descriptors2, vector<DMatch>& matches);

//	static void matchFeatures(vector<Mat>& image_for_all, vector<Mat>& descriptor_for_all, vector<vector<DMatch>>& matches_for_all);
};