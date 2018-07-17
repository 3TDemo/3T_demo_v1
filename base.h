#pragma once
#pragma once
#include "stdafx.h"
#include "FeatureMatch.h"

const double CCDwidth = 35.94;
const double FocalLength = 28.0;

const double Picture_cx = 4369;
const double Picture_cy = 2912;
const double ncx = 4369, nfx = 4369;

const double dx = (1 / CCDwidth * 25.4 * 4 / 5) / Picture_cx;
const double dy = (1 / CCDwidth * 25.4 * 4 / 5) / Picture_cy;

const double dpx = dx * ncx / nfx;
const double dpy = dy;

const double fu = 28.0, fv = 28.0;

const double ku = fu / dpx;
const double kv = fv / dpy;

const cv::Matx33d matx = cv::Matx33d(
	2759.48, 0, 1520.69,
	0, 2764.16, 1006.81,
	0, 0, 1);

const cv::Matx33d matx2 = cv::Matx33d(
	660.803, 0, 320,
	0, 660.803, 240,
	0, 0, 1);

const cv::Matx33d matx3 = cv::Matx33d(
	3424.7491, 0, 1536,
	0, 3424.7491, 1152,
	0, 0, 1);

const cv::Mat K = cv::Mat(matx3);

const double focal = 0.5*(K.at<double>(0) + K.at<double>(4));

const cv::Point2d p_p(K.at<double>(2), K.at<double>(5));