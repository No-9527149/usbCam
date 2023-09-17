#include <dirent.h>
#include <opencv2/opencv.hpp>

#include "calibrate.h"

using namespace cv;

/**
  * @brief  Load image list from folder path
  * @note   None
  * @param  imgDir  string
  * @param  imgList vector<string>
  * @retval true    If successfully load image list.
**/
bool loadImgList(const string &imgDir, vector<string> &imgList) {
    DIR *dir;
    struct dirent *ent;
    dir = opendir(imgDir.c_str());
    if (dir == nullptr)
        return false;

    while ((ent = readdir(dir)) != nullptr) {
        string fileName(ent->d_name);
        if (fileName.size() > 2){
            string filePath = imgDir + "/" + fileName;
            imgList.push_back(filePath);
        }
    }
    closedir(dir);
    for (const auto &imgPath : imgList) {
        cout << imgPath << endl;
    }
    return true;
}

/**
  * @brief  Split init image
  * @note   Split init image into left && right, and save as left/img_%d.jpg && right/img_%d.jpg.
  * @param  imgList vector<string>
  * @retval 0       If successfully split image.
**/
int split(vector<string> &imgList) {
    int imgNum = (int)imgList.size() / 2;
    cout << "Number of Initial Images: " << imgNum << endl;

    for (int i = 0; i < imgNum; ++i) {
        Mat initImg;
        initImg = imread(imgList[i]);
        if (!initImg.empty()) {
            int mid = initImg.cols / 2;
            Mat leftImg = initImg(Rect(0, 0, mid, initImg.rows));
            Mat rightImg = initImg(Rect(mid, 0, initImg.cols - mid, initImg.rows));

            string leftImgDir = "../image/left/img_" + typeToString(i) + ".jpg";
            string rightImgDir = "../image/right/img_" + typeToString(i) + ".jpg";

            cv::imwrite(leftImgDir, leftImg);
            cv::imwrite(rightImgDir, rightImg);
        }
    }
    return 0;
}