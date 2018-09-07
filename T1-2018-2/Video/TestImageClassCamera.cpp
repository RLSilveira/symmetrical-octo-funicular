// ****************************************************************
//  TestImageClassCamera.cpp
//
//
// ****************************************************************

#include <iostream>

#include "CameraClass.h"

CameraClass *Camera;

int capturingBackground = 1;

// *********************************************************
// Mantém atualizada a iamgem que vai para a tela
void myIdle()
{

	Camera->Update();	
	glutPostRedisplay();  // força o redesenho da tela

	
}
// *********************************************************   
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
	double hw=Camera->SizeX()*1.0/Camera->SizeY()/2;

	// Posiciona a imagem no canto inferior esquerdo da 
	// janela de seleção (veja o "glOrtho")
	Camera->SetPos(-10,-10);
 
    DesenhaEixosImagem();

	Camera->Display();

	DesenhaEixosJanelaSelecao();
	glutSwapBuffers();
}

// *********************************************************
void parsekey(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: 
		delete Camera;
		//   CoUninitialize();
		exit(1);
	case ' ':
        capturingBackground = !capturingBackground;
		break;
	case 's':
		break;
	default:
		glutPostRedisplay();
		return;
	}
}

void Init()
{
//	glEnable(GL_DEPTH_TEST);
		
	// Coloquei aqui o que retirei da função display...
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10,10,-10,10,-1,1);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();		
	
}

int main(int argc, char* argv[])
{
	//  CoInitialize(NULL);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Live camera image");
	glutDisplayFunc(display);
	glutKeyboardFunc(parsekey);
	glutIdleFunc(myIdle);
	
	Init();

	// Cria o objeto Câmera sem abrir a janela de configuração
	//
	Camera = new CameraClass(0);
	
	glutMainLoop();
	
	return 1;
}
