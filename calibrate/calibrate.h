#ifndef USBCAM_CALIBRATE_H
#define USBCAM_CALIBRATE_H

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool loadImgList(const string &imgDir, vector<string> &imgList);
int split(vector<string> &imgList);
void calChessBoardCorners(Size boardSize, vector<Point3f> &corners);
bool calibrate(Mat &interMat, Mat &disCoe, vector<vector<Point2f>> &imgPoints, vector<vector<Point3f>> &objectPoints, Size &imgSize, vector<string> imgList);

#endif
