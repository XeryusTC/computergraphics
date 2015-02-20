#ifndef MOUSE_H
#define MOUSE_H

#include "bool.h"

/* MOUSE */
typedef struct MouseInfo {
	int lastx, lasty;
	bool calculateDelta;
	bool leftDown;
	bool rightDown;
} MouseInfo;

void mouseDelta(MouseInfo *mouse, int x, int y, int *dx, int *dy);

/* SCREEN */
typedef struct ScreenInfo {
	int width, height;
} ScreenInfo;

#endif // MOUSE_H
