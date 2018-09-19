// **********************************************************************
// PUCRS/FACIN
// COMPUTA��O GR�FICA
//
// Arquivo: AVITest.cpp
//
// Programa de testes para carga de Imagens e v�deos AVI
//
//  Este programa deve ser compilador junto com a classe "ImageClass",
//  que est� implementada no arquivo "ImageClass.cpp"
//
//  Para a carga das imagens este programa utiliza a biblioteca
//  IM http://www.tecgraf.puc-rio.br/im de autoria de Antonio Scuri
//
// Marcio Serolli Pinho
// 12/08/2003
//  pinho@inf.pucrs.br
// **********************************************************************

#include <cstdlib>
#include <iostream>
using namespace std;

#include "AVIClass.h"

int cinza = false;
int removeRuido = false;


AVIClass Video;

// **********************************************************************
//  void init(void)
//  Inicializa os par�metros globais de OpenGL
//
// **********************************************************************
void init(void)
{
cout << "Init..." ;
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    imFormatRegisterAVI();
    //imFormatRegisterWMV();
    //if(Video.openVideoFile("Videos\\VideoDemo.avi") == 0)
    if(Video.openVideoFile("Videos\\VideoBike.avi") == 0)
    {
       cout << "Problemas na abertura do video" << endl;
    }
    else cout << "Abertura OK ! - Nro de Frames:" << Video.getTotalFrames() << endl;
    cout << "Dimensoes do Video:" << Video.SizeX() << " x " << Video.SizeY() <<endl;

cout << "Fim." << endl;

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
//  void ConverteCinza(ImageClass *Img)
//
//
// **********************************************************************
void ConverteCinza(AVIClass V)
{

    int x,y;
    int i;
    unsigned char r,g,b;
    //cout << "Iniciou Black & White....";
    //NovaImagem->DrawPixel(20, 1,100,255,0,0 );

    for(x=0;x<V.SizeX();x++)
    {
        for(y=0;y<V.SizeY();y++)
        {
            i = V.GetPointIntensity(x,y);
            //Img->ReadPixel(x,y, r,g,b);
            if (i> 225)
              V.DrawPixel(x, y,255,255,255);
            else V.DrawPixel(x, y,255,0,0);
        }
    }
}


void OrdenaVetor(int window[])
{
    int temp, i , j;
    for(i = 0; i < 9; i++){
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--){
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

void MontaVetor(int xP, int yP, int vetor[], AVIClass V){

    int idxV = 0;
    for(int x=xP-1;x<=xP+1;x++)
    {
        for(int y=yP-1;y<=yP+1;y++)
        {
            vetor[idxV++] = V.GetPointIntensity(x,y);
        }
    }
}

void RemoveRuido(AVIClass V)
{

    int x,y;
    int i;
    unsigned char r,g,b;
    //cout << "Iniciou Black & White....";
    //NovaImagem->DrawPixel(20, 1,100,255,0,0 );

    int Vetor[9];
    for(x=1;x<V.SizeX() -1;x++)
    {
        for(y=1;y<V.SizeY() -1;y++)
        {
            MontaVetor(x,y, Vetor, V); // Coloca em VETOR os valores das intensidades ao redor do ponto x,y.
            OrdenaVetor(Vetor);
            int mediana = Vetor[5];
            V[x][y] = mediana;
        }
    }
}

// **********************************************************************
//  void CalculaNivelDeZoom(float &ZoomH, float &ZoomV)
//
//
// **********************************************************************
void CalculaNivelDeZoom(float &ZoomH, float &ZoomV)
{
     ZoomH = (float)glutGet(GLUT_WINDOW_WIDTH)/Video.SizeX();
     ZoomV = (float)glutGet(GLUT_WINDOW_HEIGHT)/Video.SizeY();
}
// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
int frame = 1;
void display( void )
{

    int loadFrameOK;
    float ZoomH, ZoomV;

    glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10,10,-10,10,-1,1);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

    loadFrameOK = Video.loadImageFrame(frame);
    // avan�a o nro do frame
    frame ++;
    cout << "Frame: " << frame << endl;
    // se atingiu o final do v�deo, ent�o recome�a
    if (frame == Video.getTotalFrames())
    {
         frame = 0;
        // getchar();
         return;
    }
    if (loadFrameOK)
    {

      // cout << "Imagem Exibida !!" << endl;
       Video.SetPos(-10,-10);
       // acerta o zoom da imagem para que ocupe toda a janela
       CalculaNivelDeZoom(ZoomH, ZoomV);
       Video.SetZoomH(ZoomH);
       Video.SetZoomV(ZoomV);
       if (cinza)
       {
          ConverteCinza(Video);
       }
       if (removeRuido)
       {
          RemoveRuido(Video);
       }
       Video.Display();
    }
    else cout << "Erro..." << endl;

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
        case 'r' :
                removeRuido = !cinza;
                glutPostRedisplay();
                break;

        case 'c': cinza = !cinza;
                  glutPostRedisplay();
                  break;

        case ' ':
                   glutPostRedisplay();
                   break;
        case 27: // Termina o programa qdo
             Video.closeVideoFile();
        exit ( 0 );   // a tecla ESC for pressionada
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
    init ();

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (300,10);

    // Define o tamanho da janela gr�fica do programa
    glutInitWindowSize  ( 640, 480);
    glutCreateWindow    ( "AVI Loader - v2.0" );

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    //glutIdleFunc ( display );

    MessageBox(NULL, "Pressione a barra de espa�o para avancar o v�deo.", "Mensagem", MB_OK);
    cout << "Pressione a barra de espa�o para avancar o v�deo." << endl;

    glutMainLoop ();
    cout <<"FIM\n";

    return 0;
}


