LIBS    = -lm -lGL -lGLU -lglut -lBox2D -lconfig++ -lSOIL
FLAGS = -Wall -std=c++11

all : build/app

clean :
	rm -r build

run : build/app
	cd build && ./app

build :
	mkdir build 
screenshots:
	mkdir screenshots
build/main.o : main.cpp
	$(CXX) -c main.cpp -o build/main.o

build/configs.o : components/configs.cpp
	$(CXX) -c components/configs.cpp -o build/configs.o

build/triangle.o : components/triangle.cpp
	$(CXX) -c components/triangle.cpp -o build/triangle.o

build/circle.o : components/circle.cpp
	$(CXX) -c components/circle.cpp -o build/circle.o

build/rectangle.o : components/rectangle.cpp
	$(CXX) -c components/rectangle.cpp -o build/rectangle.o

build/textures.o : components/textures.cpp
	$(CXX) -c components/textures.cpp -o build/textures.o

build/app : build screenshots build/main.o build/triangle.o build/circle.o build/rectangle.o build/configs.o build/textures.o
	cd build &&	$(CXX) main.o triangle.o circle.o rectangle.o configs.o textures.o $(LIBS) -o app
