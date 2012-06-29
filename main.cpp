/* 
Tugas Besar Grafkom 
 IF-9
 Kelompok 8 :
               Risky Prastya    : 10108443
               M.Ardhi Lazuardi : 10108451  (ketua)
               Bayu TriNovianto : 10108461
               Ali Firdaus      : 10108464

*/


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h> //for malloc/free
#include <math.h>


static GLfloat p=0.0;
static GLfloat c=0.0;
static GLfloat n=0.0;
static GLfloat m=0.0;
static GLfloat z=1.0;
static GLfloat rot=0.0;
static int buka = 0, tutup = 0;
int i,j;
GLUquadric *texture;
GLuint loadTexture;


int LoadBitmap(char *filename)
{
    FILE * file;
    char temp;
    long i;

    // own version of BITMAPINFOHEADER from windows.h for Linux compile
    struct {
      int biWidth;
      int biHeight;
      short int biPlanes;
      unsigned short int biBitCount;
      unsigned char *data;
    } infoheader;

    GLuint num_texture;

    if( (file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading

    fseek(file, 18, SEEK_CUR);  /* start reading width & height */
    fread(&infoheader.biWidth, sizeof(int), 1, file);

    fread(&infoheader.biHeight, sizeof(int), 1, file);

    fread(&infoheader.biPlanes, sizeof(short int), 1, file);
    if (infoheader.biPlanes != 1) {
      printf("Planes from %s is not 1: %u\n", filename, infoheader.biPlanes);
      return 0;
    }

    // read the bpp
    fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
    if (infoheader.biBitCount != 24) {
      printf("Bpp from %s is not 24: %d\n", filename, infoheader.biBitCount);
      return 0;
    }

    fseek(file, 24, SEEK_CUR);

    // read the data
    if(infoheader.biWidth<0){
      infoheader.biWidth = -infoheader.biWidth;
    }
    if(infoheader.biHeight<0){
      infoheader.biHeight = -infoheader.biHeight;
    }
    infoheader.data = (unsigned char *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
    if (infoheader.data == NULL) {
      printf("Error allocating memory for color-corrected image data\n");
      return 0;
    }

    if ((i = fread(infoheader.data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
      printf("Error reading image data from %s.\n", filename);
      return 0;
    }

    for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
      temp = infoheader.data[i];
      infoheader.data[i] = infoheader.data[i+2];
      infoheader.data[i+2] = temp;
    }


    fclose(file); // Closes the file stream

    glGenTextures(1, &num_texture);
    glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);
    
    free(infoheader.data); // Free the memory we used to load the texture

    return (num_texture); // Returns the current texture OpenGL ID
}



void init(void)
{
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //parameter cahaya specular
GLfloat mat_shininess[] = { 100.0 }; //parameter shininess
GLfloat light_position[] = { 0.0, 0.0, -10.0, 1.0 }; //parameter posisi pencahayaan
glClearColor (0.0, 0.0, 0.0, 0.0);
glShadeModel (GL_SMOOTH); //mengaktifkan shade model
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glEnable(GL_LIGHTING); //mengaktifkan pencahayaan
glEnable(GL_LIGHT0); //mengaktifkan sumber cayaha
glEnable(GL_DEPTH_TEST);

texture = gluNewQuadric();
gluQuadricTexture(texture, GL_TRUE);

}
void keyboard (unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
    c = c + 0.5;
    if (c > 360.0)
    c = 360;//c - 360.0;
    glutPostRedisplay(); //generate layar
    break;
    
    case 'd':
    c = c - 0.5;
    if (c < -360.0)
    c = -360;//c + 360.0;
    glutPostRedisplay();
    break;

    case 'q':
    n = n + 0.5;
    if (n > 360.0)
    c = 360;//c - 360.0;
    glutPostRedisplay(); //generate layar
    break;
    
    case 'e':
    n = n - 0.5;
    if (n < -360.0)
    n = -360;//c + 360.0;
    glutPostRedisplay();
    break;
    
    case 'z':
    m = m + 0.5;
    if (m > 360.0)
    c = 360;//c - 360.0;
    glutPostRedisplay(); //generate layar
    break;
    
    case 'x':
    m = m - 0.5;
    if (m < -360.0)
    m = -360;//c + 360.0;
    glutPostRedisplay();
    break;
    
    case 'w':
    z = z + 0.1;
    if (z > 5.0)
    z = z - 5.0;
    glutPostRedisplay(); //generate layar
    break;
    
    case 's':
    z = z - 0.1;
    if (z < 0.0)
    z = z + 5.0;
    glutPostRedisplay();
    break;
    case 'b':
         buka = (buka + 7) % 90;
         glutPostRedisplay();
         break;
      case 'B':
         buka = (buka - 7) % 90;
         glutPostRedisplay();
         break;
      case 't':
         tutup = (tutup + 5) % 45;
         glutPostRedisplay();
         break;
      case 'T':
         tutup = (tutup - 5) % 45;
         glutPostRedisplay();
         break;
    
    default:
    break;
   
    }
}

void rotate(void){
     rot = rot + 0.05;
     if (rot > 360.0)
     rot = rot - 360.0;
     glutPostRedisplay();
}

void kursi (void){
     glPushMatrix();
     glScalef(1.0, 1.5, 0.3);
     glutSolidCube(1.0);
     glPopMatrix();
     
     glPushMatrix();
     glTranslatef(0.0, -0.7, 0.35);
     glRotatef(90, 1.0, 0.0, 0.0);
     glScalef(1.0, 1.0, 0.3);
     glutSolidCube(1.0);
     glPopMatrix();
     
     glPushMatrix();
     glTranslatef(0.5, -0.7, 0.35);
     glRotatef(90, 0.0, 1.0, 0.0);
     glScalef(1.0, 1.0, 0.2);
     glutSolidCube(1.0);
     glPopMatrix();
     
     glPushMatrix();
     glTranslatef(-0.5, -0.7, 0.35);
     glRotatef(90, 0.0, 1.0, 0.0);
     glScalef(1.0, 1.0, 0.2);
     glutSolidCube(1.0);
     glPopMatrix(); 
        
}

void korsi(void){
     glPushMatrix();
glColor3f(0, 1.0, 0.0);
glTranslatef(-4.2,1,2);
for(j=0; j<6; j++){
         glTranslatef(0.0, ((j*0.0)+0.5), (j*(0.0)+(-2.0)));
         kursi();
}
glPopMatrix();
}

void door(void)
{
   glPushMatrix();
   glTranslatef(4.2, 0.0, -0.0);
   glRotatef ((GLfloat) tutup, 0.0, -1.0, 0.0);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
   glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
   glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
   glEnd();
   glPopMatrix();
   //glBindTexture(GL_TEXTURE_2D, texName[0]);
   glPushMatrix();
   
   glRotatef ((GLfloat) buka, 0.0, -1.0, 0.0);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, -2.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, -2.0);
   glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
   glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
   glEnd();
   glPopMatrix();
}

void front_b(void){
//lantai dasar
glPushMatrix();
glTranslatef(0.0, -2.0, 3.3);
glColor3f(1.0, 0.0, 0.0);
glScalef(10.0, 0.5, 8.0);
glTranslatef(0.0, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();

//tatakan
glPushMatrix();
glTranslatef(0.0, -2.0, 13.5);
glColor3f(1.0, 2.0, 0.5);
glScalef(10.0, 0.5, 5);
glTranslatef(0.0, 1.5, -0.3);
glutSolidCube(1);
glPopMatrix();


glPushMatrix();
glTranslatef(0.0, -2.0, 13.5);
glColor3f(9,9, 9);
glScalef(5, 0.5,1);
glTranslatef(0.0,2, -2);
glutSolidCube(1);
glPopMatrix();


//layar
glPushMatrix();
glEnable ( GL_TEXTURE_2D );
glBindTexture ( GL_TEXTURE_2D, loadTexture);
//loadTexture = LoadBitmap("scandal.bmp");
glTranslatef(0.0, 0.0, 13.8);
glColor3f(1.0, 1.0, 1.0);
glScalef(5.0, 3.0, 0.4);
glTranslatef(0.0, 0.5, 1.0);
glRotatef(180,0,1,0);
      glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0,-1.0, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(  1.0,-1.0, 0.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(  1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f( -1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0,-1.0, 0.0);
      glEnd();
//glutSolidCube(1.0);
glDisable ( GL_TEXTURE_2D );
glPopMatrix();


//tembok sebelah layar
glPushMatrix();
glTranslatef(-3.0, -2.0, 14.8);
glColor3f(1.0, 0.0, 1.0);
glScalef(21.5, 7.5, 0.8);
glTranslatef(0.0, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();


//tembok sebelah kiri pintu
 glPushMatrix();
glTranslatef(-29.6, 1.7, 14.8);
glColor3f(1.0, 0.0, 1.0);
glScalef(15.2, 7.5, 0.8);
glTranslatef(0.0, 0.0, 1.0);
glutSolidCube(1.0);
glPopMatrix();

//tembok atas pintu
glPushMatrix();
glTranslatef(-15.5, 4.7, 14.8);
glColor3f(1.0, 0.0, 1.0);
glScalef(14.0, 1.5, 0.8);
glTranslatef(0.0, 0.0, 1.0);
glutSolidCube(1.0);
glPopMatrix();

}
void tatakan(void){
glPushMatrix();                                                          //tiang depan kiri
            glScalef(1.0, 15.0, 1.0);
            glColor3f(1.0,1.0,0.0);
            glTranslatef(-1.4, -0.02, 1.4);
            glutSolidCube(0.2);
            glPopMatrix(); 
glPushMatrix();                                                          //tiang belakang kanan
            glScalef(1.0, 15.0, 1.0);
            glColor3f(1.0,1.0,0.0);
            glTranslatef(1.4, -0.02, -1.4);
            glutSolidCube(0.2);
glPopMatrix();
glPushMatrix();                                                          //tiang belakang kiri
            glScalef(1.0, 15.0, 1.0);
            glColor3f(2.0,2.0,0.0);
            glTranslatef(-1.4, -0.02, -1.4);
            glutSolidCube(0.2);
glPopMatrix();
glPushMatrix();                                                          //tatakan bawah
            glColor3f(2.0, 0.0, 0.0);
            glTranslatef(0.0, -1.0, 0.0);
            glTranslatef(0.0, (0.5), 0.0);
            glScalef(1.0, 0.02, 1.0);
            glutSolidCube(3.0);
        glPopMatrix();
        glPushMatrix();                                                          //tatakan tengah
            glColor3f(2.0, 0.0, 0.0);
            glTranslatef(0.0, -2.3, 0.0);
            glTranslatef(0.0, (0.5), 0.0);
            glScalef(1.0, 0.02, 1.0);
            glutSolidCube(3.0);
        glPopMatrix();
        glPushMatrix();                                                          //tiang depan kanan
            glScalef(1.0, 15.0, 1.0);
            glColor3f(2.0,1.0,0.0);
            glTranslatef(1.4, -0.02, 1.4);
            glutSolidCube(0.2);
glPopMatrix();
glPushMatrix();                                                          //tatakan atas
            glColor3f(2.0, 0.0, 0.0);
            glTranslatef(0.0, 0.7, 0.0);
            glTranslatef(0.0, (0.5), 0.0);
            glScalef(1.0, 0.02, 1.0);
            glutSolidCube(3.0);
glPopMatrix();
}

void botol(void){
     
glPushMatrix();
        glTranslatef(0.0,-0.4,0.0);
        glScalef(0.9, 2.0, 1.0);  
        glColor3f(1.0,1.0,0.5);
        glRotatef(90,1.0,0.0,0.0);
        glutSolidCone(0.2, 0.4, 50, 10);
    glPopMatrix();
glPushMatrix();
        glTranslatef(0.0,-0.4,0.0);
        glScalef(0.9, 1.5, 1.0);
        glColor3f(1.0,1.0,0.5);
        glRotatef(-90,1.0,0.0,0.0);
        glutSolidCone(0.2, 0.4, 50, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0,-1.4,0.0);
        glScalef(1.0, 3.9, 1.0);  
        glColor3f(1.0,1.0,0.5);
        glRotatef(-90,1.0,0.0,0.0);
        glutSolidCone(0.25, 0.4, 50, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0,0.15,0.0);
        glScalef(0.2, 3.0, 0.2);  
        glColor3f(1.0,1.0,0.5);
        glRotatef(90,1.0,0.0,0.0);
        glutSolidCone(0.25, 0.4, 50, 10);
    glPopMatrix();


}

void gedung(void)
{

glPushMatrix();
glTranslatef(-1.2, 0.0, 0.0);
for (i=0; i<7; i++){
    glTranslatef((i*0.0)+1.2, 0.0, 0.0);
    korsi();
}
glPopMatrix();

//pijakan
glPushMatrix();
glTranslatef(0.0, -2.0, 8.3);
glColor3f(1.0, 0.0, 0.0);
glScalef(15.0, 1.0, 2);
for(i=0; i<9; i++){
glTranslatef(0.0, 0.5, (i*0.0)-1.0);
glutSolidCube(1.0);
      }
glPopMatrix();

front_b();

//pintu
glPushMatrix();
glScalef(2.0, 3.0, 1.0);
glTranslatef(-11.0, 0.3, 16.0);
door();
glPopMatrix();


///tempat sampah
glPushMatrix();
        glRotatef(90, 10.0, 0.0, 0.0);   
        glTranslatef(-9,5.0,0.5);                                    //lingkaran LUAR
        glColor3f(0.0,0,1);
        glScalef(1.0, 1.0, 100.0);
        glutSolidTorus(0.01, 0.5, 360, 360);
glPopMatrix();



//hiasan dinding
glPushMatrix();
        glRotatef(0, 10.0, 0.0, 0.0);   
        glTranslatef(-8,5.0,0.5);                              
        glColor3f(5,0,1);
        glScalef(1.5, 1.0, 100.0);
        glutSolidTorus(0.01, 0.2, 360, 360);
glPopMatrix();

glPushMatrix();
        glRotatef(0, 10.0, 0.0, 0.0);   
        glTranslatef(-8,4,-2);                              
        glColor3f(5,0,1);
        glScalef(1.5, 1.0, 100.0);
        glutSolidTorus(0.01, 0.2, 360, 360);
glPopMatrix();

glPushMatrix();
        glRotatef(0, 10.0, 0.0, 0.0);   
        glTranslatef(-8,5.0,-4.5);                              
        glColor3f(5,0,1);
        glScalef(1.5, 1.0, 100.0);
        glutSolidTorus(0.01, 0.2, 360, 360);
glPopMatrix();





//lantai dasar2
glPushMatrix();
glTranslatef(6.4, -2.1, -16.7);
glColor3f(0.0, 1.0, 0.7);
glScalef(3.0, 0.3, 21.80);
glTranslatef(0.0, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();


//tembok samping2
glPushMatrix();
glTranslatef(7.9, -2.0, -26.0);
glColor3f(1.0, 0.0, 1.0);
glScalef(0.5, 7.5, 28.0);
glTranslatef(0.0, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();


//tembok samping3
glPushMatrix();
glTranslatef(-37.0, -2.0, -26.0);
glColor3f(1.0, 0.0, 1.0);
glScalef(0.5, 7.5, 28.22);
glTranslatef(0.0, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();

//meja tiket1
glPushMatrix();
glRotatef(180, 0.0, 1.0, 0.0);
glTranslatef(55.7, -2.0, -21.5);
glColor3f(1.0, 0.0, 0.0);
glScalef(-4.5, 5.0, 14.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//meja tiket2
glPushMatrix();
glRotatef(-180, 0.0, 0.0, 0.0);
glTranslatef(105.0, 1.1, -12.5);
glColor3f(1.0, 0.0, 0.0);
glScalef(-12.0, 2.2, 2.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//meja tiket3
glPushMatrix();
glRotatef(-180, 0.0, 0.0, 0.0);
glTranslatef(103.4, 1.1, -4.1);
glColor3f(1.0, 0.0, 0.0);
glScalef(-12.0, 2.0, 2.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//kursi tiket1
glPushMatrix();
glRotatef(90, 0.0, 1.0, 0.0);
glTranslatef(-4.6, -0.6, -32.1);
glColor3f(1.0, 0.0, 0.0);
kursi();
glPopMatrix();

//kursi tiket2
glPushMatrix();
glRotatef(90, 0.0, 1.0, 0.0);
glTranslatef(-6.8, -0.6, -32.1);
glColor3f(1.0, 0.0, 0.0);
kursi();
glPopMatrix();

//kompi1
glPushMatrix();
glTranslatef(-40.4, 0.2, 1.7);
glColor3f(1.0, 1.0, 1.0);
glScalef(2.0, 0.0, 2.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//kompi2
glPushMatrix();
glTranslatef(-40.4, 0.2, 4.0);
glColor3f(1.0, 1.0, 1.0);
glScalef(2.0, 0.0, 2.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//layar film
glPushMatrix();
glTranslatef(-39.4, 0.5, -1.7);
glColor3f(1.0, 1.0, 1.0);
glScalef(0.5, 2.5, 6.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//layar coming soon
glPushMatrix();
glEnable ( GL_TEXTURE_2D );
glBindTexture ( GL_TEXTURE_2D, loadTexture);
loadTexture = LoadBitmap("Avriel.bmp");
glTranslatef(-37.4, 0.5, -5.7);
glColor3f(0.0, 0.0, 0.0);
glScalef(0.2, 4.5, 3.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glColor3f(1,1,1);
glRotatef(90,0,1,0);
glScalef(0.4, 0.35, 0.4);
glTranslatef(0,0,1);
      glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0,-1.0, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(  1.0,-1.0, 0.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(  1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f( -1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0,-1.0, 0.0);
      glEnd();
glDisable ( GL_TEXTURE_2D );

glPopMatrix();

glPushMatrix();
glEnable ( GL_TEXTURE_2D );
glBindTexture ( GL_TEXTURE_2D, loadTexture);
loadTexture = LoadBitmap("missA.bmp");
glTranslatef(-37.4, 0.5, -8.7);
glColor3f(0.0, 0.0, 0.0);
glScalef(0.2, 4.5, 3.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glColor3f(1,1,1);
glRotatef(90,0,1,0);
glScalef(0.4, 0.35, 0.4);
glTranslatef(0,0,1);
      glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0,-1.0, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(  1.0,-1.0, 0.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(  1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f( -1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0,-1.0, 0.0);
      glEnd();
glDisable ( GL_TEXTURE_2D );

glPopMatrix();

//tembok samping kursi
glPushMatrix();
glTranslatef(7.9, -2.0, -26.0);
glColor3f(1.0, 0.0, 1.0);
glScalef(0.5, 7.5, 18.0);
glTranslatef(-31.0, 0.5,1.3);
glutSolidCube(1.0);
glPopMatrix();

glPushMatrix();
glTranslatef(7.9, -2.0, -26.0);
glColor3f(1.0, 0.0, 1.0);
glScalef(0.5, 7.5,7);
glTranslatef(-31.0, 0.5, 5.5);
glutSolidCube(1.0);
glPopMatrix();

//lantai dasarblkg
glPushMatrix();
glTranslatef(1.4, -2.0, -20.8);
glColor3f(1.0, 0.0, 0.0);
glScalef(12.9, 0.2, 18.0);
glTranslatef(0.0, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();

//tembok blkg
glPushMatrix();
glTranslatef(-14.4, -2.0, -12.1);
glColor3f(1.0, 0.0, 1.0);
glScalef(45.0, 7.5, 0.2);
glTranslatef(0.0, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();


//kursi tunggu
glPushMatrix();
glTranslatef(-6.0, -2.0, -7.7);
glColor3f(1.0, 1.0, 0.0);
glScalef(0.5, 2.5, 7.0);
glTranslatef(-5, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();

glPushMatrix();
glTranslatef(-6.8, -2.0, -4.3);                               //komponen kursi tunggu
glColor3f(1.0, 1.0, 1.5);
glScalef(1.3, 1.5, 0.2);
glTranslatef(-2, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();

glPushMatrix();
glTranslatef(-6.8, -2.0, 2.5);
glColor3f(1.0, 1.0, 1.5);                                     //kompenen kursi tunggu
glScalef(1.3, 1.5, 0.2);
glTranslatef(-2, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();

glPushMatrix();
glTranslatef(0.3, -2.0, -9.1);
glColor3f(1.0, 0.0, 0.0);                                     //konsumen kursi tunggu
glScalef(-2.15, 1.5, 7.5);
glTranslatef(4.5, 0, 1.1);
glutSolidCube(1.0);
glPopMatrix();


//lantai dasar2
glPushMatrix();
glTranslatef(-19.5, -2.1, -26.0);
glColor3f(0.0, 1.0, 0.7);
glScalef(35.0, 0.3, 28.0);
glTranslatef(0.0, 0.5, 1.0);
glutSolidCube(1.0);
glPopMatrix();


//meja jajan1
glPushMatrix();
glRotatef(90, 0.0, 1.0, 0.0);
glTranslatef(30.3, 0.5, -33.2);
glColor3f(1.0, 0.0, 0.0);
glScalef(-4.5, 3.0, 12.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//meja jajan2
glPushMatrix();
glRotatef(45, 0.0, 0.0, 0.0);
glTranslatef(-11.4, 0.7, -29.5);
glColor3f(1.0, 0.0, 0.0);
glScalef(-3.5, 4.0, 17.0);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//meja jajan3
glPushMatrix();
glRotatef(45, 0.0, 0.0, 0.0);
glTranslatef(-1.5, 0.7, -22.2);
glColor3f(1.0, 0.0, 0.0);
glScalef(-3.5, 4.0, 10.5);
glTranslatef(6.0, 0.0, 1.1);
glutSolidCube(0.7);
glPopMatrix();

//gelas1
glPushMatrix();
          glTranslatef(-18.26,2.85, -3.55);
          glRotatef(90, 1,0,0);
          glScalef(0.6,0.5,0.7);
          glColor3f(0.0, 1.0, 0.0);
          glutSolidCone(0.32, 1.26, 100, 25);
          glPopMatrix();
glPushMatrix();
          glTranslatef(-18.26, 1.85, -3.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.1);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidCone(0.22, 1.26, 100, 25);
glPopMatrix();

//gelas2
glPushMatrix();
          glTranslatef(-17.26,2.85, -3.55);
          glRotatef(90, 1,0,0);
          glScalef(0.6,0.5,0.7);
          glColor3f(0.0, 1.0, 0.0);
          glutSolidCone(0.32, 1.26, 100, 25);
          glPopMatrix();
glPushMatrix();
          glTranslatef(-17.26, 1.85, -3.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.1);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidCone(0.22, 1.26, 100, 25);
glPopMatrix();

//gelas3
glPushMatrix();
          glTranslatef(-19.26, 2.85, -3.55);
          glRotatef(90, 1,0,0);
          glScalef(0.6,0.5,0.7);
          glColor3f(0.0, 1.0, 0.0);
          glutSolidCone(0.32, 1.26, 100, 25);
          glPopMatrix();
glPushMatrix();
          glTranslatef(-19.26, 1.85, -3.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.1);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidCone(0.22, 1.26, 100, 25);
glPopMatrix();

//gelas4
glPushMatrix();
          glTranslatef(-20.26,2.85, -3.55);
          glRotatef(90, 1,0,0);
          glScalef(0.6,0.5,0.7);
          glColor3f(0.0, 1.0, 0.0);
          glutSolidCone(0.32, 1.26, 100, 25);
          glPopMatrix();
glPushMatrix();
          glTranslatef(-20.26, 1.85, -3.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.1);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidCone(0.22, 1.26, 100, 25);
glPopMatrix();

//gelas5
glPushMatrix();
          glTranslatef(-21.26,2.85, -3.55);
          glRotatef(90, 1,0,0);
          glScalef(0.6,0.5,0.7);
          glColor3f(0.0, 1.0, 0.0);
          glutSolidCone(0.32, 1.26, 100, 25);
          glPopMatrix();
glPushMatrix();
          glTranslatef(-21.26, 1.85, -3.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.1);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidCone(0.22, 1.26, 100, 25);
glPopMatrix();

//popcorn
glPushMatrix();
          glTranslatef(-18.26, 2.65, -2.55);
          glRotatef(90, 1,0,0);
          glScalef(0.5,0.5,1.5);
          glColor3f(0.0, 1.0, 1.0);
          glutSolidCube(0.8);
          glPopMatrix();
glPushMatrix();
          glTranslatef(-18.24, 2.35, -2.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.5);
          glColor3f(1.0, 1.0, 1.0);
          glutSolidSphere(0.15, 100, 25);
glPopMatrix();
glPushMatrix();
          glTranslatef(-18.10, 3.35, -2.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.5);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidSphere(0.22, 100, 25);
glPopMatrix();
glPushMatrix();
          glTranslatef(-18.06, 3.35, -2.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.5);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidSphere(0.22, 100, 25);
glPopMatrix();
glPushMatrix();
          glTranslatef(-18.16, 3.35, -2.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.5);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidSphere(0.22, 100, 25);
glPopMatrix();
glPushMatrix();
          glTranslatef(-18.26, 3.35, -2.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.5);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidSphere(0.22, 100, 25);
glPopMatrix();
glPushMatrix();
          glTranslatef(-18.36, 3.35, -2.55);
          glRotatef(-90, 1,0,0);
          glScalef(1.0, 0.5, 0.5);
          glColor3f(0.0, 0.0, 1.0);
          glutSolidSphere(0.22, 100, 25);
glPopMatrix();

//meja tatakan
glPushMatrix();
          glTranslatef(-17.26, 2.85, -2.55);
          glRotatef(0.0, 1,0,0);
          glScalef(1.0,1.0,1.0);
          glColor3f(0.0, 1.0, 0.0);
          botol();
          glPopMatrix();
glPushMatrix();
          glTranslatef(-21.16, 0.5, -10.25);
          glRotatef(0.0, 1,0,0);
          glScalef(0.6,1.0,1.0);
          glColor3f(0.0, 1.0, 0.0);
          tatakan();
          glPopMatrix();
//botol
glPushMatrix();
          glTranslatef(-21.6, 2.85, -10.55);
          glRotatef(0.0, 1,0,0);
          glScalef(1.0,1.0,1.0);
          glColor3f(0.0, 1.0, 0.0);
          botol();
          glPopMatrix();
          glPushMatrix();
          glTranslatef(-21.6, 2.85, -10.0);
          glRotatef(0.0, 1,0,0);
          glScalef(1.0,1.0,1.0);
          glColor3f(0.0, 1.0, 0.0);
          botol();
          glPopMatrix();
          glPushMatrix();
          glTranslatef(-21.0, 2.85, -10.55);
          glRotatef(0.0, 1,0,0);
          glScalef(1.0,1.0,1.0);
          glColor3f(0.0, 1.0, 0.0);
          botol();
          glPopMatrix();
          glPushMatrix();
          glTranslatef(-21.0, 1.25, -10.55);
          glRotatef(0.0, 1,0,0);
          glScalef(1.0,1.0,1.0);
          glColor3f(0.0, 1.0, 0.0);
          botol();
          glPopMatrix();
          glTranslatef(-21.6, 1.25, -10.55);
          glRotatef(0.0, 1,0,0);
          glScalef(1.0,1.0,1.0);
          glColor3f(0.0, 1.0, 0.0);
          botol();
          glPopMatrix();

}


void display(void)
{
glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glEnable(GL_COLOR_MATERIAL); //mengaktifkan fungsi untuk memberikan warna pada material
//glColorMaterial(GL_FRONT, GL_DIFFUSE); //parameter-parameter fungsi warna material


glPushMatrix();
glTranslatef(n*1.0, 0.0, m*1.0);
glRotatef(c*10, 0.0, 1.0, 0.0);
glScalef(z, z, z);
gedung();
glPopMatrix();

glPopMatrix();
glDisable(GL_COLOR_MATERIAL); //menonaktifkan fungsi untuk memberikan warna pada material
glFlush ();
}
void reshape (int w, int h)
{
glViewport (0, 0, (GLsizei) w, (GLsizei) h);
glMatrixMode (GL_PROJECTION);
glLoadIdentity();
if (w <= h)
glOrtho (-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
5.0*(GLfloat)h/(GLfloat)w, -30.0, 30.0);
else
glOrtho (-5.0*(GLfloat)w/(GLfloat)h,
5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -30.0, 30.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt (1.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
int main(int argc, char** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize (1200, 500);
glutInitWindowPosition (100, 100);
glutCreateWindow ("Studio Konser Musik");
init ();
glutDisplayFunc(display);
glutReshapeFunc(reshape);
glutKeyboardFunc(keyboard);
//glutSpecialFunc(specialkey);
glutIdleFunc(rotate);
glutMainLoop();
return 0;
}
