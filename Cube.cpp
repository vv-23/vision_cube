#include "Cube.h"

cv::Size threshold;

Cube::Cube(cv::Mat frame) {
    mWorkingFrame = frame;
    mOriginalFrame = frame;
}

cv::Mat Cube::colorFilter(cv::Mat frame, Cube::filterMode mode) {
    cv::Mat ret;
    if (mode==filterMode::TOGRAY) {
        cv::Scalar lowThreshold(0), highThreshold(255);
        cv::Mat grayFrame, mask;
        cv::cvtColor(frame, grayFrame, CV_BGR2GRAY);
        cv::inRange(grayFrame, lowThreshold, highThreshold, mask);
        cv::bitwise_and(frame, frame, ret, mask);
    }
    if (mode==filterMode::TOHSV) {
        cv::Scalar lowThreshold(20,100,80), highThreshold(30,255,230);
        cv::Mat hsvFrame, mask;
        cv::cvtColor(frame, hsvFrame, CV_BGR2HSV);
        cv::inRange(hsvFrame, lowThreshold, highThreshold, mask);
        mBinaryImage = mask;
        cv::bitwise_and(frame, frame, ret, mask);
        cv::cvtColor(ret, ret, CV_BGR2GRAY);
    }
    return ret;
}

std::vector<cv::KeyPoint> Cube::blobDetect(cv::Mat frame) {
    cv::SimpleBlobDetector::Params params;
    std::vector<cv::KeyPoint> keypoints;                                                             
    
    /*params.filterByColor = true;
    params.blobColor = 0;*/
    
    // Change thresholds
    params.minThreshold = 80;
    params.maxThreshold = 255;
    
    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 10;
    
    // Filter by Circularity
    /*params.filterByCircularity = true;
    params.minCircularity = 0.68;
    params.maxCircularity = 0.8;*/
    
    // Filter by Convexity
    std::cout << params.minDistBetweenBlobs << std::endl;
    
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
    detector->detect(frame, keypoints);
    return keypoints;
}

int Cube::getPosition() {
    mWorkingFrame = colorFilter(mOriginalFrame, filterMode::TOHSV);
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