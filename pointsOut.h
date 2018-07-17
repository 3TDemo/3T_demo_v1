#pragma once
#include "stdafx.h"
#include <iostream>
#include "FeatureMatch.h"
#include <opencv2/opencv.hpp>


using namespace std;
//using namespace cv;

struct point2d
{
	int camera;
	int pointIdx;
	cv::Point2d pt2;
};

struct point3d
{
	cv::Point3f pt3;
	cv::Vec3b color;
	vector<point2d> points2d;
};

void getPointsData(vector<cv::Point3f>& structure,
	vector<vector<cv::KeyPoint>>& key_point_for_all,
	vector<vector<cv::DMatch>>& matches_for_all,
	vector<cv::Mat>& image_for_all,
	vector<vector<int>>& correspond_struct_idx,
	
	vector<point3d>& points)
{
	point2d point;//前一个相机的2维点
	point2d point2; //后一个相机的2维点
	point3d point3;//一个三维点
	for (int i = 0; i < matches_for_all.size(); i++)
	{
		for (int j = 0; j < matches_for_all[i].size(); j++)
		{
			point3.points2d.clear();
			cv::DMatch match = matches_for_all[i][j];//匹配点
			int matchIdx = correspond_struct_idx[i][match.queryIdx];//该匹配点在structure中的索引
			if (matchIdx < 0)
				continue;//索引< 0 说明不在structure内
			point.camera = i;
			point.pointIdx = match.queryIdx;
			cv::Point2d pt = key_point_for_all[i][point.pointIdx].pt;
			cv::Mat img = image_for_all[i];
			//获取图片长宽
			int width = img.cols;
			int height = img.rows;
			//将原点移至图片中心
			point.pt2 = cv::Point2d(pt.x - width/2,pt.y - height/2);
			
			

			point2.camera = i + 1;
			point2.pointIdx = match.trainIdx;
			cv::Point2d pt2 = key_point_for_all[i + 1][point2.pointIdx].pt;
			cv::Mat img2 = image_for_all[i + 1];
			int width2 = img.cols;
			int height2 = img.rows;
			point2.pt2 = cv::Point2d(pt2.x - width2 / 2, pt2.y - height2 / 2);

			if (matchIdx < points.size())//点已经存在
			{
				points[matchIdx].points2d.push_back(point2);
				continue;
			}
			point3.pt3 = structure[matchIdx];		
			point3.color = image_for_all[i].at<cv::Vec3b>(pt.y, pt.x);
			point3.points2d.push_back(point);
			point3.points2d.push_back(point2);
			
			points.push_back(point3);
		}
	}
}

