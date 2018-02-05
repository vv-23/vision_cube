#include "opencv2/opencv.hpp"
#include <cstdlib>
#include "params_parser.h"
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <iostream>
using namespace cv;


int main(int argc, char **argv) {
    paramsInput params;
    int descriptor = v4l2_open("/dev/video0", O_RDWR);
    v4l2_control c;
  c.id = V4L2_CID_EXPOSURE_AUTO;
  c.value = 1;	
  if(v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
    std::cout << "success";
  c.id = V4L2_CID_EXPOSURE_AUTO_PRIORITY;
  c.value = 0;
  if (v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
    std::cout << "success";

  c.id = V4L2_CID_EXPOSURE_ABSOLUTE;
  c.value = std::atof(argv[1]);
  if (v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
    std::cout << "success";
    VideoCapture capture(0);
    while (1) {
        if (!capture.isOpened()) std::cout << "ERROR\n";
        Mat frame;
        capture >> frame;
        imshow("Camera", frame);
        waitKey(5);
    }
    capture.release();
    return 0;
}