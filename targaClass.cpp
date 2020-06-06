#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

using namespace std;

class tga
{

	
public:
	GLubyte *data;
	unsigned int bpp;
	unsigned int width;
	unsigned int height;
	unsigned int id;
	
	tga(char *tN)
	{
		load_utgatex(tN);
	}
	
	FILE *fp;

	void load_utgatex(char *filename)
	{
		GLubyte uheader_format[12]={0,0,2,0,0,0,0,0,0,0,0,0};
		GLubyte header_to_compare[12];
		GLubyte attr[6];
		int s;
		unsigned int length;
		unsigned int temp;
		unsigned int fu;

		fp=fopen(filename, "rb");

		if(!fp)
		{
			printf("Could not open file %s\n", filename);
		} else {
			fread(header_to_compare, 1, sizeof(header_to_compare), fp);
			s=memcmp(uheader_format, header_to_compare, sizeof(uheader_format));
		
			if(s==0) 
			{
				fread(attr, 1, sizeof(attr), fp);
				width=attr[1] * 256 + attr[0];
				height=attr[3] * 256 + attr[2];
				bpp=attr[4]/8;
				length = width * height * bpp;
				data=(GLubyte *)malloc(length);
				if(!data)
				{
					printf("Could not allocate memory for the TGA texture\n");
				} else {
					fread(data, 1, length, fp); 
				}
	
				for(fu=0; fu<length; fu+=bpp)
				{
					temp=data[fu];
					data[fu]=data[fu + 2];
					data[fu +2]=temp;
				}	

			}
		}

		fclose(fp);
	
	}
};
