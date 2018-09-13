SRC=src/targaClass.cpp \
	src/md2Class.cpp \
	src/main.cpp \
	third-party-source-code/glad.c

OBJ=bin/targaClass.o \
	bin/md2Class.o \
	bin/main.o \
	bin/glad.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS = -lGL -lGLU -lglfw -ldl
	INCLUDES=-I ./include \
		-I /usr/include	
endif
ifeq ($(UNAME_S),Darwin)
FRAMEWORKS=-framework OpenGL

LIBS= -L/System/Library/Frameworks \
	  -lglfw
INCLUDES=-I./includes
endif

WARNINGS=-Wall

all:
	g++ -c $(SRC) $(INCLUDES) $(WARNINGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	g++ $(OBJ) $(FRAMEWORKS) $(LIBS) $(INCLUDES) -o bin/main $(WARNINGS)
