// **********************************************************************
// AVIClass.cpp
//        classe para carga de vídeos no formato AVI de imagens
//
// **********************************************************************

#include <iostream>
using namespace std;

#include "AVIClass.h"

#define TAM_PIXEL 3

// **********************************************************************
//
// **********************************************************************
AVIClass::AVIClass()
{
    imFormatRegisterAVI();
   // imFormatRegisterWMV();

}
// **********************************************************************
//
// **********************************************************************
int AVIClass::openVideoFile(char *name)
{
cout << "OpenVideoFile..." << endl;
    int error;

    ifile = imFileOpen(name, &error);
    if (error != IM_ERR_NONE)
    {
        cout << "CALL: imFileOpen" << endl;
        printError(error);
        cout << "OpenVideoFile... Done." << endl;
        return 0;
    }
    imFileGetInfo(ifile, format, compression, &image_count);

    error = imFileReadImageInfo(ifile, 0, &width, &height, &color_mode, &data_type);
    //cout << "Frame: " << frame << endl ;
    if (error != IM_ERR_NONE)
    {
        cout << "CALL: imFileReadImageInfo" << endl;
        cout << "Error: " << error << " reading image info (ReadImageInfo)." << endl;
        printError(error);
        cout << "OpenVideoFile... Done." << endl;
        return 0;
    }
    // Cria um objeto para armazenar a imagem
    cout << "Creating a new image..." << endl;
    //Img = new ImageClass();
    // Aloca memória para a imagem
    SetSize(width, height);

cout << "OpenVideoFile... Done." << endl;
    return 1;

}

// **********************************************************************
//
// **********************************************************************
int AVIClass::loadImageFrame(int nFrame)
{
//cout << "LoadImageFrame (" << nFrame <<")" << endl;
    int error;

    error = imFileReadImageInfo(ifile, nFrame, &width, &height, &color_mode, &data_type);

    if (error != IM_ERR_NONE)
    {
        cout << "Error: " << error << " reading image info (ReadImageInfo)." << endl;
        printError(error);
        return 0;
    }

    error = imFileReadImageData(ifile, GetImagePtr(), 0, -1);
    if (error != IM_ERR_NONE)
    {

        cout << "Error: " << error << " reading image data (ReadImageData)." << endl;
        printError(error);
        return 0;
    }
    else
    {
        // cout << "Reading succesfull!" << endl;
    }

    // Se a imagem está no formato de Palette, converte para
    // o formato RGB-True Color
    if (imColorSpace(color_mode) == IM_MAP)
    {
       long palette[256];
       int palette_count;

       imFileGetPalette(ifile, palette, &palette_count);
       imConvertMapToRGB(GetImagePtr(), width*height, 3, 1, palette, palette_count);
    }
    return 1;


}
// **********************************************************************
//
// **********************************************************************
int AVIClass::getTotalFrames()
{
    return image_count;
}

// **********************************************************************
//
// **********************************************************************
void AVIClass::closeVideoFile()
{
    imFileClose(ifile);
}
// **********************************************************************
//
// **********************************************************************
void AVIClass::printError(int error)
{
     string msg;
     switch (error)
    {
        case IM_ERR_OPEN:
        msg = "---Error Opening File.";
        break;
        case IM_ERR_MEM:
        msg = "Insuficient memory.";
        break;
        case IM_ERR_ACCESS:
        msg = "Error Accessing File.";
        break;
        case IM_ERR_DATA:
        msg = "Image type not Suported.";
        break;
        case IM_ERR_FORMAT:
        msg = "Invalid Format.";
        break;
        case IM_ERR_COMPRESS:
        msg = "Invalid or unsupported compression.";
        break;
        default:
        msg = "Unkown error.";
     }
     cout << "Erro: " << msg << endl;
}

// **********************************************************************
// void PrintImageInfo(int color_mode)
// **********************************************************************
void AVIClass::printImageInfo(int color_mode)
{
// Imprime informações sobre a imagem
    cout << "File Info...." << endl;
        cout << "Color Mode: " << imColorModeSpaceName(color_mode) << endl;

        if (imColorModeIsPacked(color_mode))
           cout << "Color Mode PACKED" << endl;
        else cout << "Color Mode NON PACKED" << endl;

        if (imColorModeHasAlpha(color_mode))
           cout << "Com ALPHA" << endl;
        else cout << "Sem ALPHA" << endl;

        if (imColorModeIsTopDown(color_mode))
           cout << "TopDown" << endl;
        else cout << "BottonUP" << endl;
}
