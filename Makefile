LIBS =  `pkg-config --libs opencv`
CFLAGS = `pkg-config --cflags opencv`
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
record.exe: record.o params_parser.o
	g++ -g -o record.exe record.o params_parser.o $(LIBS) $(CFAGS)
record.o: record.cpp
	g++ -g -c record.cpp -o record.o $(LIBS) $(CFLAGS)
slice.exe: slice.o
	g++ -g -o slice.exe slice.o params_parser.o $(LIBS) $(CFLAGS)
slice.o: slice_video.cpp
	g++ -g -c slice_video.cpp -o slice.o $(LIBS) $(CFLAGS)
clean:
	rm *.o *.exe
