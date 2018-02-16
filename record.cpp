#include "opencv2/opencv.hpp"
#include <iostream>
#include "params_parser.h"
#include <string>

using namespace cv;


int main(int argc, char **argv) {
  std::string filename(argv[1]);
  paramsInput params;
  params.readFile(filename);
  const double INTERVAL = params.getValue("INTERVAL", -9001);
  VideoCapture capture(params.getValue("CAMID", -9001));
  int step = 0;
  int framecount = 0;
  std::stringstream ss;
  ss << "frames/";
  while(1) {
    ss.str("frames/");
    if(!capture.isOpened()) {
      std::cout << "ERR OPENING CAMERA\n";
    }
    Mat frame;
    capture >> frame;
    if (step == (int)(capture.get(CV_CAP_PROP_FPS)*INTERVAL)) {
      ss << framecount << ".jpeg";
      imwrite("frames/"+std::to_string(framecount)+".jpeg", frame);
      step = 0;
    }
    framecount++;
    step++;
  }
  return 0;
}
