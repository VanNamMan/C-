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
void myKmean(vector<vector<Point>> cnts , vector<vector<Point>> &clusters , int d);
void vstack(vector<vector<Point>> cnts , vector<Point> &v);
float distance(Point p1, Point p2);
int distance(Rect r1, Rect r2);
int distance(Rect r, vector<Rect> vRect);
static Scalar randomColor(RNG& rng);


