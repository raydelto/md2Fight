UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS = -lGL -lGLEW -lglfw -ldl
	INCLUDES=-I ./includes
endif
ifeq ($(UNAME_S),Darwin)
FRAMEWORKS=-framework OpenGL -framework Cocoa -framework IOKit

LIBS= -L/System/Library/Frameworks \
	  -L/opt/glfw/lib-arm64 \
	  -lglfw3
INCLUDES=-I./includes -I/opt/glfw/include
SRC=third-party-source-code/glad.c
OBJ=bin/glad.o
endif

SRC += src/Tga.cpp \
	   src/Md2.cpp \
	   src/main.cpp


OBJ += 	bin/Tga.o \
		bin/Md2.o \
		bin/main.o

WARNINGS=-w

FLAGS=-std=c++11

all:
	g++ -c $(SRC) $(INCLUDES) $(WARNINGS) $(FLAGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	g++ $(OBJ) $(FRAMEWORKS) $(LIBS) $(INCLUDES) -o bin/main $(WARNINGS) $(FLAGS)
