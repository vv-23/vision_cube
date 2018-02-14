#include "Cube.h"

cv::Size threshold;

Cube::Cube(cv::Mat frame, const std::string& filename) {
    frame.copyTo(mWorkingFrame);
    frame.copyTo(mOriginalFrame);
    mParams.readFile(filename);
}
Cube::Cube(cv::Mat frame, const paramsInput& params) {
    mParams = params;
    frame.copyTo(mWorkingFrame);
    frame.copyTo(mOriginalFrame);
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
        mask.copyTo(ret);
        //cv::cvtColor(ret, ret, CV_BGR2GRAY);
    }
    return ret;
}

std::vector<cv::KeyPoint> Cube::blobDetect(cv::Mat frame) {
    cv::SimpleBlobDetector::Params params;
    std::vector<cv::KeyPoint> keypoints;                                                             
    
    params.filterByColor = true;
    params.blobColor = 0;
    
    // Change thresholds
    params.minThreshold = mParams.getValue("BLOB_MIN_THRESHOLD", -9001);
    params.maxThreshold = mParams.getValue("BLOB_MAX_THRESHOLD", -9001);
    // Filter by Area.
    params.filterByArea = /*(mParams.getValue("FILTER_BY_AREA",-9001)==1)*/ true;
    std::cout << params.filterByArea;
    params.minArea = mParams.getValue("MIN_AREA",-9001);
    params.maxArea = mParams.getValue("MAX_AREA", -9001);
    
    // Filter by Circularity
    /*params.filterByCircularity = true;
    params.minCircularity = 0.6;
    params.maxCircularity = 0.9;*/
    
    params.minDistBetweenBlobs = mParams.getValue("MIN_DISTANCE_BETWEEN_BLOBS",-9001);
    // Filter by Convexity
    //std::cout << params.minDistBetweenBlobs << std::endl;
    
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
    detector->detect(frame, keypoints);
    return keypoints;
}

std::vector<cv::Rect> Cube::Rects(cv::Mat frame) {
    cv::Mat src;
    frame.copyTo(src);
    std::vector<cv::Rect> recs;
    cv::Mat contours_img(cv::Size(mOriginalFrame.cols, mOriginalFrame.rows), CV_8U), blurred;
    std::vector<std::vector<cv::Point>> edges;
    cv::blur(src, blurred, cv::Size(3,3));
    cv::Mat canny_output;
    std::vector<cv::Vec4i> hierachy;
    cv::Canny(src, canny_output, mParams.getValue("CANNY_LOW_THRESHOLD", -9001), mParams.getValue("CANNY_HIGH_THRESHOLD", -9001));
    cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8U);
    cv::findContours(canny_output, edges, hierachy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    cv::Mat rectangles = cv::Mat(cv::Size(1280, 720), CV_8U);
    for (int i = 0; i<edges.size(); i++) {
        if (edges[i].size() > mParams.getValue("CONTOURS_MIN_LENGTH", -9001)) {
            cv::drawContours(drawing, edges, i, cv::Scalar(255), 1, 8, hierachy, 0);
            cv::Rect r = cv::boundingRect(edges[i]);
            double minArea = mParams.getValue("RECTANGLE_MIN_AREA", -9001);
            double maxArea = mParams.getValue("RECTANGLE_MAX_AREA", -9001);
            double minAspectRatio = mParams.getValue("RECTANGE_ASPECT_RATIO_MIN", -9001);
            double maxAspectRatio = mParams.getValue("RECTANGE_ASPECT_RATIO_MAX", -9001);
            double aspectRatio = r.width/r.height;
            bool valid = (r.area() > minArea) && (r.area() < maxArea) && (aspectRatio > minAspectRatio) && (aspectRatio < maxAspectRatio);
            if (valid) {
                recs.push_back(r);
                /*cv::rectangle(rectangles, r, cv::Scalar(255,255,255), 1, 8, 0);*/
            }
        }
    }
    //cv::imshow("Contours", drawing);
    cv::imshow("Rectangles", rectangles);
    return recs;
}

int Cube::getPosition(detectionMode mode) {
    if (mode == detectionMode::BLOB) {
        cv::Mat filtered;
        cv::bitwise_not(colorFilter(mOriginalFrame, filterMode::TOHSV), filtered);
        filtered.copyTo(mWorkingFrame);
        int ret;
        std::vector<cv::KeyPoint> keypoints = blobDetect(filtered);
        cv::Mat im_with_keypoints;
        cv::drawKeypoints(mWorkingFrame, keypoints, im_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        cv::imshow("Debug", im_with_keypoints);
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
    if (mode==detectionMode::CONTOURS) {
        cv::Mat filtered = colorFilter(mOriginalFrame, filterMode::TOHSV);
        filtered.copyTo(mWorkingFrame);
        int ret;
        Rects(mWorkingFrame);
        return ret;
    }
}

cv::Mat Cube::showFrame() {
    return mOriginalFrame;
}
