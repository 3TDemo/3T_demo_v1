#include "FeatureMatch.h"
void FeatureMatch::OrbFeatureMatch()
{
	if (!img1.data || !img2.data)
	{
		cout << "error reading images " << endl;
	}
	Ptr<FeatureDetector> detector = ORB::create();
	Ptr<DescriptorExtractor> descriptor = ORB::create();
	Ptr<DescriptorMatcher> matcher_1 = DescriptorMatcher::create("BruteForce-Hamming");
	Ptr<ORB> orb = ORB::create();//(500, 1.1f, 10,  31, 0,  2, ORB::HARRIS_SCORE, 31);orb参数设置研究中
	Mat descriptors1, descriptors2;//描述符
	orb->detectAndCompute(img1, Mat(), l_keypoints, descriptors1);
	orb->detectAndCompute(img2, Mat(), r_keypoints, descriptors2);
	BFMatcher matcher(NORM_L2);
	matcher.match(descriptors1, descriptors2, matches);
	//可视化，显示关键点
	Mat ShowKeypoints1, ShowKeypoints2;
	drawKeypoints(img1, l_keypoints, ShowKeypoints1);
	drawKeypoints(img2, r_keypoints, ShowKeypoints2);
	imshow("l_keypoints", ShowKeypoints1);
	waitKey(0);
	imshow("r_keypoints", ShowKeypoints2);
	waitKey(0);

	//检测 Oriented FAST 角点位置
	detector->detect(img1, l_keypoints);
	detector->detect(img2, r_keypoints);
	//根据角点位置计算 BRIEF 描述子
	descriptor->compute(img1, l_keypoints, descriptors1);
	descriptor->compute(img2, r_keypoints, descriptors2);
	//对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
	vector<DMatch> matches_1;
	//BFMatcher matcher ( NORM_HAMMING );
	matcher_1->match(descriptors1, descriptors2, matches_1);
	//匹配点对筛选
	double min_dist = 1000, max_dist = 10;
	//找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
	for (int i = 0; i < descriptors1.rows; i++)
	{
		double dist = matches_1[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	//筛选
	vector< DMatch > f_matches;
	for (int i = 0; i < descriptors1.rows; i++)
	{
		if (matches_1[i].distance <= max(2 * min_dist, 20.0))
		{
			f_matches.push_back(matches_1[i]);
		}
	}
	//绘制匹配结果
	Mat img_f_match;
	drawMatches(img1, l_keypoints, img2, r_keypoints, f_matches, img_f_match);
	imshow("匹配点对", img_f_match);
	waitKey(0);
}
