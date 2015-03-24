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



// CONSTANTS 
extern ScreenInfo screen;
extern Camera cam;
extern CONTROL_MODE rotate_mode;
extern float rotx, roty;

extern GLfloat light_pos[4];// = {-1000.0, 600.0, 400.0, 0.0};
extern GLfloat light_color[3];// = {1.0, 1.0, 1.0};

float y_ball_1 = 100, y_ball_2 = 300, y_ball_3 = 500, vY_ball_1 = 0, vY_ball_2 = 0, vY_ball_3 = 0;


void bounce(float time, float r, float *y, float *vY)
{
	if (abs(*y - r) < 0.1 && abs(*vY) < 0.1)
		return;
	float 	dvY = -9.81 * time,
		vA = *vY + 0.5 * dvY;
	*y  += vA * time;
	*vY += dvY;
	if (*y < r){
		float 	dY = r - *y,
			dT = dY / abs(vA);
		*y = r + dT * abs(vA) * 0.7;
		*vY *= -0.7;
	}
}



void displayScene02(void) {
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


    //Ball 1 (RED)
    setGlMaterial(1.0f,0.0f,0.0f,0.2,0.7,0.5,64);
    glPushMatrix();
    bounce (0.1, 50, &y_ball_1, &vY_ball_1);
    glTranslated(90,y_ball_1,100);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    //Ball 2 (GREEN)
    setGlMaterial(0.0f,1.0f,0.0f,0.2,0.3,0.5,8);
    glPushMatrix();
    bounce (0.1, 50, &y_ball_2, &vY_ball_2);
    glTranslated(210,y_ball_2,300);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();
    
    //Ball 3 (BLUE)
    setGlMaterial(0.0f,0.0f,1.0f,0.2,0.7,0.8,32);
    glPushMatrix();
    bounce (0.1, 50, &y_ball_3, &vY_ball_3);
    glTranslated(290,y_ball_3,150);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    // Ground square
    setGlMaterial(0.5f,0.5f,0.5f,0.2,0.8,0.5,32);
	glBegin(GL_QUADS);
		glVertex3f(  1000.0,  0.0,  1000.0);
		glVertex3f(  1000.0,  0.0, -1000.0);
		glVertex3f( -1000.0,  0.0, -1000.0);
		glVertex3f( -1000.0,  0.0,  1000.0);
	glEnd();
    glPopMatrix();
}


void reshapeScene02(int w, int h)
{
	screen.width = w;
	screen.height = h;
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(2.0*atan2(h/2.0,1000.0)*180.0/M_PI,(GLdouble)w/(GLdouble)h,100,2000);
    glMatrixMode(GL_MODELVIEW);
}

