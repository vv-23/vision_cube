LIBS =  `pkg-config --libs opencv libv4l2`
CFLAGS = `pkg-config --cflags opencv libv4l2`
CAMERATEST = `Camera_test.exe`
CUBE = `Cube.exe`

$(CUBE):
	g++ -g -o cube.exe main.cpp Cube.cpp params_parser.cpp $(CFLAGS) $(LIBS) 
$(CAMERATEST):
	g++ -g -o Camera_test.exe Camera_test.cpp params_parser.cpp $(CFLAGS) $(LIBS)