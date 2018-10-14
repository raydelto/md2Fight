#include "targaClass.cpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct
{
	int id;
	int version;
	int twidth;
	int theight;
	int framesize;
	int textures;
	int vNum;
	int tNum;
	int fNum;
	int numGLcmds;
	int Number_Of_Frames;
	int offsetSkins;
	int offsetTCoord;
	int offsetIndx;
	int offsetFrames;
	int offsetGLcmds;
	int offsetEnd;
} header;

typedef struct
{
	float s;
	float t;
} textcoord;

typedef struct
{
	short s;
	short t;
} textindx;

typedef struct
{
	unsigned char v[3];
	unsigned char normalIndex;
} framePoint_t;

typedef struct
{
	float scale[3];
	float translate[3];
	char name[16];
	framePoint_t fp[1];
} frame;

typedef struct
{
	unsigned short meshIndex[3];
	unsigned short stIndex[3];
} mesh;

typedef struct
{
	float point[3];
} vector;

typedef struct
{
	int numFrames;
	int numPoints;
	int numTriangles;
	int numST;
	int frameSize;
	int twidth;
	int theight;
	int currentFrame;
	int nextFrame;
	float interpol;
	mesh *triIndx;
	textcoord *st;
	std::vector *pointList;
	float x, y, z;
	float nextX, nextY, nextZ;
	float radius;
	float dist_to_player;
	int state;
	float speed;
} modData;

class Md2Model
{
  private:
	GLuint textu;
	modData *mod;
	void Normal(float *p1, float *p2, float *p3);
	void load(char *filenme);

  public:
	int done;
	char *textN;

	Md2Model(char *fName, char *textName);
	void Draw(int startFrame, int endFrame);
};
