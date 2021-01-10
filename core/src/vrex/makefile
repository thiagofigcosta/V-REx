FORMAT=.cpp
CXX=g++
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SRC=$(wildcard *$(FORMAT)) $(wildcard slide/*$(FORMAT))
OBJS=$(wildcard obj/*.o)
EXEC=bin/vrex
CFLAGS=-c --std=c++11 -Wall -fno-strict-aliasing -O3 -fopenmp -mtune=intel -mavx2 -I /usr/local/include/mongocxx/v_noabi/ -I /usr/local/include/bsoncxx/v_noabi/ -I /usr/local/include/libmongoc-1.0 -I /usr/local/include/libbson-1.0 -D BOOST_STACKTRACE_USE_BACKTRACE
LDFLAGS=-L /usr/local/lib -lmongocxx -lbsoncxx -ldl -lbacktrace -fPIC -fopenmp 

all: compileLinkAndRun

.SILENT compile:
	$(CXX) $(CFLAGS) $(SRC)
	if [ ! -d "obj" ]; then mkdir obj; fi
	@mv *.o obj/

link:
	if [ ! -d "bin" ]; then mkdir bin; fi
	$(CXX) $(OBJS) -o $(EXEC) $(LDFLAGS)

run:
	./$(EXEC)


compileAndLink: compile link

compileLinkAndRun: compile link run

linkAndRun: link run

clean:
	rm -rf *.o obj/*.o $(EXEC)