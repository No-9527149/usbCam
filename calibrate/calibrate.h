#ifndef USBCAM_CALIBRATE_H
#define USBCAM_CALIBRATE_H

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

bool loadImgList(const string &imgDir, vector<string> &imgList);
int split(vector<string> &imgList);


#endif
