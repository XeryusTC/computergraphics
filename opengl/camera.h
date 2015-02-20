#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera {
	float x, y, z, pitch, yaw, roll;
} Camera;

void cameraLookDirVector(Camera cam, float *x, float *y, float *z);
void cameraMoveLeft(Camera *cam, float distance);
void cameraMoveRight(Camera *cam, float distance);
void cameraMoveForward(Camera *cam, float distance);
void cameraMoveBackward(Camera *cam, float distance);
void cameraMoveUp(Camera *cam, float distance);
void cameraMoveDown(Camera *cam, float distance);

#endif // CAMERA_H
