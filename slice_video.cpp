#include "opencv2/opencv.hpp"
#include <string>
#include "params_parser.h"
#include <boost/filesystem.hpp>
using namespace cv;

int main(int argc, char **argv) {
    int framecount = 0;
    paramsInput params;
    params.readFile(argv[1]);
    bool batch = params.getValue("BATCH", 0);
    const double interval = params.getValue("INTERVAL", -9001);
    std::string folder = params.getString("SAVEFOLDER", "sliced_frames");
    if (!batch) {
        std::string filename = params.getString("FILENAME", "");
        cv::VideoCapture capture(filename);
        int step = 0;
        
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
    }
    else {
        std::string prefix = params.getString("PREFIX", "negative");
        int first = params.getValue("FIRST", 1);
        int last  = params.getValue("LAST", 16);
        std::string extension = params.getString("EXTENSION", ".mp4");
        for (int i = first; i<=last; i++) {
            std::string filename  = prefix+std::to_string(i)+ extension;
            cv::VideoCapture capture(filename);
            int step = 0;
        
            while(capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT)-1) {
            //we can loop the video by re-opening the capture every time the video reaches its last frame

            if (!capture.isOpened()) {
                std::cout << "ERROR ACQUIRING VIDEO FEED\n";
                getchar();
                return -1;
            }
            Mat frame;
            capture >> frame;
            //imshow("Video", frame);
            if (step == interval) {
                imwrite(folder+std::to_string(framecount)+".bmp", frame);
                step = 0;
            }
            else step++;
            framecount++;
            //waitKey(10);
            }
        capture.release();

        }
    }
    return 0;
}
