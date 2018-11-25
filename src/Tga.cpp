#include "Tga.h"

Tga::Tga(char *tN)
{
    LoadTexture(tN);
}

void Tga::LoadTexture(char *filename)
{
    GLubyte uheader_format[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GLubyte header_to_compare[12];
    GLubyte attr[6];
    int buffer;
    unsigned int length;
    unsigned int bpp;
    FILE *fp;

    fp = fopen(filename, "rb");

    if (!fp)
    {
        printf("Could not open file %s\n", filename);
    }
    else
    {
        fread(header_to_compare, 1, sizeof(header_to_compare), fp);
        buffer = memcmp(uheader_format, header_to_compare, sizeof(uheader_format));

        if (buffer == 0)
        {
            fread(attr, 1, sizeof(attr), fp);
            m_width = attr[1] * 256 + attr[0];
            m_height = attr[3] * 256 + attr[2];
            bpp = attr[4] / 8;
            length = m_width * m_height * bpp;
            m_data = (GLubyte *)malloc(length);
            if (!m_data)
            {
                printf("Could not allocate memory for the TGA texture\n");
            }
            else
            {
                fread(m_data, 1, length, fp);
            }

            for (int i = 0; i < length; i += bpp)
            {
                std::swap(m_data[i], m_data[i+2]);
            }
        }
    }

    fclose(fp);
}
