#if defined(__APPLE__)&& defined(__MACH__)
	#include <GLUT/glut.h>
#elif defined(_WIN32)
	#include <GL/freeglut.h>
#else
	#include <GL/glut.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "camera.h"
#include "input.h"
#include "scene.h"
//#include "mesh.h"

#define SPHERE_N (20)



/* CONSTANTS */
GLfloat cubeVertices[8*3] = {-1,-1,-1, -1,-1, 1, -1, 1,-1,  1,-1,-1, -1, 1, 1,  1,-1, 1,  1, 1,-1,  1, 1, 1};
GLubyte cubeFaces[3*12] = {
        0,2,1, 4,2,1, // left face
        3,5,7, 3,6,7, // right face
        0,1,5, 0,3,5, // bottom face
        4,2,6, 7,4,6, // top face
        2,0,3, 2,6,3, // back face
        5,1,4, 5,7,4, // front face
    };
GLfloat cubeFaceColors[3*8] = {0,0,1, 0,1,0, 0,1,1, 1,0,0, 1,0,1, 1,1,0, 1,1,1, .5,.5,.5};

//GLfloat model_light_pos[4] = {100.0, 600.0, 200.0, 0.0};
//GLfloat model_light_color[3] = {1.0, 1.0, 1.0};


extern ScreenInfo screen;
extern Camera cam;
extern CONTROL_MODE rotate_mode;
extern float rotx, roty;

GLfloat light_pos[4] = {-200.0, 600.0, 1500.0, 0.0};
GLfloat light_color[3] = {1.0, 1.0, 1.0};

void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n)
{
    GLfloat ambient[] = {ka*r,ka*g,ka*b,1.0};
    GLfloat diffuse[] = {kd*r,kd*g,kd*b,1.0};
    GLfloat specular[] = {ks,ks,ks,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, n);
}

void displayScene01(void) {
	// Set rotation
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(200.0, 200.0, 200.0);
	glRotatef(rotx, 1.0, 0.0, 0.0);
	glRotatef(roty, 0.0, 1.0, 0.0);
	glTranslatef(-200.0, -200.0, -200.0);

    // Add light
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

    setGlMaterial(0.0f,0.0f,1.0f,0.2,0.7,0.5,64);
    glPushMatrix();
    glTranslated(90,320,100);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    setGlMaterial(0.0f,1.0f,0.0f,0.2,0.3,0.5,8);
    glPushMatrix();
    glTranslated(210,270,300);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    setGlMaterial(1.0f,0.0f,0.0f,0.2,0.7,0.8,32);
    glPushMatrix();
    glTranslated(290,170,150);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    setGlMaterial(1.0f,0.8f,0.0f,0.2,0.8,0.0,1);
    glPushMatrix();
    glTranslated(140,220,400);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    setGlMaterial(1.0f,0.5f,0.0f,0.2,0.8,0.5,32);
    glPushMatrix();
    glTranslated(110,130,200);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();
}

void displayDefaultScene(void) {
	// Set rotation
    glMatrixMode(GL_MODELVIEW);
    glRotatef(rotx, 1.0, 0.0, 0.0);
    glRotatef(roty, 0.0, 1.0, 0.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
    glColorPointer( 3, GL_FLOAT, 0, cubeFaceColors);

    // draw a cube
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeFaces);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void reshapeScene01(int w, int h)
{
	screen.width = w;
	screen.height = h;
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(2.0*atan2(h/2.0,1000.0)*180.0/M_PI,(GLdouble)w/(GLdouble)h,100,1000);
    glMatrixMode(GL_MODELVIEW);
}

void reshapeDefault(int w, int h)
{
    screen.width = w;
    screen.height = h;
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLdouble)w/(GLdouble)h,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
}

