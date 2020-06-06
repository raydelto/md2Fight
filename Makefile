linux:
	g++ main.cpp -o main -Wall -lSDL -lGL -lGLU -lSDL_mixer
cygwin:
	g++ main.cpp -o main -lSDL -lopengl32 -lglu32 -lm -lSDL_mixer
clean:
	rm -rf *.o *.dump *.exe *.*~
