#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <vector>
#include <map>
#include "params_parser.h"



class Cube {
public:
    enum class filterMode {TOHSV, TOGRAY};
private:
    cv::Mat mWorkingFrame, mOriginalFrame, mBinaryImage;
    cv::Mat colorFilter(cv::Mat frame, filterMode mode);
    std::vector<cv::KeyPoint> blobDetect(cv::Mat frame);
    paramsInput mParams;
public:
    Cube(cv::Mat frame, const std::string& filename = "parameters.txt");
    int getPosition();
    cv::Mat showFrame();
};
