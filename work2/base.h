#pragma once
#include "stdafx.h"
#include "FeatureMatch.h"


const double	
focal = 5.0,
u0 = 320,
v0 = 240,//u0，v0表示图像的中心像素坐标和图像原点像素坐标之间相差的横向和纵向像素数。

//dx和dy表示：x方向和y方向的一个像素分别占多少长度单位，即一个像素代表的实际物理值的大小，其是实现图像物理坐标系与像素坐标系转换的关键。
dx = 5.312 / 640,
dy = 3.984 / 480,

fu = focal / dx,
fv = focal / dy;




const double	fx = focal * 180 / 25.4, 
				fy = focal * 180 / 25.4;

const Matx33d matx = Matx33d(fx, 0, u0,
							0, fy, v0,
							0, 0, 1);

const Mat K = Mat(matx);//内参矩阵

const Point2d p_p(fu, fv);//主点坐标