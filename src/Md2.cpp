#include "Md2.h"

Md2::Md2(char *fName, char *textName) : m_textureName(textName), m_initialized(false)
{
	Load(fName);
}

void Md2::Draw(int startFrame, int endFrame)
{
	if (!m_initialized)
	{
		Tga texture(m_textureName);
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.GetWidth(), texture.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0); 

		m_initialized = true;
	}
	glBindTexture(GL_TEXTURE_2D, m_texture);

	md2model::vector *stFrame;

	if (m_model->interpol == 0.0)
	{
		m_model->currentFrame = startFrame;
	}

	if (m_model->interpol >= 1.0)
	{
		m_model->interpol = 0.0f;
		m_model->currentFrame++;

		if (m_model->currentFrame >= endFrame)
		{
			m_model->currentFrame = startFrame;
		}

		m_model->nextFrame = m_model->currentFrame + 1;

		if (m_model->nextFrame >= endFrame)
		{
			m_model->nextFrame = startFrame;
		}
	}

	stFrame = &m_model->pointList[m_model->numPoints * m_model->currentFrame];

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	for (int index = 0; index < m_model->numTriangles; index++)
	{

		glTexCoord2f(m_model->st[m_model->triIndx[index].stIndex[0]].s, m_model->st[m_model->triIndx[index].stIndex[0]].t);
		glVertex3fv(stFrame[m_model->triIndx[index].meshIndex[0]].point);

		glTexCoord2f(m_model->st[m_model->triIndx[index].stIndex[2]].s, m_model->st[m_model->triIndx[index].stIndex[2]].t);
		glVertex3fv(stFrame[m_model->triIndx[index].meshIndex[2]].point);

		glTexCoord2f(m_model->st[m_model->triIndx[index].stIndex[1]].s, m_model->st[m_model->triIndx[index].stIndex[1]].t);
		glVertex3fv(stFrame[m_model->triIndx[index].meshIndex[1]].point);
	}
	glEnd();
	m_model->interpol += 0.1;
}

void Md2::Normal(float *p1, float *p2, float *p3)
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

void Md2::Load(char *filenme)
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
	m_model = (modData *)malloc(sizeof(modData));

	m_model->pointList = (md2model::vector *)malloc(sizeof(md2model::vector) * head->vNum * head->Number_Of_Frames);
	m_model->numPoints = head->vNum;
	m_model->numFrames = head->Number_Of_Frames;
	m_model->frameSize = head->framesize;

	for (int count = 0; count < head->Number_Of_Frames; count++)
	{
		fra = (frame *)&buffer[head->offsetFrames + head->framesize * count];
		pntlst = (md2model::vector *)&m_model->pointList[head->vNum * count];
		for (int count2 = 0; count2 < head->vNum; count2++)
		{
			pntlst[count2].point[0] = fra->scale[0] * fra->fp[count2].v[0] + fra->translate[0];
			pntlst[count2].point[1] = fra->scale[1] * fra->fp[count2].v[1] + fra->translate[1];
			pntlst[count2].point[2] = fra->scale[2] * fra->fp[count2].v[2] + fra->translate[2];
		}
	}

	m_model->st = (textcoord *)malloc(sizeof(textcoord) * head->tNum);
	m_model->numST = head->tNum;
	stPtr = (textindx *)&buffer[head->offsetTCoord];

	for (int count = 0; count < head->tNum; count++)
	{
		m_model->st[count].s = (float)stPtr[count].s / (float)head->twidth;
		m_model->st[count].t = (float)stPtr[count].t / (float)head->theight;
	}

	triIndex = (mesh *)malloc(sizeof(mesh) * head->fNum);
	m_model->triIndx = triIndex;
	m_model->numTriangles = head->fNum;
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

	m_model->currentFrame = 0;
	m_model->nextFrame = 1;
	m_model->interpol = 0.0;

	fclose(fp);
}
