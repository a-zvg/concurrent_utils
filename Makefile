CXXFLAGS=-g -std=c++14 -Wall -Wextra

all:
	g++ $(CXXFLAGS) main3.cpp -lpthread
