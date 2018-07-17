#include "stdafx.h"
#include "Structure.h"

void Structure::init_structure(
	vector<vector<cv::KeyPoint>>& key_points_for_all,
	/*vector<vector<Vec3b>>& colors_for_all,*/
	vector<cv::Mat>& images_for_all,
	vector<vector<cv::DMatch>>& matches_for_all,

	vector<cv::Point3f>& structure,//保存所有相机的齐次坐标
	vector<vector<int>>& correspond_struct_idx,//2幅图像的结构索引
	vector<cv::Vec3b>& colors,
	vector<cv::Mat>& rotations,
	vector<cv::Mat>& motions
)
{
	//计算头两幅图像之间的变换矩阵
	vector<cv::Point2f> p1, p2;
	//vector<Vec3b> c2;//无作用
	cv::Mat R, T;	//旋转矩阵和平移向量

	cv::Mat mask;	//mask中大于零的点代表匹配点，等于零代表失配点
	Transform::get_matched_points(key_points_for_all[0], key_points_for_all[1], matches_for_all[0], p1, p2);
	Transform::get_matched_colors(images_for_all[0], key_points_for_all[0], matches_for_all[0], colors);

	Transform::find_transform(p1, p2, R, T, mask);

	//对头两幅图像进行三维重建
	//排除误匹配的点
	Mask::maskout_points(p1, mask);
	Mask::maskout_points(p2, mask);
	Mask::maskout_colors(colors, mask);

	//第一个相机设置为世界坐标
	cv::Mat R0 = cv::Mat::eye(3, 3, CV_64FC1);//矩阵规范化
	cv::Mat T0 = cv::Mat::zeros(3, 1, CV_64FC1);

	Reconstruct::reconstruct(R0, T0, R, T, p1, p2, structure);

	//保存每一个相机相对于前一个相机的变换矩阵//初始化加入2个 n-1+1
	rotations = { R0, R };
	motions = { T0, T };

	//将correspond_struct_idx的大小初始化为与key_points_for_all完全一致
	correspond_struct_idx.clear();
	correspond_struct_idx.resize(key_points_for_all.size());//n个相机n个结构索引
	for (int i = 0; i < key_points_for_all.size(); ++i)//
	{
		correspond_struct_idx[i].resize(key_points_for_all[i].size(), -1);//
	}

	//填写头两幅图像的结构索引
	int idx = 0;
	vector<cv::DMatch>& matches = matches_for_all[0];
	for (int i = 0; i < matches.size(); i++)
	{
		if (mask.at<uchar>(i) == 0) {
			continue;
		}
		//matches[i].queryIdx为keyPoints中matches点的坐标
		correspond_struct_idx[0][matches[i].queryIdx] = idx;
		correspond_struct_idx[1][matches[i].trainIdx] = idx;
		++idx;
	}
	//for (int i = 0; i < matches_for_all.size(); i++) {
	//	vector<DMatch>& matches = matches_for_all[i];
	//	for (int j = 0; j < matches.size(); i++)
	//	{
	//		if (mask.at<uchar>(j) == 0) {
	//			cout << "skip" << endl;
	//			continue;
	//		}
	//		//matches[i].queryIdx为keyPoints中matches点的坐标
	//		correspond_struct_idx[i][matches[j].queryIdx] = idx;
	//		correspond_struct_idx[i + 1][matches[j].trainIdx] = idx;
	//		++idx;
	//	}
	//}
}