#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "FileIO.h"
#include <io.h>
#include "FeatureMatch.h"
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Structure.h"
#include "base.h"
#include "FeatureMatch.h"
#include"pointsOut.h"
class CalRAndTPlusColor
{
private:
	vector<cv::Mat> images_for_all;
	vector<vector<cv::KeyPoint>> key_points_for_all;
	vector<cv::Mat> descriptor_for_all;//用来匹配特征点的图像描述子
	vector<vector<cv::Vec3b>> colors_for_all;
	vector<vector<cv::DMatch>> matches_for_all;
	vector<point3d> points;

	//点
	vector<cv::Point3f> structure;
	//保存第i副图像中第j个特征点对应的structure中点的索引
	vector<vector<int>> correspond_struct_idx;
	//匹配点颜色存储向量
	vector<cv::Vec3b> colors;
	//旋转矩阵向量
	vector<cv::Mat> rotations;
	//平移矩阵向量
	vector<cv::Mat> motions;

	void loadMat();
	void match_all();
	void init_and_add();

public:
	void save();
	CalRAndTPlusColor();

	vector<cv::Mat>& getRotation() {
		return rotations;
	}//旋转矩阵

	vector<cv::Mat>& getMotion() {
		return motions;
	}//平移矩阵

	vector<cv::Vec3b>& getColor() {
		return colors;
	}//颜色向量

	static const cv::Mat& getInnerMat() {
		return K;
	}//内参矩阵

	static const double getFocal() {
		return focal;
	}//焦距
	vector<cv::Mat>& getImages()
	{
		return images_for_all;
	}
	vector<point3d> getPoints()
	{
		vector<point3d> points;
		getPointsData(structure,
			key_points_for_all,
			matches_for_all,
			images_for_all,
			correspond_struct_idx,
			
			points);
		return points;
	}//structure所有点信息
	static vector<int>& getK1And1AndK2() {
		vector<int> kp;
		kp.resize(2);
		for (int i = 0; i < 2; i++) {
			kp.push_back(0);
		}
		return kp;
	}//畸变参数k1和k2

	static vector<int>& getP1AndP2() {
		vector<int> p;
		p.resize(2);
		for (int i = 0; i < 2; i++) {
			p.push_back(0);
		}
		return p;
	}//畸变参数p1和p2

	vector<cv::Point3f>& getStructure() {

		return structure;
	}//每个相机的齐次坐标

	vector<vector<int>>& getCorrespondStru() {
		vector<vector<int>> matchStrut(matches_for_all.size());
		for (int i = 0; i < matches_for_all.size(); i++) {
			vector<int> temp(2);
			temp.push_back(i);
			temp.push_back(i + 1);
			matchStrut.push_back(temp);
		}
		return matchStrut;
	}//返回每个匹配点对应的相机编号

	vector<vector<vector<cv::Point2f>>>& getMatchesPts() {
		vector<vector<vector<cv::Point2f>>> matchPt(matches_for_all.size());
		for (int i = 0; i < matches_for_all.size(); i++) {
			vector<vector<cv::Point2f>> temp1;
			for (int j = 0; j < matches_for_all[i].size(); j++) {
				vector<cv::Point2f> temp2;
				temp2.push_back(key_points_for_all[i][matches_for_all[i][j].queryIdx].pt);
				temp2.push_back(key_points_for_all[i][matches_for_all[i][j].trainIdx].pt);

				temp1.push_back(temp2);
			}
			matchPt.push_back(temp1);
		}
		return matchPt;
	}//返回每个匹配点对应每张图片中的像素点

	vector<vector<cv::Mat>>& getPos() {
		vector<vector<cv::Mat>> posMat;
		for (int i = 0; i < matches_for_all.size(); i++) {
			vector<cv::Mat> pos;
			pos.push_back(cv::Mat());
		}
		return posMat;
	}//返回每个匹配点的空间坐标
};