CXX=g++
CFLAGS=-c --std=c++11 -Wall
LDFLAGS=
SOURCES=file_utils.cpp
OBJECTS=file_utils.o
EXECUTABLE=file_utils

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CXX) $(CFLAGS) -c $(SOURCES)