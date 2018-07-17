#include"stdafx.h"
#include <iostream>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
#include <pcl/common/transforms.h>
#include <pcl/point_types.h>
#include <pcl/io/io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/NOrmal_3d_omp.h>
#include <pcl/features/NOrmal_3d.h>
#include <pcl/features/integral_image_normal.h>
#include <pcl/filters/passthrough.h>
#include <pcl/surface/gp3.h>
#include <pcl/surface/poisson.h>
#include <process.h>
#include <boost/thread/thread.hpp>
#include <fstream>
#include <math.h>
#include <random>
#include <sstream>
using std::string;

void plytomesh(string plyname);
