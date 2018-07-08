#include "cmvsIO.h";
#include <string>;
#include <string.h>
#include <vector>;
#include <iostream>;
#include <windows.h>;
#include "CalRAndTPlusColor.h";
#include <direct.h>;


using namespace std;

//创建cmvs所需的目录结构
void cmvsIO::prepareDir() {
	string root = "./cmvs";
	string pmvs = "./cmvs/pmvs";
	 string imagedir = "./cmvs/pmvs/visualize";
	 string txtdir = "./cmvs/pmvs/txt";
	 string modeldir = "./cmvs/pmvs/models";
	 bool flag1 = CreateDirectory(pmvs.c_str(), NULL);
	 bool imageflag = CreateDirectory(imagedir.c_str(), NULL);
	 bool txtflag = CreateDirectory(txtdir.c_str(), NULL);
	 bool molflag = CreateDirectory(modeldir.c_str(), NULL);
	 if (imageflag == true && txtflag == true && molflag == true) {
		 cout << "Directory created" << endl;
	 }
};

 //将图片文件按格式复制到目录中
/*void cmvsIO::prepareImage(string filename) {
	 string copy = "./cmvs/pmvs/visualize";
 };
 */

//创建CalRAnd实例
CalRAndTPlusColor cmvsIO::call() {
	CalRAndTPlusColor cal = CalRAndTPlusColor();
	return cal;
}

//计算P矩阵
Mat cmvsIO::getMatP(Mat inner, Mat rotation, Mat motion) {
	Mat result;
	Mat outer;
	outer = rotation + motion;
	result = inner * outer;
	return result;
};
 //生成需要的文件
void cmvsIO::prepareFiles(CalRAndTPlusColor cratpc) {
	int imageNum = 0;
	string txtdir = "./cmvs/pmvs/txt";
	string imgdir = "./cmvs/pmvs/visualize";
	//CalRAndTPlusColor cratpc = CalRAndTPlusColor();
	vector<Mat> rotation = cratpc.getRotation();       //相机旋转矩阵
	vector<Mat> motion = cratpc.getMotion();       //相机平移矩阵
	vector<int> ks = cratpc.getK1And1AndK2();          //相机畸变参数k1,k2
	vector<Point3f> cam3d = cratpc.getStructure();          //相机空间坐标
	vector<vector<vector<Point2f>>> points3d = cratpc.getMatchesPts();        //获取匹配点列表
	double focal = cratpc.getFocal();      //获取相机焦距
	vector<vector<int>> matchcam = cratpc.getCorrespondStru();         //获取每个稀疏点匹配的相机
	vector<vector<Point3f>> pointspo = cratpc.getPos();
	Mat inner = CalRAndTPlusColor::getInnerMat();
	int camNum = rotation.size();   //相机（图片）数
	int pointNum = points3d.size();     //稀疏点云点数
	int k1 = ks[0];      //畸变参数k1
	int k2 = ks[1];      //畸变参数k2
	/*----------------创建包含照片信息的txt文件-------------------------*/
	for (int i = 0; i < camNum; i++) {
		//文件名
		string num = to_string(i);
		string txtname = num + ".txt";
		string imgname = i + ".jpg";
		//文件地址
		string txtdir = "./cmvs/pmvs/txt/" + txtname;
		string imgdir = "./cmvs/pmvs/visualize/" + imgname;
		Mat rot = rotation[i];
		Mat mot = motion[i];
		Mat P = getMatP(inner, rot, mot);
		ofstream txtIO;
		txtIO.open(txtdir);
		txtIO << "CONTOUR" << endl;
		for (int n = 0; n < 3; n++) {
			txtIO << P.at<float>(n, 0) << "\t" << P.at<float>(n, 1) << "\t" << P.at<float>(n, 2) << "\t" << P.at<float>(n, 3) << endl;
		}
		txtIO.close();
	}
	/*--------------------------------创建bundle.rd.out--------------------------------------*/
	ofstream bundleIO;
	string bundledir = "./cmvs/pmvs/bundle.rd.out";
	bundleIO.open("bundledir");
	bundleIO << "# Bundle file v0.3" << endl << "<" << camNum << ">" << "\t" << "<" << pointNum << ">" << endl;
	for (int i = 0; i < camNum; i++) {
		bundleIO << "<camera" << i + 1 << ">" << endl;
		bundleIO << focal << "\t" << k1 << "\t" << k2 << endl;
		Mat rot = rotation[i];
		Mat mot = motion[i];
		for (int n = 0; n < 3; n++) {
			bundleIO << rot.at<float>(n, 0) << "\t" << rot.at<float>(n, 1) << "\t" << rot.at<float>(n, 2) << endl;
		}
		bundleIO << mot.at<float>(0, 0) << "\t" << mot.at<float>(0, 1) << "\t" << mot.at<float>(0, 2) << endl;
	}

	for (int i = 0; i < pointNum; i++) {
		bundleIO << "<point" << i + 1 << ">" << endl;
		vector<int> camlist = matchcam[i];    //匹配的相机列表
	}
};

//生成bundle.rd.out
/*void cmvsIO::prepareBundle(CalRAndTPlusColor cratpc) {
	vector<Mat> rotate = cratpc.getRotation();
	string dir = "./cmvs/pmvs";
	vector<vector<vector<Point2f>>> matchPoints = cratpc.getMatchesPts();
}
*/
 //生成稠密云
  void cmvsIO::init() {
	 //获取根目录存在rootStr中
	 TCHAR szPath[MAX_PATH] = { 0 };   
	 if (GetModuleFileName(NULL, szPath, MAX_PATH))
	 {
		 (_tcsrchr(szPath, _T('\\')))[1] = 0;
	 }
	 string rootStr = szPath;
	 //拼出system命令输出目录
	 string outStr = rootStr + "\\cmvs\\pmvs";
	 string outOpStr = rootStr + "\\cmvs\\pmvs\\option-0000";
	 //拼出system命令exe文件位置
	 string cStr = rootStr + "\\cmvs\\cmvs.exe";
	 string genStr = rootStr + "\\cmvs\\genOption.exe";
	 string pStr = rootStr + "\\cmvs\\pmvs.exe";
	 //拼出三条system命令
	 string sys1 = cStr + " " + outStr;
	 string sys2 = genStr + " " + outStr;
	 string sys3 = pStr + " " + outOpStr;
	 //执行system命令
	 system(sys1.c_str());
	 system(sys2.c_str());
	 system(sys3.c_str());

 }

 
