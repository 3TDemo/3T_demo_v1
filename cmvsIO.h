#pragma once
#include <string>;
#include <vector>
#include "CalRAndTPlusColor.h"
#include "pointsOut.h"
using namespace std;

class cmvsIO {
private:

	//vector<>
	//string Root;
public:
	static CalRAndTPlusColor call();
	static void prepareImage(string filename);
	static void prepareDir(string rootdir);
	static void prepareFiles(string rootdir);
	static void getParams();
	static string init(string rootdir);
	static vector<string> pathChange(string input);
	//static void prepareBundle(CalRAndTPlusColor cratpc);
	static cv::Mat getMatP(cv::Mat inner, cv::Mat rotation, cv::Mat motion);
	static void changeFiles(string rootdir);
};