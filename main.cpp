#include "opencv2/opencv.hpp"
#include <cstdlib>
#include "params_parser.h"
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <iostream>
#include "Cube.h"
using namespace cv;



int main(int argc, char **argv) {
    char *params_file  = argv[1];                 //Might needs improvement
    double exposure = std::atof(argv[2]);
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
  c.value = exposure;
  if (v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
    std::cout << "success";
    cv::VideoCapture capture(0);
    while (1) {
        if (!capture.isOpened()) {
          std::cout << "ERROR ACQUIRING VIDEO FEED\n";
          getchar();
          return -1;
        }
        cv::Mat frame;
        //read first frame
        capture >> frame;
        Cube cube(frame, params_file);
        cube.getPosition();
        cv::imshow("Cube", cube.showFrame());
        cv::imshow("Camera", frame);
        //cv::imshow("Video", frame);
        cv::waitKey(10);
    };
    capture.release();
    return 0;
}