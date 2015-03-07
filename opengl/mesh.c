#if defined(__APPLE__)&& defined(__MACH__)
    #include <GLUT/glut.h>
#elif defined(_WIN32)
    #include <GL/freeglut.h>
#else
    #include <GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>

#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "glm.h"

extern ScreenInfo screen;
extern Camera cam;
extern CONTROL_MODE rotate_mode;
extern float rotx, roty;

GLMmodel *obj;
GLfloat model_light_pos[4] = {100.0, 600.0, 200.0, 0.0};
GLfloat model_light_color[3] = {1.0, 1.0, 1.0};

GLuint vboId, vboIndices;

void displayMesh(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	cameraLookAt(cam);

	// Set rotation
	glMatrixMode(GL_MODELVIEW);
	glRotatef(rotx, 1.0, 0.0, 0.0);
	glRotatef(roty, 0.0, 1.0, 0.0);

	// Add light
	glLightfv(GL_LIGHT0, GL_POSITION, model_light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  model_light_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  model_light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, model_light_color);

    if (obj)
        glmDraw(obj, GLM_SMOOTH);

    if (rotate_mode == MODE_FPS)
		resetMousePosition();

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshapeMesh(int w, int h)
{
	screen.width = w;
	screen.height = h;
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLdouble)w/(GLdouble)h,1.5,1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void loadModel(char *filename)
{
    if (obj)
        unloadModel();
    obj = glmReadOBJ(filename);
    glmFacetNormals(obj);
    glmVertexNormals(obj, 90.0);
    glmUnitize(obj);
    glmScale(obj, 200.0);
}

void unloadModel(void)
{
    glmDelete(obj);
}

