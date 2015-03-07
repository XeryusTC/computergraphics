#ifndef SCENE_H
#define SCENE_H

typedef enum SCENE {
	DEFAULT_SCENE,
	SCENE01,
    MESH,
} SCENE;

void displayDefault(void);
void reshapeDefault(int w, int h);

void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n);
void displayScene01(void);
void reshapeScene01(int w, int h);

#endif // SCENE_H
