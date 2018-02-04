#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <vector>



class Cube {
public:
    enum class filterMode {TOHSV, TOGRAY};
private:
    cv::Mat mWorkingFrame, mOriginalFrame, mBinaryImage;
    cv::Mat colorFilter(cv::Mat frame, filterMode mode);
    std::vector<cv::KeyPoint> blobDetect(cv::Mat frame);
public:
    Cube(cv::Mat frame);
    int getPosition();
    cv::Mat showFrame();
};
