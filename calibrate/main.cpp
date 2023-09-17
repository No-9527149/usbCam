#include "calibrate.h"

int main() {
    vector<string> imgList;
    loadImgList("../image", imgList);
    split(imgList);
    return 0;
}