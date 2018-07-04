#pragma once
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
class FeatureMatch {
private:
	vector<KeyPoint> l_keypoints, r_keypoints;
	vector<DMatch> matches;
	Mat img1, img2;
public:
	FeatureMatch(const Mat l_img, const Mat r_img)
	{
		l_img.copyTo(img1);
		r_img.copyTo(img2);
	}
	~FeatureMatch()
	{
		//ºı…Ÿ»›∆˜»›¡ø
		l_keypoints.shrink_to_fit();
		r_keypoints.shrink_to_fit();
		matches.shrink_to_fit();
		img1.release();
		img2.release();
	}
	void OrbFeatureMatch();
	vector<KeyPoint> getKeypoints_L() {
		return l_keypoints;
	}
	vector<KeyPoint> getKeypoints_R() {
		return r_keypoints;
	}
	vector<DMatch> getMatches() {
		return matches;
	}
};