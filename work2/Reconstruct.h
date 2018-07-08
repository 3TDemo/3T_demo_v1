#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class Reconstruct
{
public:
	
	//static void reconstruct(Mat & K, Mat & R1, Mat & T1, Mat & R2, Mat & T2, vector<Point2f>& p1, vector<Point2f>& p2, vector<Point3f>& structure);

	static void reconstruct(Mat & R1, Mat & T1, Mat & R2, Mat & T2, vector<Point2f>& p1, vector<Point2f>& p2, vector<Point3f>& structure);

	static void save_structure(string file_name, vector<Mat>& rotations, vector<Mat>& motions, vector<Point3f>& structure, vector<Vec3b>& colors);

	static void maskout_points(vector<Point2f>& p1, Mat & mask);

	static void maskout_colors(vector<Vec3b>& p1, Mat & mask);

	static void fusion_structure(vector<DMatch>& matches, vector<int>& struct_indices, vector<int>& next_struct_indices, vector<Point3f>& structure, vector<Point3f>& next_structure, vector<Vec3b>& colors, vector<Vec3b>& next_colors);

};

