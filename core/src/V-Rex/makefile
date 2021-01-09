FORMAT=.cpp
CC=g++
JSC=~/.emscripten_bin/em++
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
WEBFILES=$(call rwildcard, res/, *)
SRC=$(wildcard scr/*$(FORMAT))
CLASSES=$(wildcard scr/Classes/*$(FORMAT))
LIBS=$(wildcard scr/Libs/*$(FORMAT))
SOIL=$(wildcard scr/Libs/soil/*.c)
CLIP=scr/Libs/clip/clip.cpp
OBJS=$(wildcard obj/*.o)
EXEC=bin/AdventureDiscrete
CFLAGS=-w -Wall -std=c++11
LDFLAGS= 
#LDFLAGSMAC=-lm -framework OpenGL -framework OpenAL -framework GLUT -framework CoreFoundation #-F/usr/lib/ #-framework libGLEW.2.0.0 (glew unused)

all:
	echo type web, compileAll, compile, link, linkMac, run, runMac, compileAndLink, compileAndLinkMac, compileLinkAndRun, compileLinkAndRunMac

web:
	$(JSC) -s LEGACY_GL_EMULATION=1 -s ALLOW_MEMORY_GROWTH=1 -std=c++11 -O3 $(SRC) $(CLASSES) $(LIBS) $(SOIL) $(CLIP) -o Web.html $(foreach var,$(WEBFILES),--preload-file $(var))
	@mv Web.* bin/

compileAll:
	$(CC) -c $(CFLAGS) $(SRC) $(CLASSES) $(LIBS) $(SOIL) $(CLIP)
	if [ ! -d "obj" ]; then mkdir obj; fi
	@mv *.o obj/

.SILENT compile:
	$(CC) -c $(CFLAGS) $(filter-out $@,$(MAKECMDGOALS))
	if [ ! -d "obj" ]; then mkdir obj; fi
	@mv *.o obj/

link:
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

linkMac:
	$(CC) $(OBJS) -o $(EXEC)MacOSX $(LDFLAGSMAC)

run:
	./$(EXEC)

runMac:
	./$(EXEC)MacOSX

compileAndLink: compileAll link

compileAndLinkMac: compileAll linkMac

compileLinkAndRun: compileAll link run

compileLinkAndRunMac: compileAll linkMac runMac

linkAndRun: link run

linkAndRunMac: linkMac runMac

clean:
	rm -rf *.o obj/*.o $(EXEC) Web.*