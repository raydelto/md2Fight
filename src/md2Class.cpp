#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "targaClass.cpp"

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

namespace md2model
{
	typedef struct
	{
		float point[3];
	} vector;
}

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
	md2model::vector *pointList;
	float x, y, z;
	float nextX, nextY, nextZ;
	float radius;
	float dist_to_player;
	int state;
	float speed;
} modData;

class md2
{
  private:
	GLuint textu;
	modData *mod;

	void Normal(float *p1, float *p2, float *p3)
	{
		float a[3], b[3], result[3];
		float length;

		a[0] = p1[0] - p2[0];
		a[1] = p1[1] - p2[1];
		a[2] = p1[2] - p2[2];

		b[0] = p1[0] - p3[0];
		b[1] = p1[1] - p3[1];
		b[2] = p1[2] - p3[2];

		result[0] = a[1] * b[2] - b[1] * a[2];
		result[1] = b[0] * a[2] - a[0] * b[2];
		result[2] = a[0] * b[1] - b[0] * a[1];

		length = sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2]);

		glNormal3f(result[0] / length, result[1] / length, result[2] / length);
	}

	void load(char *filenme)
	{
		FILE *fp;
		int length;

		char *buffer;

		header *head;
		textindx *stPtr;

		frame *fra;
		md2model::vector *pntlst;
		mesh *triIndex, *bufIndexPtr;

		fp = fopen(filenme, "rb");
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		buffer = (char *)malloc(length + 1);
		fread(buffer, sizeof(char), length, fp);

		head = (header *)buffer;
		mod = (modData *)malloc(sizeof(modData));

		mod->pointList = (md2model::vector *)malloc(sizeof(md2model::vector) * head->vNum * head->Number_Of_Frames);
		mod->numPoints = head->vNum;
		mod->numFrames = head->Number_Of_Frames;
		mod->frameSize = head->framesize;

		for (int count = 0; count < head->Number_Of_Frames; count++)
		{
			fra = (frame *)&buffer[head->offsetFrames + head->framesize * count];
			pntlst = (md2model::vector *)&mod->pointList[head->vNum * count];
			for (int count2 = 0; count2 < head->vNum; count2++)
			{
				pntlst[count2].point[0] = fra->scale[0] * fra->fp[count2].v[0] + fra->translate[0];
				pntlst[count2].point[1] = fra->scale[1] * fra->fp[count2].v[1] + fra->translate[1];
				pntlst[count2].point[2] = fra->scale[2] * fra->fp[count2].v[2] + fra->translate[2];
			}
		}

		mod->st = (textcoord *)malloc(sizeof(textcoord) * head->tNum);
		mod->numST = head->tNum;
		stPtr = (textindx *)&buffer[head->offsetTCoord];

		for (int count = 0; count < head->tNum; count++)
		{
			mod->st[count].s = (float)stPtr[count].s / (float)head->twidth;
			mod->st[count].t = (float)stPtr[count].t / (float)head->theight;
		}

		triIndex = (mesh *)malloc(sizeof(mesh) * head->fNum);
		mod->triIndx = triIndex;
		mod->numTriangles = head->fNum;
		bufIndexPtr = (mesh *)&buffer[head->offsetIndx];

		for (int count = 0; count < head->Number_Of_Frames; count++)
		{
			for (int count2 = 0; count2 < head->fNum; count2++)
			{
				triIndex[count2].meshIndex[0] = bufIndexPtr[count2].meshIndex[0];
				triIndex[count2].meshIndex[1] = bufIndexPtr[count2].meshIndex[1];
				triIndex[count2].meshIndex[2] = bufIndexPtr[count2].meshIndex[2];

				triIndex[count2].stIndex[0] = bufIndexPtr[count2].stIndex[0];
				triIndex[count2].stIndex[1] = bufIndexPtr[count2].stIndex[1];
				triIndex[count2].stIndex[2] = bufIndexPtr[count2].stIndex[2];
			}
		}

		mod->currentFrame = 0;
		mod->nextFrame = 1;
		mod->interpol = 0.0;

		fclose(fp);
	}

  public:
	int done = 0;
	char *textN;

	md2(char *fName, char *textName)
	{
		load(fName);
		textN = textName;
	}

	void Draw(int startFrame, int endFrame)
	{
		if (!done)
		{
			tga texture(textN);
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &textu);
			glBindTexture(GL_TEXTURE_2D, textu);
			// glGenerateMipmap(GL_TEXTURE_2D);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			done = 1;
		}

		md2model::vector *stFrame;
		md2model::vector *cdFrame;
		md2model::vector vector[3];

		float x1, y1, z1;
		float x2, y2, z2;

		if (mod->interpol == 0.0)
		{
			mod->currentFrame = startFrame;
		}

		if (mod->interpol >= 1.0)
		{
			mod->interpol = 0.0f;
			mod->currentFrame++;

			if (mod->currentFrame >= endFrame)
			{
				mod->currentFrame = startFrame;
			}

			mod->nextFrame = mod->currentFrame + 1;

			if (mod->nextFrame >= endFrame)
			{
				mod->nextFrame = startFrame;
			}
		}

		stFrame = &mod->pointList[mod->numPoints * mod->currentFrame];
		cdFrame = &mod->pointList[mod->numPoints * mod->nextFrame];

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textu);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glBegin(GL_TRIANGLES);
		for (int index = 0; index < mod->numTriangles; index++)
		{
			x1 = stFrame[mod->triIndx[index].meshIndex[0]].point[0];
			y1 = stFrame[mod->triIndx[index].meshIndex[0]].point[1];
			z1 = stFrame[mod->triIndx[index].meshIndex[0]].point[2];

			x2 = cdFrame[mod->triIndx[index].meshIndex[0]].point[0];
			y2 = cdFrame[mod->triIndx[index].meshIndex[0]].point[1];
			z2 = cdFrame[mod->triIndx[index].meshIndex[0]].point[2];

			vector[0].point[0] = x1 + mod->interpol * (x2 - x1);
			vector[0].point[1] = y1 + mod->interpol * (y2 - y1);
			vector[0].point[2] = z1 + mod->interpol * (z2 - z1);

			x1 = stFrame[mod->triIndx[index].meshIndex[2]].point[0];
			y1 = stFrame[mod->triIndx[index].meshIndex[2]].point[1];
			z1 = stFrame[mod->triIndx[index].meshIndex[2]].point[2];

			x2 = cdFrame[mod->triIndx[index].meshIndex[2]].point[0];
			y2 = cdFrame[mod->triIndx[index].meshIndex[2]].point[1];
			z2 = cdFrame[mod->triIndx[index].meshIndex[2]].point[2];

			vector[2].point[0] = x1 + mod->interpol * (x2 - x1);
			vector[2].point[1] = y1 + mod->interpol * (y2 - y1);
			vector[2].point[2] = z1 + mod->interpol * (z2 - z1);

			x1 = stFrame[mod->triIndx[index].meshIndex[1]].point[0];
			y1 = stFrame[mod->triIndx[index].meshIndex[1]].point[1];
			z1 = stFrame[mod->triIndx[index].meshIndex[1]].point[2];

			x2 = cdFrame[mod->triIndx[index].meshIndex[1]].point[0];
			y2 = cdFrame[mod->triIndx[index].meshIndex[1]].point[1];
			z2 = cdFrame[mod->triIndx[index].meshIndex[1]].point[2];

			vector[1].point[0] = x1 + mod->interpol * (x2 - x1);
			vector[1].point[1] = y1 + mod->interpol * (y2 - y1);
			vector[1].point[2] = z1 + mod->interpol * (z2 - z1);

			Normal(vector[0].point, vector[2].point, vector[1].point);

			glTexCoord2f(mod->st[mod->triIndx[index].stIndex[0]].s, mod->st[mod->triIndx[index].stIndex[0]].t);
			glVertex3fv(vector[0].point);

			glTexCoord2f(mod->st[mod->triIndx[index].stIndex[2]].s, mod->st[mod->triIndx[index].stIndex[2]].t);
			glVertex3fv(vector[2].point);

			glTexCoord2f(mod->st[mod->triIndx[index].stIndex[1]].s, mod->st[mod->triIndx[index].stIndex[1]].t);
			glVertex3fv(vector[1].point);
		}
		glEnd();
		mod->interpol += 0.1;
	}
};
