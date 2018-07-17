#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

//using namespace cv;
using namespace std;

class Transform
{
public:
//	static bool find_transform(Mat & K, vector<Point2f>& p1, vector<Point2f>& p2, Mat & R, Mat & T, Mat & mask);
	static bool find_transform(vector<cv::Point2f>& p1, vector<cv::Point2f>& p2, cv::Mat & R, cv::Mat & T, cv::Mat & mask);
	static void get_matched_points(vector<cv::KeyPoint>& p1, vector<cv::KeyPoint>& p2, vector<cv::DMatch> matches, vector<cv::Point2f>& out_p1, vector<cv::Point2f>& out_p2);
	static void get_matched_colors(cv::Mat & image, vector<cv::KeyPoint>& keypoints, vector<cv::DMatch> matches, vector<cv::Vec3b>& out);
	//static void get_matched_colors(vector<Mat>& images_for_all, vector<DMatch> matches, vector<Vec3b>& out_c1, );
	//static void get_matched_colors(vector<Vec3b>& c1, vector<Vec3b>& c2, vector<DMatch> matches, vector<Vec3b>& out_c1, vector<Vec3b>& out_c2);

	static void get_objpoints_and_imgpoints(vector<cv::DMatch>& matches, vector<int>& struct_indices, vector<cv::Point3f>& structure, vector<cv::KeyPoint>& key_points, vector<cv::Point3f>& object_points, vector<cv::Point2f>& image_points);

};

