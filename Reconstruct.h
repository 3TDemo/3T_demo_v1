#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
//using namespace cv;

class Reconstruct
{
public:
	
	//static void reconstruct(Mat & K, Mat & R1, Mat & T1, Mat & R2, Mat & T2, vector<Point2f>& p1, vector<Point2f>& p2, vector<Point3f>& structure);

	static void reconstruct(cv::Mat & R1, cv::Mat & T1, cv::Mat & R2, cv::Mat & T2, vector<cv::Point2f>& p1, vector<cv::Point2f>& p2, vector<cv::Point3f>& structure);

	static void save_structure(string file_name, vector<cv::Mat>& rotations, vector<cv::Mat>& motions, vector<cv::Point3f>& structure, vector<cv::Vec3b>& colors);

	static void maskout_points(vector<cv::Point2f>& p1, cv::Mat & mask);

	static void maskout_colors(vector<cv::Vec3b>& p1, cv::Mat & mask);

	static void fusion_structure(vector<cv::DMatch>& matches, vector<int>& struct_indices, vector<int>& next_struct_indices, vector<cv::Point3f>& structure, vector<cv::Point3f>& next_structure, vector<cv::Vec3b>& colors, vector<cv::Vec3b>& next_colors);

};

