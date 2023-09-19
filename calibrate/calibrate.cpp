#include <dirent.h>
#include "calibrate.h"

const float CHESSBOARD_SQUARE_SIZE = 12.5f;
#define CHESSBOARD_SQUARE_WIDTH_NUM 9;
#define CHESSBOARD_SQUARE_HEIGHT_NUM 8;

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
    int imgNum = (int)imgList.size();
    cout << "Number of Initial Images: " << imgNum << endl;

    for (int i = 0; i < imgNum; ++i) {
        Mat initImg;
        initImg = imread(imgList[i]);
        if (!initImg.empty()) {
            int mid = initImg.cols / 2;
            Mat leftImg = initImg(Rect(0, 0, mid, initImg.rows));
            Mat rightImg = initImg(Rect(mid, 0, initImg.cols - mid, initImg.rows));

            string leftImgDir = "../../calibrate/left/img_" + typeToString(i) + ".jpg";
            string rightImgDir = "../calibrate/right/img_" + typeToString(i) + ".jpg";

            cv::imwrite(leftImgDir, leftImg);
            cv::imwrite(rightImgDir, rightImg);
        }
    }
    return 0;
}

/**
  * @brief  Calculate chessboard corners
  * @note   None
  * @param  boardSize   Size
  * @param  corners     vector<Point3f>
  * @retval None
**/
void calChessBoardCorners(Size boardSize, vector<Point3f> &corners) {

    corners.resize(0);
    for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j <boardSize.width; ++j) {
            corners.push_back((Point3f(float(j) * CHESSBOARD_SQUARE_SIZE, float(i) * CHESSBOARD_SQUARE_SIZE, 0)));
        }
    }
}

/**
  * @brief  Calibrate
  * @note   None
  * @param  interMat        Mat
  * @param  disCoe          Mat
  * @param  imgPoints       vector<vector<Point2f>>
  * @param  objectPoints    vector<vector<Point3f>>
  * @param  imgSize         Size
  * @param  imgList         vector<string>
  * @retval true            If done with calibrate
**/
bool calibrate(Mat &interMat, Mat &disCoe, vector<vector<Point2f>> &imgPoints, vector<vector<Point3f>> &objectPoints, Size &imgSize, vector<string> imgList) {
    /* Error of reProjection */
    double errorReProjection = 0;

    Size boardSize;
    boardSize.width = CHESSBOARD_SQUARE_WIDTH_NUM;
    boardSize.height = CHESSBOARD_SQUARE_HEIGHT_NUM;

    vector<Point2f> pointBuffer;

    vector <Mat> rotationVector, transVector;

    bool flag = false;

    int imgNum = (int) imgList.size() / 2;
    cout << "Number of Images: " << imgNum << endl;
    namedWindow("view", 1);

    /* Number of valid chessboard images */
    int nums = 0;

    for(int i = 0; i < imgNum; ++i) {
        Mat view, viewGray;
        view = imread(imgList[i]);
        imgSize = view.size();

        cvtColor(view, viewGray, COLOR_BGR2GRAY);

        bool found = findChessboardCorners(view, boardSize, pointBuffer, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            nums++;
            cornerSubPix(viewGray, pointBuffer, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::Type::EPS + TermCriteria::Type::MAX_ITER, 30, 0.1));
            drawChessboardCorners(view, boardSize, Mat(pointBuffer), found);
            bitwise_not(view, view);
            imgPoints.push_back(pointBuffer);
            cout << ".";
        }
        imshow("view", view);
        waitKey(50);
    }
    cout << "Valid numbers of chessboard: " << nums << endl;

    /* Calculate chessboard */
    calChessBoardCorners(boardSize, objectPoints[0]);
    objectPoints.resize(imgPoints.size(), objectPoints[0]);

    errorReProjection = calibrateCamera(objectPoints, imgPoints, imgSize, interMat, disCoe, rotationVector, transVector);
    flag = checkRange(interMat) && checkRange(disCoe);

    if (flag) {
        cout << "Done with reProjection Error: " << errorReProjection << endl;
        return true;
    }
    else{
        return false;
    }
}

