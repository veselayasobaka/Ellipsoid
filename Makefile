.PHONY: all clean
	
all: ellipsoid
	
main.o: main.cpp
	g++  -std=c++11 -c -o  build/main.o main.cpp
ellipsoid: main.o
	g++ build/main.o -lGL -lGLU -lglut -o build/ellipsoid
clean:
	rm -f build/*
