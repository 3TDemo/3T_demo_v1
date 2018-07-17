// work2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "FileIO.h"
#include <io.h>
#include "FeatureMatch.h"
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <vector>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/opencv.hpp>
#include "Structure.h"
#include "base.h"
#include"pointsOut.h"
#include"CalRAndTPlusColor.h"
#include"cmvsIO.h"
#include"ToMeshWithRGB.h"


//using namespace cv;
using namespace std;

void main()
{
	string root = "d:";
	cmvsIO::prepareDir(root);
	cmvsIO::prepareFiles(root);
	//cmvsIO::changeFiles(root);
	string ply = cmvsIO::init(root);
	plytomesh(ply);
}