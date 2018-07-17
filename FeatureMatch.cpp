#include "stdafx.h"
#include "FeatureMatch.h"


void FeatureMatch::FeatureObtatinAndMatchForAll(vector<cv::Mat>& images,
	vector<vector<cv::KeyPoint>>& key_points_for_all,
	vector<cv::Mat>& descriptor_for_all,
	vector<vector<cv::Vec3b>>& colors_for_all,
	vector<vector<cv::DMatch>>& matches_for_all)
{
	SiftFeatureMatch(images, key_points_for_all, descriptor_for_all
		//colors_for_all
	);
	//OrbFeatureMatch(images, key_points_for_all, descriptor_for_all, colors_for_all);
	matchFeatures(//image_for_all, 
		descriptor_for_all,
		matches_for_all);
}

void FeatureMatch::SiftFeatureMatch(
	vector<cv::Mat>& images,
	vector<vector<cv::KeyPoint>>& key_points_for_all,
	vector<cv::Mat>& descriptor_for_all
	//vector<vector<Vec3b>>& colors_for_all
)
{
	key_points_for_all.clear();
	descriptor_for_all.clear();
	cv::Mat image;

	//读取图像，获取图像特征点，并保存
	cv::Ptr<cv::Feature2D> sift = cv::xfeatures2d::SIFT::create(0, 3, 0.04, 10);
	for (auto it = images.begin(); it != images.end(); ++it)
	{
		cv::Mat img = *it;
		if (img.empty()) continue;

		vector<cv::KeyPoint> key_points;
		cv::Mat descriptor;
		//偶尔出现内存分配失败的错误
		sift->detectAndCompute(img, cv::noArray(), key_points, descriptor);

		//特征点过少，则排除该图像
		if (key_points.size() <= 10) continue;

		key_points_for_all.push_back(key_points);
		descriptor_for_all.push_back(descriptor);

		//vector<Vec3b> colors(key_points.size());

		//for (int i = 0; i < key_points.size(); ++i)
		//{
		//	Point2f& p = key_points[i].pt;
		//	auto color1 = img.at<Vec3b>(p.x, p.y);
		//	colors.push_back(color1);
		//}
		//colors_for_all.push_back(colors);
	}
}


void FeatureMatch::OrbFeatureMatch(
	vector<cv::Mat>& images,
	vector<vector<cv::KeyPoint>>& key_points_for_all,
	vector<cv::Mat>& descriptor_for_all,
	vector<vector<cv::Vec3b>>& colors_for_all
)
{
	key_points_for_all.clear();
	descriptor_for_all.clear();
	colors_for_all.clear();

	cv::Ptr<cv::ORB> orb = cv::ORB::create(500, 1.1f, 10, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31);//(500, 1.1f, 10,  31, 0,  2, ORB::HARRIS_SCORE, 31);orb参数设置研究中

	cv::Mat image;
	for (auto it = images.begin(); it != images.end(); ++it)
	{
		image = *it;
		if (image.empty()) continue;

		//cout << "Extracing features: " << *it << endl;

		vector<cv::KeyPoint> key_points;
		cv::Mat descriptor;

		//偶尔出现内存分配失败的错误
		orb->detectAndCompute(image, cv::Mat(), key_points, descriptor);

		//特征点过少，则排除该图像
		if (key_points.size() <= 10) continue;

		key_points_for_all.push_back(key_points);
		descriptor_for_all.push_back(descriptor);

		vector<cv::Vec3b> colors(key_points.size());

		//for (int i = 0; i < key_points.size(); ++i)
		//{
		//	Point2f& p = key_points[i].pt;
		//	colors[i] = image.at<Vec3b>(p.y, p.x);
		//}
		//colors_for_all.push_back(colors);
	}
}

void
FeatureMatch::matchFeatures
(cv::Mat & query,
	cv::Mat & train,
	//	Mat& descriptors1,
	//	Mat& descriptors2,
	vector<cv::DMatch>& matches)
{
	//Ptr<FeatureDetector> detector = ORB::create();
	//Ptr<DescriptorExtractor> descriptor = ORB::create();
	//Ptr<DescriptorMatcher> matcher_1 = DescriptorMatcher::create("BruteForce-Hamming");
	////检测 Oriented FAST 角点位置
	//vector<KeyPoint> l_keyPoints;
	//vector<KeyPoint> r_keyPoints;
	//detector->detect(query, l_keyPoints);
	//detector->detect(train, r_keyPoints);
	////根据角点位置计算 BRIEF 描述子
	//descriptor->compute(query, l_keyPoints, descriptors1);
	//descriptor->compute(query, r_keyPoints, descriptors2);
	////对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
	//vector<DMatch> matches_1;
	////BFMatcher matcher ( NORM_HAMMING );
	//matcher_1->match(descriptors1, descriptors2, matches_1);
	////匹配点对筛选
	//double min_dist = 1000, max_dist = 10;
	////找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
	//for (int i = 0; i < descriptors1.rows; i++)
	//{
	//	double dist = matches_1[i].distance;
	//	if (dist < min_dist) min_dist = dist;
	//	if (dist > max_dist) max_dist = dist;
	//}
	////筛选
	//for (int i = 0; i < descriptors1.rows; i++)
	//{
	//	if (matches_1[i].distance <= max(2 * min_dist, 20.0))
	//	{
	//		matches.push_back(matches_1[i]);
	//	}
	//}
	vector<vector<cv::DMatch>> knn_matches;
	cv::BFMatcher matcher(cv::NORM_L2);
	matcher.knnMatch(query, train, knn_matches, 2);

	//获取满足Ratio Test的最小匹配的距离
	float min_dist = FLT_MAX;
	for (int r = 0; r < knn_matches.size(); ++r)
	{
		//Ratio Test
		if (knn_matches[r][0].distance > 0.6*knn_matches[r][1].distance)
			continue;

		float dist = knn_matches[r][0].distance;
		if (dist < min_dist) min_dist = dist;
	}

	matches.clear();
	for (size_t r = 0; r < knn_matches.size(); ++r)
	{
		//排除不满足Ratio Test的点和匹配距离过大的点
		if (
			knn_matches[r][0].distance > 0.6*knn_matches[r][1].distance ||
			knn_matches[r][0].distance > 5 * max(min_dist, 10.0f)
			)
			continue;
		//保存匹配点
		matches.push_back(knn_matches[r][0]);
	}
}

void
FeatureMatch::matchFeatures
(
	vector<cv::Mat>& descriptor_for_all,
	vector<vector<cv::DMatch>>& matches_for_all
)
{
	matches_for_all.clear();
	// n个图像，两两顺次有 n-1 对匹配
	for (int i = 0; i < descriptor_for_all.size() - 1; ++i)
	{
		cout << "Matching images " << i << " - " << i + 1 << endl;

		vector<cv::DMatch> matches;

		matchFeatures(descriptor_for_all[i], descriptor_for_all[i + 1], matches);

		//储存n-1个matches结果
		matches_for_all.push_back(matches);
	}
}
