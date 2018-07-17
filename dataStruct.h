#pragma once
#include <opencv2/core/core.hpp>
#include <vector>
#include <iostream>
#include <list>
#include <set>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace std;
//using namespace cv;
struct CloudPoint {
	cv::Point3d pt;
	vector<int> imgpt_for_img;
	double reprojection_error;
};
struct MyPoint
{
	double x;
	double y;
	double z;
	short intensity;
	MyPoint() {  }
	MyPoint(double tx, double ty, double tz, short illu) {
		x = tx;
		y = ty;
		z = tz;
		intensity = illu;
	}
	bool operator==(const MyPoint& rhs) {
		return(x == rhs.x) && (y == rhs.y) && (z == rhs.z);
	}
	MyPoint operator+(const MyPoint& rhs) {
		MyPoint out;
		out.x = x + rhs.x;
		out.y = y + rhs.y;
		out.z = z + rhs.z;
		return out;
	}
	MyPoint operator/(const int n) {
		MyPoint out;
		out.x = x / n;
		out.y = y / n;
		out.z = z / n;
		return out;
	}
	MyPoint operator=(const MyPoint& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		intensity = rhs.intensity;
		return MyPoint(x, y, z, intensity);
	}
	void print()
	{
		std::cout << "x = " << x << "  y = " << y << "  z = " << z << std::endl;
	}
};
vector<cv::DMatch> FlipMatches(const vector<cv::DMatch>& matches);
void KeyPointsToPoints(const vector<cv::KeyPoint>& kps, vector<cv::Point2f>& ps);
void PointsToKeyPoints(const vector<cv::Point2f>& ps, vector<cv::KeyPoint>& kps);
vector<cv::Point3d> CloudPointsToPoints(const vector<CloudPoint> cpts);
void drawArrows(cv::Mat& frame, const vector<cv::Point2f>& prevPts, const vector<cv::Point2f>& nextPts, const vector<uchar>& status, const vector<float>& verror, const cv::Scalar& line_color = cv::Scalar(0, 0, 255));
void imshow_250x250(const string& name_, const cv::Mat& patch);
vector<cv::DMatch> FlipMatches(const vector<cv::DMatch>& matches) {
	vector<cv::DMatch> flip;
	for (int i = 0; i<matches.size(); i++) {
		flip.push_back(matches[i]);
		swap(flip.back().queryIdx, flip.back().trainIdx);
	}
	return flip;
}
vector<cv::Point3d> CloudPointsToPoints(const vector<CloudPoint> cpts) {
	vector<cv::Point3d> out;
	for (unsigned int i = 0; i<cpts.size(); i++) {
		out.push_back(cpts[i].pt);
	}
	return out;
}
void KeyPointsToPoints(const vector<cv::KeyPoint>& kps, vector<cv::Point2f>& ps) {
	ps.clear();
	for (unsigned int i = 0; i<kps.size(); i++) ps.push_back(kps[i].pt);
}
void PointsToKeyPoints(const vector<cv::Point2f>& ps, vector<cv::KeyPoint>& kps) {
	kps.clear();
	for (unsigned int i = 0; i<ps.size(); i++) kps.push_back(cv::KeyPoint(ps[i], 1.0f));
}
#define intrpmnmx(val,min,max) (max==min ? 0.0 : ((val)-min)/(max-min))
void drawArrows(cv::Mat& frame, const vector<cv::Point2f>& prevPts, const vector<cv::Point2f>& nextPts, const vector<uchar>& status, const vector<float>& verror, const cv::Scalar& _line_color)
{
	double minVal, maxVal; cv::minMaxIdx(verror, &minVal, &maxVal, 0, 0, status);
	int line_thickness = 1;
	for (size_t i = 0; i < prevPts.size(); ++i)
	{
		if (status[i])
		{
			double alpha = intrpmnmx(verror[i], minVal, maxVal); alpha = 1.0 - alpha;
			cv::Scalar line_color(alpha*_line_color[0],
				alpha*_line_color[1],
				alpha*_line_color[2]);
			cv::Point p = prevPts[i];
			cv::Point q = nextPts[i];
			double angle = atan2((double)p.y - q.y, (double)p.x - q.x);
			double hypotenuse = sqrt((double)(p.y - q.y)*(p.y - q.y) + (double)(p.x - q.x)*(p.x - q.x));
			if (hypotenuse < 1.0)
				continue;
			// Here we lengthen the arrow by a factor of three.
			q.x = (int)(p.x - 3 * hypotenuse * cos(angle));
			q.y = (int)(p.y - 3 * hypotenuse * sin(angle));
			// Now we draw the main line of the arrow.
			line(frame, p, q, line_color, line_thickness);
			// Now draw the tips of the arrow. I do some scaling so that the
			// tips look proportional to the main line of the arrow.
			p.x = (int)(q.x + 9 * cos(angle + CV_PI / 4));
			p.y = (int)(q.y + 9 * sin(angle + CV_PI / 4));
			line(frame, p, q, line_color, line_thickness);
			p.x = (int)(q.x + 9 * cos(angle - CV_PI / 4));
			p.y = (int)(q.y + 9 * sin(angle - CV_PI / 4));
			line(frame, p, q, line_color, line_thickness);
		}
	}
}
bool hasEnding(string const &fullString, string const &ending)
{
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}
bool hasEndingLower(string const &fullString_, string const &_ending)
{
	string fullstring = fullString_, ending = _ending;
	transform(fullString_.begin(), fullString_.end(), fullstring.begin(), ::tolower); // to lower
	return hasEnding(fullstring, ending);
}
void imshow_250x250(const string& name_, const cv::Mat& patch) {
	cv::Mat bigpatch; resize(patch, bigpatch, cv::Size(250, 250));
	imshow(name_, bigpatch);
}
