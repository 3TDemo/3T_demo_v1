#include <string>;
#include <vector>
#include "CalRAndTPlusColor.h"
using namespace std;

class cmvsIO {
private:
	//vector<>
	//string Root;
public:
	static CalRAndTPlusColor call();
	static void prepareImage(string filename);
	static void prepareDir();
	static void prepareFiles(CalRAndTPlusColor cratpc);
	static void getParams();
	static void init();
	//static void prepareBundle(CalRAndTPlusColor cratpc);
	static Mat getMatP(Mat inner, Mat rotation, Mat motion);
};
#pragma once
