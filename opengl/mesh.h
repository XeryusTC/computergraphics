#ifndef MESH_H
#define MEHS_H

void displayMesh(void);
void reshapeMesh(int w, int h);
void loadModel(char *filename);
void unloadModel(void);

void glmInitVBO(char *filename);
void glmDrawVBO(void);
void glmDestroyVBO(void);

#endif // MESH_H
