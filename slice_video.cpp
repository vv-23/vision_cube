#include "opencv2/opencv.hpp"
#include <string>
#include "params_parser.h"
using namespace cv;

int main(int argc, char **argv) {
    paramsInput params;
    params.readFile(std::string(argv[1]));
    const double interval = params.getValue("INTERVAL", -9001);
    
    return 0;
}