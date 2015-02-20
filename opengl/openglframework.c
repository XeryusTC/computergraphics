/*
* An OpenGL template/framework file for the Computer Graphics course
* at the University of Groningen.
*/

// If windows is used, windows.h should be included (before gl.h and glu.h)
#if defined(_WIN32)
#include <windows.h>
#endif

// If you use GLUT you do not need to include gl.h and glu.h
// as glut.h guarantees that gl.h and glu.h are properly
// included.
// So if you do not wish to use  glut.h, uncomment the following lines.
//#include <GL/gl.h>
//#include <GL/glu.h>

#if defined(NEED_GLEW)
#include "glew.h"
#endif

// Apparently Mac OS X puts the GLUT headers somewhere different.
// For windows we use freeglut.
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
#include <string.h>

#include "bool.h"
#include "camera.h"
#include "input.h"
#include "scene.h"

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

/* Globals galore */
MouseInfo mouse;
ScreenInfo screen;

CONTROL_MODE rotate_mode;
SCENE scene;
float rotx=0, roty=0;
Camera cam;

void displayDefault(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f,0.0f,1.0f);
	cameraLookAt(cam);

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

	if (rotate_mode == MODE_FPS)
		resetMousePosition();

    glutSwapBuffers();
    glutPostRedisplay();
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

int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
    GLenum err;
#endif

	// Set default options
	rotate_mode = MODE_ROTATE_CLICK;
	scene = DEFAULT_SCENE;
	screen.width=800;
	screen.height=600;
	// Parse command line arguments
	int i;
	for (i=1; i<argc; ++i) {
		// Control modes
		if (strcmp(argv[i], "-c")==0 || strcmp(argv[i], "--click")==0) {
			rotate_mode = MODE_ROTATE_CLICK;
		} else if (strcmp(argv[i], "-f")==0 || strcmp(argv[i], "--fps")==0) {
			rotate_mode = MODE_FPS;
		} else if (strcmp(argv[i], "-p")==0 || strcmp(argv[i], "--passive")==0) {
			rotate_mode = MODE_ROTATE_PASSIVE;
		}
		// Scene
		else if (strcmp(argv[i], "-1")==0 || strcmp(argv[i], "--scene01")==0) {
			scene = SCENE01;
			screen.width=400;
			screen.height=400;
		}
	}

	// Create window
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screen.width, screen.height);
    glutInitWindowPosition(220,100);
    glutCreateWindow("Computer Graphics - OpenGL framework");
	if (rotate_mode == MODE_FPS)
		glutSetCursor(GLUT_CURSOR_NONE);

#if defined(NEED_GLEW)
    /* Init GLEW if needed */
    err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(1);
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

    /* Select clearing (background) color */
    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);

    /* Register GLUT callback functions */
	switch (scene) {
	case DEFAULT_SCENE:
    	glShadeModel(GL_FLAT);
	    glutDisplayFunc(displayDefault);
    	glutReshapeFunc(reshapeDefault);

		// Setup camera
		cam.x = 0.0;
		cam.y = 0.0;
		cam.z = 5.0;
		cam.pitch = 0.0;
		cam.yaw = 180.0;
		cam.roll = 0.0;

		break;
	case SCENE01:
    	glShadeModel(GL_SMOOTH);
		glutDisplayFunc(displayScene01);
		glutReshapeFunc(reshapeScene01);

		// Setup light
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// Setup camera
		cam.x = 200.0;
		cam.y = 200.0;
		cam.z = 1000.0;
		cam.pitch = 0.0;
		cam.yaw = 180.0;
		cam.roll = 0.0;
		break;
	}

	glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);

	mouse.calculateDelta=false;

    glutMainLoop();

    return 0;
}
