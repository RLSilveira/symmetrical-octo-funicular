// **********************************************************************
// CameraClass.cpp
//        classe para manipulação de imagens de câmera
// **********************************************************************

#include <iostream>
using namespace std;


#include "CameraClass.h"

#define TAM_PIXEL 3

// **********************************************************************
//
//	Constructor
// **********************************************************************
CameraClass::CameraClass(int ShowDialog, int device)
{
	gHeight = gWidth = 0;

	myVideoCap = imVideoCaptureCreate();
	if (!myVideoCap)
	{
	    cout << "Problemas na inicializacao da camera " << device << "." << endl;
	    return;
	}
	cout << "Inicializacao da camera " << device << " realizada." << endl;

	cout << "Conexao: " << imVideoCaptureConnect(myVideoCap, device) << endl;
	//imVideoCaptureLive(myVideoCap, 1);

	if (ShowDialog)
		imVideoCaptureShowDialog(myVideoCap, 0, NULL); // o indice 0 escolhe qual o dialogo a ser mostrado.

	imVideoCaptureGetImageSize(myVideoCap, &gWidth, &gHeight);

	SetSize(gWidth, gHeight);
}
//Alternativa imVideoCaptureOneFrame
void CameraClass::StartCapture()
{
    imVideoCaptureLive(myVideoCap, 1);
}
// **********************************************************************
//
//
// **********************************************************************
int CameraClass::DriverOK()
{
    return (myVideoCap != NULL);
}
// **********************************************************************
//
//	Destructor
// **********************************************************************
CameraClass::~CameraClass()
{
	imVideoCaptureDisconnect(myVideoCap);
	imVideoCaptureDestroy(myVideoCap);
	ImageClass::Delete();
}

// *********************************777777777777777777777777*************************************
//
//	Update
// **********************************************************************
void CameraClass::Update()
{
    if (!DriverOK())
        return;

	if (!imVideoCaptureLive(myVideoCap, -1))
	{
		cout <<"Retornando....\n";
		return;
	}
	//while(!Live())
	imVideoCaptureFrame(myVideoCap, GetImagePtr(), IM_RGB|IM_PACKED, 1000);
//	imVideoCaptureFrame(myVideoCap, GetImagePtr(), 1000);

}
bool CameraClass::Live()
{
    return imVideoCaptureLive(myVideoCap, -1);

}
// **********************************************************************
//
//	OpenDialog
// **********************************************************************
void CameraClass::OpenDialog(int nDialog)
{
   imVideoCaptureShowDialog(myVideoCap, nDialog, NULL); // o segundo parâmetro escolhe
                                                        // qual o dialogo a ser mostrado.

}
