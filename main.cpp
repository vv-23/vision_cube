#include "Cube.h"
#include <iostream>


int main() {
    cv::VideoCapture capture(1);
    while (1) {
		
    //we can loop the video by re-opening the capture every time the video reaches its last frame

    if (!capture.isOpened()) {
      std::cout << "ERROR ACQUIRING VIDEO FEED\n";
      getchar();
      return -1;
    }
    //check if the video has reach its last frame.
    //we add '-1' because we are reading two frames from the video at a time.
    //if this is not included, we get a memory error!
    cv::Mat frame;
    //read first frame
    capture >> frame;
    Cube cube(frame);
    cube.getPosition();
    cv::imshow("Camera", cube.showFrame());
    cv::waitKey(10);
    
  }
	


  capture.release();
    return 0;
}