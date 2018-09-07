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

ImageClass *Image, *NovaImagem;

#define LIMIAR 150
// **********************************************************************
//  void ConvertBlackAndWhite()
//
//
// **********************************************************************
void ConvertBlackAndWhite()
{
    // Tarefa 1:
    //  Mude o valor do LIMIAR para números pequenos como 5 ou 10

    int x,y;
    int i;
    cout << "Iniciou Black & White....";

    for(x=0;x<Image->SizeX();x++)
    {
        for(y=0;y<Image->SizeY();y++)
        {
            i = Image->GetPointIntensity(x,y);
            // printf("Intens Fora: %5d\n",i);
            if (Image->GetPointIntensity(x,y) < LIMIAR)
            {
                NovaImagem->DrawPixel(x, y,0,0,0);
            }
			else NovaImagem->DrawPixel(x, y, 255,255,255);

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

    Image = new ImageClass();
    int r;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    r = Image->Load("Aguia.bmp");

    if (!r)
        exit(1);
    else
        cout <<"Imagem carregada!\n";

    // Instacia o objeto que irá exibor a nova imagem
    // Caso precise alterar o tamanho da nova imagem, mude os parâmetros
    // da construtura, na chamada abaixo
    NovaImagem = new ImageClass(Image->SizeX(), Image->SizeY());

    // posiciona a imagem nova ao lado da antiga
    NovaImagem->SetPos(Image->SizeX()+10, 1);

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
void display( void )
{

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    Image->Display();
    NovaImagem->Display();

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
        NovaImagem->Delete();
        Image->Delete();
        exit ( 0 );   // a tecla ESC for pressionada
        break;
        case '2':
        ConvertBlackAndWhite();
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;

        break;
        case ' ':
        //Image->DrawLineH(50, 1, 30, 200,0,0);
        Image->Clear();
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
    glutCreateWindow    ( "Image Loader" );

    init ();

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    //glutIdleFunc ( display );

    glutMainLoop ( );
    cout << "FIM" << endl;

    return 0;
}


