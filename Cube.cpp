#include "Cube.h"

cv::Size threshold;

Cube::Cube(cv::Mat frame) {
    mWorkingFrame = frame;
    mOriginalFrame = frame;
}

cv::Mat Cube::colorFilter(cv::Mat frame, Cube::filterMode mode) {

}

std::vector<cv::KeyPoint> blobDetect(cv::Mat frame) {
    
}