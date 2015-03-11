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
#include "glslshaders.h"
#include "scene.h"
#include "mesh.h"


#define APERTURESAMPLES 8
/* Globals galore */
MouseInfo mouse;
ScreenInfo screen;

CONTROL_MODE rotate_mode;
SCENE scene;
float rotx=0, roty=0,
      aperatureRadius,
      focalDistance;
Camera cam;

float deltaXs[APERTURESAMPLES], deltaYs[APERTURESAMPLES];
float invApertureSamples = (float) 1 / APERTURESAMPLES;

extern VBOData model;
extern bool remove_duplicates;

void initDOFoffsetValues()
{
    int   i               = 0;
    float goldenAngle     = 137.508,
          c, th, r;
    c  = aperatureRadius/sqrt(APERTURESAMPLES);

    for (i = 0; i < APERTURESAMPLES; i++){
         th         = i*goldenAngle;
         r          = c*sqrt(i);
         deltaXs[i] = sin(th)*r;
         deltaYs[i] = cos(th)*r;
    }
}

void drawScene(SCENE scene)
{
    switch (scene) {
	case DEFAULT_SCENE:
		displayDefaultScene();
        break;
	case SCENE01:
		displayScene01();
        break;
    case MESH:
		displayMesh();
        break;
    }
}

void displayDOF(void)
{
    glClear(GL_ACCUM_BUFFER_BIT);
    int   i                  = 0;
    float lookAtX, lookAtY, lookAtZ;

    glLoadIdentity();
    cameraLookDirVector(cam, &lookAtX, &lookAtY, &lookAtZ);

    lookAtX += cam.x;
    lookAtY += cam.y;
    lookAtZ += cam.z - focalDistance;

    // Do once for each DOF sampling round
    for (i = 0; i < APERTURESAMPLES; i++){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
		gluLookAt(cam.x+deltaXs[i],   cam.y+deltaYs[i],   cam.z,
				lookAtX, lookAtY, lookAtZ,
                0.0,     1.0,     0.0);

        drawScene(scene);
        glFlush();
        glAccum(GL_ACCUM, invApertureSamples);
   }

   if (rotate_mode == MODE_FPS)
        resetMousePosition();

   glAccum(GL_RETURN, 1.0);
   glutSwapBuffers();
   glutPostRedisplay();
}


void displayDefault(void)
{
   float lookAtX, lookAtY, lookAtZ;

   glLoadIdentity();
   cameraLookDirVector(cam, &lookAtX, &lookAtY, &lookAtZ);

   lookAtX += cam.x;
   lookAtY += cam.y;
   lookAtZ += cam.z - focalDistance;

   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   gluLookAt(cam.x,   cam.y,   cam.z,
           lookAtX, lookAtY, lookAtZ,
           0.0,     1.0,     0.0);

   drawScene(scene);
   glFlush();

   if (rotate_mode == MODE_FPS)
        resetMousePosition();

   glutSwapBuffers();
   glutPostRedisplay();
}

int main(int argc, char** argv)
{
  #if defined(NEED_GLEW)
    GLenum err;
  #endif

    char *modelfile;

    // Set default options
    rotate_mode = MODE_ROTATE_CLICK;
    scene = DEFAULT_SCENE;//SCENE01
    aperatureRadius=0.05;
    focalDistance=10;
    bool dof = false;
    screen.width=800;
    screen.height=600;
    // Parse command line arguments
    int  i;
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
                aperatureRadius=10;
                focalDistance=1000;
        }
        // Depth Of Field
        else if (strcmp(argv[i], "-d")==0 || strcmp(argv[i], "--dof")==0) {
                initDOFoffsetValues();
                dof = true;
	}
        // Meshes
        else if (strcmp(argv[i], "-m")==0 || strcmp(argv[i], "--mesh")==0) {
            scene = MESH;
            modelfile = argv[i+1];
            //focalDistance = 0;
        } else if (strcmp(argv[i], "-r")==0 || strcmp(argv[i], "--remove-duplicates")==0) {
            remove_duplicates = true;
        }
    }

    // Create window
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ACCUM);
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
    glClearAccum(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);

    /* Register GLUT callback functions */
    switch (scene) {
	case DEFAULT_SCENE:
    	        glShadeModel(GL_FLAT);
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
		initGLSLProgram("vertexshader.glsl", "fragmentshader.glsl");
    	        glShadeModel(GL_SMOOTH);
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
    case MESH:
        glShadeModel(GL_SMOOTH);
        glutReshapeFunc(reshapeMesh);

		model = glmInitVBO(modelfile);
        // Setup light
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        // Setup camera
        cam.x = 0.0;
        cam.y = 0.0;
        cam.z = 500.0;
        cam.pitch = 0.0;
        cam.yaw = 180.0;
        cam.roll = 0.0;
        break;
    }

    if (dof == true)
        glutDisplayFunc(displayDOF);
    else
        glutDisplayFunc(displayDefault);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);

    mouse.calculateDelta=false;

    glutMainLoop();

    // Cleanup
    if (scene == MESH) {
        free(modelfile);
		destroyVBOData(&model);
    }

    return 0;
}
