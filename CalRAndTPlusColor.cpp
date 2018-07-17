#include "stdafx.h"
#include "CalRAndTPlusColor.h"

void CalRAndTPlusColor::loadMat()
{
	FilesIO files("src/", ".jpg");
	this->images_for_all = files.getImages();
}

void CalRAndTPlusColor::match_all()
{
	//提取所有图像的特征
	//对所有图像进行顺次的特征匹配
	FeatureMatch::FeatureObtatinAndMatchForAll(images_for_all, key_points_for_all, descriptor_for_all, colors_for_all, matches_for_all);

}



void CalRAndTPlusColor::init_and_add() {
	//初始化结构（三维点云）
	Structure::init_structure(
		key_points_for_all,
		images_for_all,
		matches_for_all,

		structure,
		correspond_struct_idx,
		colors,//筛选颜色
		rotations,
		motions
	);
	for (int i = 1; i < matches_for_all.size(); ++i)
	{
		vector<cv::Point3f> object_points;
		vector<cv::Point2f> image_points;
		cv::Mat r, R, T;
		//Mat mask;

		//获取第i幅图像中匹配点对应的三维点，以及在第i+1幅图像中对应的像素点
		Transform::get_objpoints_and_imgpoints(
			matches_for_all[i],
			correspond_struct_idx[i],
			structure,
			key_points_for_all[i + 1],

			object_points,
			image_points
		);


		//求解变换矩阵
		solvePnPRansac(object_points, image_points, K, cv::noArray(), r, T);
		//将旋转向量转换为旋转矩阵
		Rodrigues(r, R);

		//保存变换矩阵
		rotations.push_back(R);
		motions.push_back(T);

		vector<cv::Point2f> p1, p2;
		vector<cv::Vec3b> c;
		Transform::get_matched_points(key_points_for_all[i], key_points_for_all[i + 1], matches_for_all[i], p1, p2);
		Transform::get_matched_colors(images_for_all[0], key_points_for_all[i], matches_for_all[i], c);

		//根据之前求得的R，T进行三维重建
		vector<cv::Point3f> next_structure;
		Reconstruct::reconstruct(rotations[i], motions[i], R, T, p1, p2, next_structure);

		//将新的重建结果与之前的融合
		Reconstruct::fusion_structure(
			matches_for_all[i],
			correspond_struct_idx[i],
			correspond_struct_idx[i + 1],
			structure,
			next_structure,
			colors,
			c
		);
	}
	
}

void CalRAndTPlusColor::save()
{
	Reconstruct::save_structure(".\\structure.yml", rotations, motions, structure, colors);
}

CalRAndTPlusColor::CalRAndTPlusColor()
{
	loadMat();
	match_all();
	init_and_add();
	save();
}
