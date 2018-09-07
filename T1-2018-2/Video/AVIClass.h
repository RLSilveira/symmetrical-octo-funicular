// **********************************************************************
// AVIClass.h
//     classe para carga de v�deos no formato AVI de imagens
//
// **********************************************************************

// ATEN��O
// Caso ocorra algum erro de compila��o em fun��o de que
// o seu compilador n�o possui as bibliotecas de OpenGL,
// abra o diret�rio "GL_Headers_and_libs" que acompanha
// este arquivo e copie o subdiret�rio "GL" para dentro do diret�rio
// "include" de seu compilador

// include files for OpenGL
#include <GL\gl.h>

#include <GL\glut.h>

// Include files for IM library
// http://www.tecgraf.puc-rio.br/im/

#include "IM/include/im.h"
#include "IM/include/im_util.h"
#include "IM/include/im_format_avi.h"
#include "IM/include/im_format_wmv.h"
#include "IM/include/im_convert.h"






// Include files for Image library
#include "ImageClassNew3.h"

class AVIClass: public ImageClass
{
    imFile* ifile;
    char format[10], compression[10];
    int error, image_count;
    int width, height, color_mode, data_type;

    public:
    AVIClass();
    int openVideoFile(char *name);
    int loadImageFrame(int nFrame);
    void printError(int error);
    void printImageInfo(int color_mode);
    int getTotalFrames();
    void closeVideoFile();
};
