#include "Cube.h"

cv::Size threshold;

Cube::Cube(cv::Mat frame, const std::string& filename) {
    frame.copyTo(mWorkingFrame);
    frame.copyTo(mOriginalFrame);
    mParams.readFile(filename);
}

cv::Mat Cube::colorFilter(cv::Mat frame, Cube::filterMode mode) {
    cv::Mat ret;
    if (mode==filterMode::TOGRAY) {
        cv::Scalar lowThreshold(mParams.getValue("GRAY_LOW",-9001)), highThreshold(mParams.getValue("GRAY_HIGH", -9001));
        cv::Mat grayFrame, mask;
        cv::cvtColor(frame, grayFrame, CV_BGR2GRAY);
        cv::inRange(grayFrame, lowThreshold, highThreshold, mask);
        cv::bitwise_and(frame, frame, ret, mask);
    }
    if (mode==filterMode::TOHSV) {
        cv::Scalar lowThreshold(mParams.getValue("H_LOW",-9001),mParams.getValue("S_LOW",-9001),mParams.getValue("V_LOW",-9001)), highThreshold(mParams.getValue("H_HIGH",-9001),mParams.getValue("S_HIGH",-9001),mParams.getValue("V_HIGH",-9001));
        cv::Mat hsvFrame, mask;
        cv::cvtColor(frame, hsvFrame, CV_BGR2HSV);
        cv::inRange(hsvFrame, lowThreshold, highThreshold, mask);
        mBinaryImage = mask;
        cv::bitwise_not(mask, ret);
        //cv::cvtColor(ret, ret, CV_BGR2GRAY);
    }
    return ret;
}

std::vector<cv::KeyPoint> Cube::blobDetect(cv::Mat frame) {
    cv::SimpleBlobDetector::Params params;
    std::vector<cv::KeyPoint> keypoints;                                                             
    
    /*params.filterByColor = true;
    params.blobColor = 0;*/
    
    // Change thresholds
    params.minThreshold = mParams.getValue("BLOB_MIN_THRESHOLD", -9001);
    params.maxThreshold = mParams.getValue("BLOB_MIN_THRESHOLD", -9001);
    
    // Filter by Area.
    params.filterByArea = mParams.getValue("FILTER_BY_AREA",-9001);
    params.minArea = mParams.getValue("MIN_AREA",-9001);
    
    // Filter by Circularity
    /*params.filterByCircularity = true;
    params.minCircularity = 0.68;
    params.maxCircularity = 0.8;*/
    
    params.minDistBetweenBlobs = mParams.getValue("MIN_DISTANCE_BETWEEN_BLOBS",-9001);
    // Filter by Convexity
    //std::cout << params.minDistBetweenBlobs << std::endl;
    
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
    detector->detect(frame, keypoints);
    return keypoints;
}

int Cube::getPosition() {
    cv::Mat filtered = colorFilter(mOriginalFrame, filterMode::TOHSV);
    filtered.copyTo(mWorkingFrame);
    int ret;
    std::vector<cv::KeyPoint> keypoints = blobDetect(mWorkingFrame);
    cv::drawKeypoints(mWorkingFrame, keypoints, mOriginalFrame, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    auto it = keypoints.begin();
    auto flag = it;
    int ymin = 9001;
    for (;it!=keypoints.end();it++) {
        if(it->pt.y < ymin) {
            flag = it;
            ymin = it->pt.y;
        }
    }
    return ret;
}

cv::Mat Cube::showFrame() {
    return mOriginalFrame;
}
