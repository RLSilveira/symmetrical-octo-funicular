// **********************************************************************
// CameraClass.h
//        classe para Captura de imagens
// **********************************************************************

#include "im_capture.h"

#include "ImageClassNew3.h"

class CameraClass: public ImageClass
{

private:
		imVideoCapture* myVideoCap;
		int gHeight, gWidth;

public:

	CameraClass(int ShowDialog, int device);
	~CameraClass();
	void StartCapture();
	bool Live();
	void Update();
	void OpenDialog(int nDialog);
	int DriverOK();

};
