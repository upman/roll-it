LIBS    = -lm -lGL -lGLU -lglut -lBox2D

all : build/app

clean :
	rm -r build

run : build/app
	cd build && ./app

build :
	mkdir build

build/main.o : main.cpp build
	$(CXX) -c main.cpp -o build/main.o

build/triangle.o : components/triangle.cpp
	$(CXX) -c components/triangle.cpp -o build/triangle.o

build/circle.o : components/circle.cpp
	$(CXX) -c components/circle.cpp -o build/circle.o

build/rectangle.o : components/rectangle.cpp
	$(CXX) -c components/rectangle.cpp -o build/rectangle.o

build/app : build/main.o build/triangle.o build/circle.o build/rectangle.o build
	cd build &&	$(CXX) main.o triangle.o circle.o rectangle.o $(LIBS) -o app
