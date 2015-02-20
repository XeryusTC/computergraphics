#ifndef MOUSE_H
#define MOUSE_H

#include "bool.h"

typedef enum CONTROL_MODE {
	MODE_ROTATE_CLICK,
	MODE_ROTATE_PASSIVE,
	MODE_FPS,
} CONTROL_MODE;


/* MOUSE */
typedef struct MouseInfo {
	int lastx, lasty;
	bool calculateDelta;
	bool leftDown;
	bool rightDown;
} MouseInfo;

void mouseDelta(MouseInfo *mouse, int x, int y, int *dx, int *dy);
void mouseCallback(int button, int state, int x, int y);
void motion(int x, int y);
void passiveMotion(int x, int y);

/* KEYBOARD */
void keyboard(unsigned char key, int x, int y);

/* SCREEN */
typedef struct ScreenInfo {
	int width, height;
} ScreenInfo;

#endif // MOUSE_H
