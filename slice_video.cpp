#include "opencv2/opencv.hpp"
#include <string>
#include "params_parser.h"
#include <boost/filesystem.hpp>
using namespace cv;

int main(int argc, char **argv) {
    paramsInput params;
    params.readFile(std::string(argv[1]));
    const double interval = params.getValue("INTERVAL", -9001);
    std::string filename = params.getString("FILENAME", "");
    std::string folder = params.getString("SAVEFOLDER", "sliced_frames");
    cv::VideoCapture capture(filename);
    int step = 0;
    int framecount = 0;
    while(capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT)-1) {
        //we can loop the video by re-opening the capture every time the video reaches its last frame

        if (!capture.isOpened()) {
            std::cout << "ERROR ACQUIRING VIDEO FEED\n";
            getchar();
            return -1;
        }
        Mat frame;
        capture >> frame;
        imshow("Video", frame);
        if (step == interval) {
            imwrite(folder+std::to_string(framecount)+".jpeg", frame);
            step = 0;
        }
        else step++;
        framecount++;
        //waitKey(10);
        }
      capture.release();
    return 0;
}