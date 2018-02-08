#include "Cube.h"
#include <iostream>


int main(int argc, char **argv) {
    std::string params_file(argv[1]);
    std::string video_file(argv[2]);
    paramsInput params;
    params.readFile(params_file);
    while (1) {
      cv::VideoCapture capture(video_file);
      while(capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT)-1) {
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
        if (frame.empty()) {
          capture.release();
          capture = cv::VideoCapture(video_file);    
        }
          
        Cube cube(frame, params);
        //std::cout << cube.getPosition() << std::endl;
        cube.getPosition(Cube::detectionMode::CONTOURS);
        //cv::imshow("Cube", cube.showFrame());
        cv::imshow("Camera", frame);
        //cv::imshow("Video", frame);
        cv::waitKey(10);
        
      }
      capture.release();
    }
	


  
    return 0;
}