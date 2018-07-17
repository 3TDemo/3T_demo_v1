#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
//using namespace cv;

class FeatureMatch {

public:

	static void FeatureObtatinAndMatchForAll(vector<cv::Mat>& images, vector<vector<cv::KeyPoint>>& key_points_for_all, vector<cv::Mat>& descriptor_for_all, vector<vector<cv::Vec3b>>& colors_for_all, vector<vector<cv::DMatch>>& matches_for_all);

	static void SiftFeatureMatch(vector<cv::Mat>& images, vector<vector<cv::KeyPoint>>& key_points_for_all, vector<cv::Mat>& descriptor_for_all
		//, vector<vector<Vec3b>>& colors_for_all
	);

	static void OrbFeatureMatch(vector<cv::Mat>& image_names, vector<vector<cv::KeyPoint>>& key_points_for_all, vector<cv::Mat>& descriptor_for_all, vector<vector<cv::Vec3b>>& colors_for_all);

	static void matchFeatures(cv::Mat & query, cv::Mat & train, vector<cv::DMatch>& matches);

	static void matchFeatures(vector<cv::Mat>& descriptor_for_all, vector<vector<cv::DMatch>>& matches_for_all);

	//	static void matchFeatures(Mat & query, Mat & train, Mat & descriptors1, Mat & descriptors2, vector<DMatch>& matches);

	//	static void matchFeatures(vector<Mat>& image_for_all, vector<Mat>& descriptor_for_all, vector<vector<DMatch>>& matches_for_all);
};