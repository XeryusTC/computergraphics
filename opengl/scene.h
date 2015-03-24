#ifndef SCENE_H
#define SCENE_H

typedef enum SCENE {
	DEFAULT_SCENE,
	SCENE01,
	SCENE02,
    MESH,
} SCENE;

void displayDefaultScene(void);
void reshapeDefault(int w, int h);

void drawScene(SCENE scene);
void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n);
void displayScene01(void);
void reshapeScene01(int w, int h);
void displayScene02(void);
void reshapeScene02(int w, int h);


#endif // SCENE_H
