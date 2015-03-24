#if defined(__APPLE__) && defined(__MACH__)
    #include <GLUT/glut.h>
#elif defined(_WIN32)
    #include <GL/freeglut.h>
#else
    #include <GL/glut.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "mesh.h"
#include "glm.h"
#include "texture.h"
#include "input.h"
#include "scene.h"

extern ScreenInfo screen;
extern float rotx, roty;
extern CONTROL_MODE rotate_mode;

GLfloat balls_light_pos[4] = {500.0, 300.0, 200.0, 0.0};
GLfloat balls_light_color[3] = {1.0, 1.0, 1.0};

extern GLMmodel *obj;
VBOData b1, b2, b3;

GLuint lasttime=0;
GLuint timestep=0;

GLfloat b1x, b1y, b1rot, b2y, b2z, b2rot, b3x, b3y, b3z, b3rot;

void initBouncingBalls()
{
    b1 = glmInitVBOTexture("obj/sphere.obj", 50.0, "earth.png");
    b2 = glmInitVBOTexture("obj/sphere.obj", 100.0, "bluegrid.png");
    b3 = glmInitVBOTexture("obj/sphere.obj", 75.0, "basketball.png");
}

void idleBouncingBalls()
{
    GLfloat t;
    GLuint delta = glutGet(GLUT_ELAPSED_TIME) - lasttime;
    if (lasttime == 0)
        delta = 0;
    lasttime = glutGet(GLUT_ELAPSED_TIME);
    timestep = (timestep + delta) % 10000;
    t = timestep * 0.006;

    b1y = fabs(sin(t) / exp((t) * 0.3)) * 100.0;
    b1x = t * 10 - 300.0;
    b1rot -= t * 0.05;

    b2y = fabs(sin(t) / exp((t) * 0.1)) * 150.0;
    b2z = t * 25 - 250;
    b2rot += t * 0.05;

    b3y = fabs(sin(t) / exp((t) * 0.2)) * 300.0;
    b3x = t * 10 + 130;
    b3z = -t * 5 + 50;
    b3rot -= t * 0.05;
}

void reshapeBouncingBalls(int w, int h)
{
    screen.width = w;
    screen.height = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)w/(GLdouble)h, 1.5, 3000.0);
    glMatrixMode(GL_MODELVIEW);
}

void displayBouncingBalls(void)
{
    glMatrixMode(GL_MODELVIEW);
    glRotatef(rotx, 1.0, 0.0, 0.0);
    glRotatef(roty, 0.0, 1.0, 0.0);

    glLightfv(GL_LIGHT0, GL_POSITION, balls_light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  balls_light_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  balls_light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  balls_light_color);

    glPushMatrix();
    glTranslatef(b1x, b1y + 50.0, 0);
    glRotatef(b1rot, 0.0, 0.0, 1.0);
    glmDrawVBO(&b1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, b2y + 100, b2z);
    glRotatef(b2rot, 1.0, 0.0, 0.0);
    glmDrawVBO(&b2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(b3x, b3y + 75, b3z);
    glRotatef(b3rot, 0.5, 0.0, 1.0);
    glmDrawVBO(&b3);
    glPopMatrix();

    // Floor
    setGlMaterial(0.5, 0.5, 0.5, 0.2, 0.8, 0.5, 64);
    glBegin(GL_QUADS);
        glVertex3f( 1000.0, 0.0,  2000.0);
        glVertex3f( 1000.0, 0.0, -1000.0);
        glVertex3f(-1000.0, 0.0, -1000.0);
        glVertex3f(-1000.0, 0.0,  2000.0);
    glEnd();
}
