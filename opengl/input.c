#if defined(__APPLE__)&& defined(__MACH__)
	#include <GLUT/glut.h>
#elif defined(_WIN32)
	#include <GL/freeglut.h>
#else
	#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "bool.h"
#include "camera.h"
#include "input.h"

extern MouseInfo mouse;
extern ScreenInfo screen;
extern CONTROL_MODE rotate_mode;
extern float rotx, roty;
extern const float rotscale;
extern Camera cam;

const float walkspeed=.5;
const float rotscale=.5;
const float fpsrotscale=.2;
const float zoomdelta=3;

void mouseDelta(MouseInfo *mouse, int x, int y, int *dx, int *dy)
{
	if (mouse->calculateDelta == false) {
		*dx = 0;
		*dy = 0;
		mouse->calculateDelta = true;
	} else {
	    *dx = x - mouse->lastx;
    	*dy = y - mouse->lasty;
	}
    mouse->lastx = x;
    mouse->lasty = y;
}

void motion(int x, int y)
{
    int dx, dy;
    mouseDelta(&mouse, x, y, &dx, &dy);
	if (rotate_mode == MODE_ROTATE_CLICK && mouse.leftDown) {
		// Moving mouse in x direction rotates around y axis
		roty += dx * rotscale;
		rotx += dy * rotscale;
	}
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
		case 'a': case 'A':
			cameraMoveLeft(&cam, walkspeed);
			break;
		case 'd': case 'D':
			cameraMoveRight(&cam, walkspeed);
			break;
		case 's': case 'S':
			if (rotate_mode == MODE_FPS)
				cameraMoveBackward(&cam, walkspeed);
			break;
		case 'w': case 'W':
			if (rotate_mode == MODE_FPS)
				cameraMoveForward(&cam, walkspeed);
			break;
		case 'e': case 'E':
			if (rotate_mode == MODE_FPS)
				cameraMoveUp(&cam, walkspeed);
			break;
        case 'q': case 'Q':
			// use Q to move up in FPS mode
			if (rotate_mode == MODE_FPS) {
				cameraMoveDown(&cam, walkspeed);
				break;
			}
			// use Q to exit when not in FPS mode
        case 27: // ESC key
            printf("Exiting...\n");
            exit(0);
            break;
    }
}

void mouseCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		mouse.leftDown = (state == GLUT_DOWN);
	} else if (button == 3 && rotate_mode != MODE_FPS) {
		cameraMoveForward(&cam, zoomdelta);
	} else if (button == 4 && rotate_mode != MODE_FPS) {
		cameraMoveBackward(&cam, zoomdelta);
	} else {
		printf("Unhandled mouse event: %d %d %d %d\n", button, state, x, y);
	}
}

void passiveMotion(int x, int y)
{
    int dx, dy;
    mouseDelta(&mouse, x, y, &dx, &dy);
	if (rotate_mode == MODE_ROTATE_PASSIVE) {
		// Moving mouse in x direction rotates around y axis
		roty += dx * rotscale;
		rotx += dy * rotscale;
	} else if (rotate_mode == MODE_FPS) {
		cam.yaw   -= dx * fpsrotscale;
		cam.pitch -= dy * fpsrotscale;
	}
}

void resetMousePosition()
{
	mouse.calculateDelta=false;
	glutWarpPointer(screen.width/2, screen.height/2);
}
