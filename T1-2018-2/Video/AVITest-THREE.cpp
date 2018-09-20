// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
//
// Arquivo: AVITest.cpp
//
// Programa de testes para carga de Imagens e vídeos AVI
//
//  Este programa deve ser compilador junto com a classe "ImageClass",
//  que está implementada no arquivo "ImageClass.cpp"
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

int intensidade = 215;
int red = 15;
int cinza = false;
int ruido = false;
int square = false;

AVIClass Video;

// **********************************************************************
//  void init(void)
//  Inicializa os parâmetros globais de OpenGL
//
// **********************************************************************
void init(void)
{
    cout << "Init..." ;
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    imFormatRegisterAVI();
    //imFormatRegisterWMV();
    if(Video.openVideoFile("Videos\\VideoBike.avi") == 0)
    //if(Video.openVideoFile("Videos\\video_original.avi") == 0)
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
            if (i> intensidade)
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

void MontaVetor(int xP, int yP, int vetor[], AVIClass V)
{
    int idxV = 0;
    for(int x = xP - 1; x <= xP + 1; x++){
        for(int y = yP - 1; y <= yP + 1; y++){
            vetor[idxV++] = V.GetPointIntensity(x,y);
        }
    }
}

void SaltAndPepper(AVIClass V)
{
    int win[9];

    for(int x = 1; x < V.SizeX() - 1; x++){
        for(int y = 1; y < V.SizeY() - 1; y++){
            MontaVetor(x, y, win, V);
            OrdenaVetor(win);
            //V.DrawPixel(x, y, win[5]);
        }
    }
}

bool verificaPontosAoRedor(int xP, int yP, AVIClass V, int redor){
    for(int x = xP - redor; x <= xP + redor; x++){
        for(int y = yP - redor; y <= yP + redor; y++){
                if(V.GetPointIntensity(x, y) != V.GetPointIntensity(xP, yP)){
                    return false;
                }
        }
    }
    return true;
}

void Square(AVIClass V, int redor){
    int c = 0;
    for(int x = redor; x < V.SizeX() - redor; x++){
        for(int y = redor; y < V.SizeY() - redor; y++){
            if(V.GetPointIntensity(x,y) < intensidade){
                if(verificaPontosAoRedor(x, y, V, redor)){
                    //V.DrawBox(x-redor, y-redor, x+redor, y+redor, 0, 0, 0);
                    //V.DrawLineH(y, x-redor, x+redor, 0, 0, 0);
                    //V.DrawLineV(x, y-redor, y+redor, 0, 0, 0);
                    c++;
                    //V.DrawLineH(y, 100, 100, 0, 0, 0);
                    //V.DrawLineV(x, 100, 100, 0, 0, 0);
                    V.DrawBox( x, y, 20, 20, 255,255,0);
                    cout << "ACHOU NOS PONTOS " << x << ", " << y << "\t" << c << endl;
                }
            }
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
    // avança o nro do frame
    frame ++;
    cout << "Frame: " << frame << endl;
    // se atingiu o final do vídeo, então recomeça
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
       if(ruido)
       {
            SaltAndPepper(Video);
            SaltAndPepper(Video);
       }
       if(square){
            Square(Video, red);
       }
       if (cinza)
       {
          ConverteCinza(Video);
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
        case 'c': cinza = !cinza;
                  glutPostRedisplay();
                  break;

        case 'r': ruido = !ruido;
                  glutPostRedisplay();
                  break;

        case 's': square = !square;
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

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( 640, 480);
    glutCreateWindow    ( "AVI Loader - v2.0" );

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    //glutIdleFunc ( display );

    MessageBox(NULL, "Pressione a barra de espaço para avancar o vídeo.", "Mensagem", MB_OK);
    cout << "Pressione a barra de espaço para avancar o vídeo." << endl;

    glutMainLoop ();
    cout <<"FIM\n";

    return 0;
}

