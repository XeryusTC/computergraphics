#include "bool.h"
#include "input.h"
#include <stdio.h>

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
