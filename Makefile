LIBS =  `pkg-config --libs opencv libv4l2`
CFLAGS = `pkg-config --cflags opencv libv4l2`
CAMERATEST = `Camera_test.exe`
CUBE = `Cube.exe`

cube.exe: main.o Cube.o params_parser.o
	g++ -g -o cube.exe main.o Cube.o params_parser.o $(LIBS) $(CFLAGS)
main.o: main.cpp
	g++ -g -c main.cpp -o main.o $(LIBS) $(CFLAGS) 
Cube.o: Cube.cpp
	g++ -g -c Cube.cpp -o Cube.o $(LIBS) $(CFLAGS)
params_parser.o: params_parser.cpp
	g++ -g -c params_parser.cpp -o params_parser.o
clean:
	rm *.o *.exe
