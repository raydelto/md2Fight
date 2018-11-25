UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS = -lGL -lGLEW -lglfw -ldl
	INCLUDES=-I ./includes
endif
ifeq ($(UNAME_S),Darwin)
FRAMEWORKS=-framework OpenGL

LIBS= -L/System/Library/Frameworks \
	  -lglfw
INCLUDES=-I./includes
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
