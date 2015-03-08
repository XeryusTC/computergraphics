#ifndef MESH_H
#define MEHS_H

#include "bool.h"

typedef struct VBOData {
	int front, size;
	int indexFront, indexSize;
	GLuint *i;
	GLfloat *v;
	GLfloat *n;
	bool dataAvailable;
	GLuint vertexVboId;
	GLuint indexVboId;
} VBOData;

// Interface
VBOData createVBOData(int s);
void createVBOFromVBOData(VBOData *d);
void destroyVBOData(VBOData *d);
void insertUniqueVBOData(VBOData *d, GLfloat *vertex, GLfloat *normal);
// Utility functions
void bindVBOData(VBOData *d);
void freeVBOData(VBOData *d);
void doubleVBODataSize(VBOData *d);
void doubleVBODataIndexSize(VBOData *d);
void insertVertexVBOData(VBOData *d, GLfloat *vertex, GLfloat *normal);
void insertIndexVBOData(VBOData *d, GLuint index);
GLint vertexIsInVBOData(VBOData *d, GLfloat *vertex, GLfloat *normal);

void displayMesh(void);
void reshapeMesh(int w, int h);
void loadModel(char *filename);
void unloadModel(void);

VBOData glmInitVBO(char *filename);
void glmDrawVBO(VBOData *d);
void glmDestroyVBO(void);

#endif // MESH_H
