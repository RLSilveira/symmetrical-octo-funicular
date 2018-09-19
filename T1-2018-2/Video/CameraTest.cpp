// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
//
// Programa básico para criar aplicações em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@inf.pucrs.br
// **********************************************************************

#include <iostream>
#include <windows.h>
#include "gl\glut.h"

using namespace std;

#include "ImageClassNew2.h"
#include "CameraClass.h"

CameraClass *Camera1, *Camera2;
ImageClass *NovaImagem1, *NovaImagem2;

#define LIMIAR 150
// **********************************************************************
//  void ConvertBlackAndWhite()
// **********************************************************************
void ConvertBlackAndWhite(ImageClass *I)
{
    // Tarefa 1:
    //  Mude o valor do LIMIAR para números pequenos como 5 ou 10

    int x,y;
    int i;
    cout << "Iniciou Black & White....";

    for(x=0; x<I->SizeX(); x++)
    {
        for(y=0; y<I->SizeY(); y++)
        {
            i = I->GetPointIntensity(x,y);
            // printf("Intens Fora: %5d\n",i);
            if (I->GetPointIntensity(x,y) < LIMIAR)
            {
                NovaImagem1->DrawPixel(x, y,0,0,0);
            }
            else NovaImagem1->DrawPixel(x, y, 255,255,255);
        }
    }
    cout << "Concluiu Black & White." << endl;

}
// **********************************************************************
//  void init(void)
//  Inicializa os parâmetros globais de OpenGL
//
// **********************************************************************
void init(void)
{
    int ShowDialog = 0;
    int Device = 0;

    Camera1 = new CameraClass(ShowDialog, Device);
    Camera1->SetZoomH(0.5);
    Camera1->SetZoomV(0.5);

    Device = 1;
    Camera2 = new CameraClass(ShowDialog, Device);
    Camera2->SetZoomH(0.5);
    Camera2->SetZoomV(0.5);

    while (!Camera1->Live())
        Camera1->StartCapture();
    cout
    while (!Camera2->Live())
        Camera2->StartCapture();

    if (Camera1->Live())
        cout << "Camera 1 OK!" << endl;
    else cout << "Camera 1 OFF!" << endl;
    if (Camera2->Live())
        cout << "Camera 2 OK!" << endl;
    else cout << "Camera 2 OFF!" << endl;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    // Instacia o objeto que irá exibor a nova imagem
    // Caso precise alterar o tamanho da nova imagem, mude os parâmetros
    // da construtura, na chamada abaixo
    NovaImagem1 = new ImageClass(Camera1->SizeX(), Camera1->SizeY());
    NovaImagem2 = new ImageClass(Camera2->SizeX(), Camera2->SizeY());

    // posiciona a imagem nova ao lado da imagem da camera
    NovaImagem1->SetZoomH(0.5);
    NovaImagem1->SetZoomV(0.5);
    NovaImagem1->SetPos(Camera1->SizeX()*0.5+10, 1);

    // posiciona a imagem da segunda camera acima da primeira
    Camera2->SetPos(0, Camera1->SizeY()*0.5+10);

    // posiciona a imagem nova ao lado da imagem da segunda camera
    NovaImagem2->SetZoomH(0.5);
    NovaImagem2->SetZoomV(0.5);
    NovaImagem2->SetPos(Camera2->SizeX()*0.5+10, Camera2->SizeY()*0.5+10 );
}
// **********************************************************************
// void LeCameras()
//
// **********************************************************************
int cont =0;
void LeCameras()
{

    if (cont == 15000)
    {
        if (Camera1->Live())
            //cout << "Camera 1 OK!" << endl;
            Camera1->Update();
        else cout << "Camera 1 OFF!" << endl;
        if (Camera2->Live())
            //cout << "Camera 2 OK!" << endl;
            Camera2->Update();
        else cout << "Camera 2 OFF!" << endl;

        glutPostRedisplay();  // força o redesenho da tela
        cont =0;
        //cout<< "fff";
    }
    cont ++;
}

// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0,w,0,h);

    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}
// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
bool flag = false;

void display( void )
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    // Exibe a imagens das duas cameras
    if (flag)
    {
        Camera1->Update();
        Camera1->Display();
    }
    else
    {
        Camera2->Update();
        Camera2->Display();
    }
    //flag = !flag;

    // Exibe as duas imagens
    NovaImagem1->Display();
    NovaImagem2->Display();

    glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
    case 27: // Termina o programa qdo
        NovaImagem1->Delete();
        Camera1->Delete();
        exit ( 0 );   // a tecla ESC for pressionada
        break;
    case '2':
        ConvertBlackAndWhite(Camera1);
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;


    default:
        break;
    }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
    case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
        break;
    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...

        break;
    default:
        break;
    }
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (10,10);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( 650, 500);
    glutCreateWindow    ( "Camera Viewer" );

    init ();

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    glutIdleFunc ( LeCameras );

    glutMainLoop ( );
    cout << "FIM" << endl;

    return 0;
}


