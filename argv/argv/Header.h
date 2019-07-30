#pragma once
#include <iostream>
#include <atlstr.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

void print(CString str);
void myKmean(vector<vector<Point>> cnts , vector<vector<vector<Point>>> &clusters);
void vstack(vector<vector<Point>> cnts , vector<Point> &v);
float distance(Point p1, Point p2);
bool isNearest(vector<Point> cnt1, vector<Point> cnt2, float d);
bool isInCnts(vector<Point> cnt, vector<vector<Point>> cnts);
static Scalar randomColor(RNG& rng);
