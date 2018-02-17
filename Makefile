LIBS =  `pkg-config --libs opencv libv4l2`
HOME = /home/vi
BOOSTVER = 1_66_0
BOOSTPATH = $(HOME)/boost_$(BOOSTVER)
BOOSTLIBS = -lboost_filesystem -lboost_system
DEBUG = /usr/local/lib/libboost_filesystem.a /usr/local/lib/libboost_system.a
CFLAGS = `pkg-config --cflags opencv`

all: cube.exe slice.exe record.exe
cube.exe: main.o Cube.o params_parser.o
	g++ -g -o cube.exe main.o Cube.o params_parser.o $(LIBS) $(CFLAGS)
main.o: main.cpp
	g++ -g -c main.cpp -o main.o $(LIBS) $(CFLAGS) 
Cube.o: Cube.cpp
	g++ -g -c Cube.cpp -o Cube.o $(LIBS) $(CFLAGS)
record.exe: record.o params_parser.o
	g++ -g -o record.exe record.o params_parser.o $(LIBS) $(CFAGS)
record.o: record.cpp
	g++ -g -c record.cpp -o record.o $(LIBS) $(CFLAGS)
slice.exe: slice.o
	g++ -g -o slice.exe slice.o params_parser.o $(LIBS) $(CFLAGS) -L$(BOOSTPATH) $(BOOSTLIBS)
slice.o: slice_video.cpp
	g++ -g -c slice_video.cpp -o slice.o $(LIBS) $(CFLAGS) -L$(BOOSTPATH) $(BOOSTLIBS)
params_parser.o: params_parser.cpp
	g++ -g -c params_parser.cpp -o params_parser.o
clean:
	rm *.o *.exe
