

#include <iostream>
using namespace std;
#include <windows.h>

#include "im_capture.h"

imVideoCapture*  init(int ShowDialog, int device, int &gHeight, int &gWidth )
{
	imVideoCapture* myVideoCap;

	myVideoCap = imVideoCaptureCreate();
	if (!myVideoCap)
	{
		cout << "Problemas na inicializacao da camera " << device << "." << endl;
		return NULL;
	}
	cout << "Inicializacao da camera " << device << " realizada." << endl;

	imVideoCaptureConnect(myVideoCap, device);

	if (ShowDialog)
		imVideoCaptureShowDialog(myVideoCap, 0, NULL); // o indice 0 escolhe qual o dialogo a ser mostrado.

	imVideoCaptureGetImageSize(myVideoCap, &gWidth, &gHeight);

	return myVideoCap;
}

using namespace std;
int main( int argc, char** argv )
{
	imVideoCapture *Camera0, *Camera1;
	int altura0, largura0, altura1, largura1;
	int device;

	device = 0;
	Camera0 = init (0, device, altura0, largura0);
	device = 1;
	Camera1 = init (0, device, altura1, largura1);

	if (!imVideoCaptureLive(Camera0, 1))
		cout <<"Camera 0 NAO disponivel" << endl;
	else
		cout <<"Camera 0 disponivel" << endl;

	if (!imVideoCaptureLive(Camera1, 1))
		cout <<"Camera 1 NAO disponivel" << endl;
	else
		cout <<"Camera 1 disponivel" << endl;

	imVideoCaptureDisconnect(Camera0);
	imVideoCaptureDestroy(Camera0);

	imVideoCaptureDisconnect(Camera1);
	imVideoCaptureDestroy(Camera1);
	return 0;
}


/*
Inicializacao da camera 0 realizada.
Inicializacao da camera 1 realizada.
Camera 0 disponivel
Camera 1 NAO disponivel

Process returned 0 (0x0)   execution time : 3.094 s
Press any key to continue.

*/
