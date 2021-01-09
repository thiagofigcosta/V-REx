CXX=g++
CFLAGS=-c --std=c++11 -Wall -I /usr/local/include/mongocxx/v_noabi/ -I /usr/local/include/bsoncxx/v_noabi/ -I /usr/local/include/libmongoc-1.0 -I /usr/local/include/libbson-1.0
LDFLAGS=-L /usr/local/lib -lmongocxx -lbsoncxx
SOURCES=mongo_connector.cpp
OBJECTS=mongo_connector.o
EXECUTABLE=mongo_connector

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CXX) $(CFLAGS) -c $(SOURCES)