#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Tga
{
public:
  Tga(char *tN);
  void LoadTexture(char *filename);
  unsigned int GetWidth() const { return m_width; }
  unsigned int GetHeight() const { return m_height; }
  GLubyte* GetData() const { return m_data; }

private:
  unsigned int m_width;
  unsigned int m_height;
  GLubyte *m_data;
};
