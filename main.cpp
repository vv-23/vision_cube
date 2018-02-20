#include "opencv2/opencv.hpp"
#include <cstdlib>
#include "params_parser.h"
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include "Cube.h"
using namespace cv;



int main(int argc, char **argv) {
  std::string params_file(argv[1]);
  paramsInput params;
  params.readFile(params_file);                 //Might needs improvement
  double exposure = params.getValue("CAMERA_EXPOSURE",-9001);
  std::stringstream ss;
  ss << "/dev/video" << std::to_string(int(params.getValue("CAMID", -9001)));
  int descriptor = v4l2_open(ss.str().c_str(), O_RDWR);
  v4l2_control c;
  c.id = V4L2_CID_EXPOSURE_AUTO;
  c.value = int(params.getValue("CAMERA_EXPOSURE_AUTO", -9001));	
  if(v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
    std::cout << "success";
  /*c.id = V4L2_CID_EXPOSURE_AUTO_PRIORITY;
  c.value = 0;
  if (v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
    std::cout << "success";*/

  c.id = V4L2_CID_EXPOSURE_ABSOLUTE;
  c.value = exposure;
  if (v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
    std::cout << "success";
  cv::VideoCapture capture(params.getValue("CAMID", 0));
  while (1) {
    if (!capture.isOpened()) {
      std::cout << "ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }
    cv::Mat frame;
    //read first frame
    capture >> frame;
    Cube cube(frame, params);
    cube.getPosition(Cube::detectionMode::BLOB);
    cv::imshow("Cube", cube.showFrame());
    //cv::imshow("Camera", frame);
    //cv::imshow("Video", frame);
    cv::waitKey(10);
  };
  capture.release();
  return 0;
}
