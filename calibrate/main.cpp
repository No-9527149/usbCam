#include "calibrate.h"

int IF_SPLIT = 0;

int main() {
    if (IF_SPLIT) {
        vector<string> imgList;
        loadImgList("../../image", imgList);
        split(imgList);
        return 0;
    }

    /* Params init */
    bool calibrateRet = false;
    Mat interMatL, interMatR, disCoeL, disCoeR;
    Mat R, T, E, F, RL, RR, PL, PR, Q;
    vector<vector<Point2f>> imgPointsL, imgPointsR;
    vector<vector<Point3f>> objectPoints(1);
    Rect validROI[2];
    Size imgSize;
    int capL = 0, caR = 1;
    double errorReProjection = 0;

    /* Read images */
    vector<string> imgListLeft;
    vector<string> imgListRight;

    string imgDirLeft = "../../calibrate/left";
    string imgDirRight = "../../calibrate/right";
    bool loadRetLeft = false;
    bool loadRetRight = false;
    loadRetLeft = loadImgList(imgDirLeft, imgListLeft);
    loadRetRight = loadImgList(imgDirRight, imgListRight);
    if (!loadRetLeft || !loadRetRight || imgListLeft.empty() || imgListRight.empty()) {
        return -1;
    }

    FileStorage fs("../../config/intrinsics.yml" ,FileStorage::WRITE);

    /* Calibrate */
    cout << "Calibrating left camera" << endl;
    calibrateRet = calibrate(interMatL, disCoeL, imgPointsL, objectPoints, imgSize, imgListLeft);
    if(!calibrateRet) {
        cout << "Error: Calibrating left camera failed!" << endl;
        return -1;
    }
    else {
        cout << "Calibrating right camera..." << endl;
    }
    calibrateRet = calibrate(interMatR, disCoeR, imgPointsR, objectPoints, imgSize, imgListRight);
    if (!calibrateRet) {
        cout << "Error: Calibrating right camera failed!" << endl;
        return -1;
    }

    fs << "M1" << interMatL << "D1" << disCoeL << "M2" << interMatR << "D2" << disCoeR;

    /* Estimate position and orientation */
    cout << "Estimating position and orientation of the right relative to the left camera" << endl;
    cout << interMatL << endl;
    cout << disCoeL << endl;
    cout << interMatR << endl;
    cout << disCoeR << endl;

    errorReProjection = stereoCalibrate(objectPoints, imgPointsL, imgPointsR, interMatL, disCoeL, interMatR, disCoeR, imgSize, R, T, E, F, CALIB_USE_INTRINSIC_GUESS);
    cout << "Error of ReProjection = " << errorReProjection << endl;

    /* Load image for 3d-reconstruction */
    if (fs.isOpened()) {
        cout << "in";
        fs << "R" << R << "T" << T << "RL" << RL << "RR" << RR << "PL" << PL << "PR" << PR << "Q" << Q;
        fs.release();
    }

    namedWindow("Canvas", 1);
    cout << "Loading images for 3d-reconstruction...";
    Mat canvas(imgSize.height, imgSize.width * 2, CV_8UC3), viewL, viewR;
    Mat canL = canvas(Rect(0, 0, imgSize.width, imgSize.height));
    Mat canR = canvas(Rect(imgSize.width, 0, imgSize.width, imgSize.height));

    viewL = imread("../../test/test_left.jpg", 1);
    viewR = imread("../../test/test_right.jpg", 1);
    viewL.copyTo(canL);
    viewR.copyTo(canR);
    cout << "Done!" << endl;
    imshow("Canvas", canvas);
    waitKey(1000);

    /* stereoRectify */
    Mat mapL[2], mapR[2], recViewL, recViewR;
    initUndistortRectifyMap(interMatL, disCoeL, RL, PL, imgSize, CV_16SC2,
                            mapL[0], mapL[1]);
    initUndistortRectifyMap(interMatR, disCoeR, RR, PR, imgSize, CV_16SC2,
                            mapR[0], mapR[1]);
    remap(viewL, recViewL, mapL[0], mapL[1], INTER_LINEAR);
    imshow("remap", recViewL);
    waitKey(1);

    remap(viewR, recViewR, mapR[0], mapR[1], INTER_LINEAR);
    recViewL.copyTo(canL);
    recViewR.copyTo(canR);

    Mat beforeRec = imread("../../test/test_double.jpg");
    for (int j = 0; j <= canvas.rows; j += 16) {
        line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
    }
    for (int j = 0; j <= canvas.rows; j += 16) {
        line(beforeRec, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1,
             8);
    }
    cout << "stereo rectify done" << endl;

    imshow("Before rectified", beforeRec);
    imshow("After rectified", canvas);

    waitKey(1000);

    return 0;
}