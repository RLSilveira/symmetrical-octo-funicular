
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <math.h>

#include <GL\gl.h>

#include "ImageClassNew3.h"
#include "BmpLib0.h"

#define TAM_PIXEL 3

// **********************************************************************
//
//	Constructor
// **********************************************************************
ImageClass::ImageClass()
{
	data = NULL;
	PosX = PosY = 0;
	sizeX = 0;
	sizeY = 0;
// NOVO
// Isto resolve o problema de ter a imagem com
// largura múltipla de 4
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	zoomH = zoomV = 1;
}

// **********************************************************************
//
//	Constructor
// **********************************************************************
ImageClass::ImageClass(int sizeX, int sizeY)
{

    SetSize(sizeX,sizeY);
	PosX = PosY = 0;
	zoomH = zoomV = 1;

// NOVO
// Isto resolve o problema de ter a imagem com
// largura múltipla de 4
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

}

// **********************************************************************
//
//
// **********************************************************************
void ImageClass::SetSize(int sizeX, int sizeY)
{
	unsigned int tam;

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	tam = sizeof(unsigned char) * sizeX * sizeY * TAM_PIXEL;
	data = (unsigned char *) malloc (tam);
	memset(data,255,tam);

	PosX = PosY = 0;
	zoomH = zoomV = 1;

// NOVO
// Isto resolve o problema de ter a imagem com
// largura múltipla de 4
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

}
// **********************************************************************
//
//
// **********************************************************************
unsigned char *ImageClass::GetImagePtr()
{
	return data;
}
// **********************************************************************
//
//
// **********************************************************************
void ImageClass::SetPos(int X, int Y)
{
	PosX = X;
	PosY = Y;
}

// **********************************************************************
//
//
// **********************************************************************
int ImageClass::Load(char * nome)
{

    if (data)
        free(data); // é necessário desalocar a área da imagem
	if (!load_bmp(nome,sizeX, sizeY))
    {
        SetSize(sizeX, sizeY);
        CopyBmpToImageClass(data);
        freeBMPMemory();
        return 1;
    }
    return 0;
}

// **********************************************************************
//
//
// **********************************************************************
void ImageClass::Save(char *nome)
{
    save_bmp(nome, data, sizeX, sizeY);
}
// **********************************************************************
//
//
// **********************************************************************
void ImageClass::Display()
{
	glPixelZoom(zoomH , zoomV);
	glRasterPos2f(PosX, PosY);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(sizeX, sizeY, GL_RGB, GL_UNSIGNED_BYTE, data);

//  glDrawPixels(sizeX, sizeY, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
}


// **********************************************************************
//
//
// **********************************************************************
void ImageClass::Delete()
{
	// Cleanup
	if (data)
	{
		free(data);
		data = NULL;
    }
}
// **********************************************************************
//
//
// **********************************************************************
void ImageClass::DrawPixel(GLint x, GLint y, unsigned char r, unsigned char g, unsigned char b)
{
	unsigned long addr;

	addr = (unsigned long)( y *(sizeX)* TAM_PIXEL + x * TAM_PIXEL );
	data[addr++] = r;
	data[addr++] = g;
	data[addr] = b;
}
// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::DrawLineH(int y, int x1, int x2,unsigned char r, unsigned char g, unsigned char b )
{
	int x;
	if (x1 <= x2)
		for (x = x1; x<=x2; x++)
		{
			DrawPixel(x,y,r,g,b);
		}
    else
		for (x = x2; x<=x1; x++)
		{
			DrawPixel(x,y,r,g,b);
		}

}
// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::DrawLineV(int x, int y1, int y2,unsigned char r, unsigned char g, unsigned char b )
{
	int y;
	for (y = y1; y<=y2; y++)
	{
		DrawPixel(x,y,r,g,b);
	}
}

// **********************************************************************
//
//
// **********************************************************************
void ImageClass::ReadPixel(GLint x, GLint y, unsigned char &r, unsigned char &g, unsigned char &b)
{
	unsigned long addr;

	addr = (unsigned long)( y *(sizeX)* TAM_PIXEL + x * TAM_PIXEL );
	r = data[addr++];
	g = data[addr++];
	b = data[addr];
}


// **********************************************************************
//
//
//
// **********************************************************************
double ImageClass::GetPointIntensity(int x, int y)
{
	unsigned char r,g,b;
	double i;

	ReadPixel(x,y,r,g,b);
	i = (0.3 * r + 0.59 * g + 0.11 * b);
	return i;
}

// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::CopyTo(ImageClass *i)
{

	unsigned int tam;

    tam = sizeof(unsigned char) * sizeX * sizeY * TAM_PIXEL;
	memcpy(i->data, data,tam);

}

// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::Clear()
{
	unsigned int tam;
	tam = sizeof(unsigned char) * SizeX() * SizeY() * TAM_PIXEL;
	memset(data,255,tam);

}

// **************************************************************
//
// **************************************************************
void ImageClass::DrawBox(int x1,int y1,int x2,int y2,unsigned char r, unsigned char g, unsigned char b )
{
    DrawLineH(y1, x1, x2, r,g,b);
    DrawLineH(y2, x1, x2, r,g,b);
    DrawLineV(x1, y1, y2, r,g,b);
    DrawLineV(x2, y1, y2, r,g,b);
}

// **************************************************************
//
// **************************************************************
void ImageClass::DrawLine(int x0,int y0,int x1, int y1,unsigned char r, unsigned char g, unsigned char bb  )
{
        int dx = x1 - x0;
        int dy = y1 - y0;
        int x,y;
        float b, m;

        DrawPixel(x0, y0, r,g,bb);
        if (abs(dx) > abs(dy)) {          // slope < 1
            m = (float) dy / (float) dx;      // compute slope
            b = y0 - m*x0;
            dx = (dx < 0) ? -1 : 1;
            while (x0 != x1) {
                x0 += dx;
                y = (int)((m*x0 + b)+0.5);
                DrawPixel(x0, y, r,g,bb);
            }
        } else
        if (dy != 0) {                              // slope >= 1
            float m = (float) dx / (float) dy;      // compute slope
            float b = x0 - m*y0;
            dy = (dy < 0) ? -1 : 1;
            while (y0 != y1) {
                y0 += dy;
                x = (int) ((m*y0 + b)+0.5);
                DrawPixel(x, y0, r,g,bb);
            }
        }
}
