#if defined(__APPLE__)&& defined(__MACH__)
    #include <GLUT/glut.h>
#elif defined(_WIN32)
    #include <GL/freeglut.h>
#else
    #include <GL/glut.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "glm.h"

#define USE_GLM_NOT_VBO

const int glfSize = sizeof(GLfloat);

extern ScreenInfo screen;
extern Camera cam;
extern CONTROL_MODE rotate_mode;
extern float rotx, roty;

GLMmodel *obj;
VBOData model;
GLfloat model_light_pos[4] = {100.0, 600.0, 200.0, 0.0};
GLfloat model_light_color[3] = {1.0, 1.0, 1.0};

GLuint vboId;
bool remove_duplicates=false;

extern GLuint myTexture;

void displayMesh(void)
{
	// Set rotation
	glMatrixMode(GL_MODELVIEW);
	glRotatef(rotx, 1.0, 0.0, 0.0);
	glRotatef(roty, 0.0, 1.0, 0.0);

	// Add light
	glLightfv(GL_LIGHT0, GL_POSITION, model_light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  model_light_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  model_light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, model_light_color);

    // Setup texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, myTexture);
#ifdef USE_GLM_NOT_VBO
    glmDraw(obj, GLM_TEXTURE);
#else
    glmDrawVBO(&model);
#endif
    glDisable(GL_TEXTURE_2D);

    if (rotate_mode == MODE_FPS)
		resetMousePosition();
}

void reshapeMesh(int w, int h)
{
	screen.width = w;
	screen.height = h;
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLdouble)w/(GLdouble)h,1.5,1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void loadModel(char *filename)
{
    if (obj)
        unloadModel();
    obj = glmReadOBJ(filename);
    glmFacetNormals(obj);
    glmVertexNormals(obj, 90.0);
    glmUnitize(obj);
    glmScale(obj, 100.0);
    glmSpheremapTexture(obj);
}

void unloadModel(void)
{
    glmDelete(obj);
}

VBOData glmInitVBO(char *filename)
{
    int i;
	VBOData d;
    // Destroy an old model if it is still loaded
    if (obj)
		unloadModel();

    loadModel(filename);

	// Start by creating a VBO
	d = createVBOData(obj->numtriangles);

	// Convert triangles to VBOData
	d.dataAvailable = true;
	for (i=0; i<obj->numtriangles; ++i) {
		insertUniqueVBOData(&d, &obj->vertices[obj->triangles[i].vindices[0] * 3],
				&obj->normals[obj->triangles[i].nindices[0] * 3]);
		insertUniqueVBOData(&d, &obj->vertices[obj->triangles[i].vindices[1] * 3],
				&obj->normals[obj->triangles[i].nindices[1] * 3]);
		insertUniqueVBOData(&d, &obj->vertices[obj->triangles[i].vindices[2] * 3],
				&obj->normals[obj->triangles[i].nindices[2] * 3]);
	}

	printf("Triangle count:    %d\n", obj->numtriangles);
	printf("Normal count:      %d\n", obj->numnormals);
	printf("Expected vertices: %d\n", obj->numtriangles*3);
	printf("Actual vertices:   %d\n", d.front);
	printf("Stored indices:    %d\n", d.indexFront);

	// Create the actual VBO
	createVBOFromVBOData(&d);

	return d;
}

void glmDrawVBO(VBOData *d)
{
	bindVBOData(d);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, d->front * glfSize * 3);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if (remove_duplicates) {
        glDrawElements(GL_TRIANGLES, d->indexFront*3, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, d->indexFront);
    }

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void glmDestroyVBO(void)
{
    glDeleteBuffersARB(1, &vboId);
}

VBOData createVBOData(int s)
{
	VBOData d;
	d.v = malloc(s * glfSize * 3);
	d.n = malloc(s * glfSize * 3);
	d.i = malloc(s * sizeof(GLuint));
	assert(d.v != NULL);
	assert(d.n != NULL);
	assert(d.i != NULL);
	d.front = d.indexFront = 0;
	d.size = d.indexSize = s;
	d.dataAvailable = false;
	return d;
}

void createVBOFromVBOData(VBOData *d)
{
	glGenBuffers(1, &d->vertexVboId);
	glGenBuffers(1, &d->indexVboId);

	bindVBOData(d);
	// Copy data into the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, d->front*2*3*glfSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, d->front*3*glfSize, d->v);
	glBufferSubData(GL_ARRAY_BUFFER, d->front*3*glfSize, d->front*3*glfSize, d->n);
	// Copy data into the index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, d->indexFront*sizeof(GLuint), d->i, GL_STATIC_DRAW);

	// Remove the vertex data
	freeVBOData(d);
}

void bindVBOData(VBOData *d)
{
	glBindBuffer(GL_ARRAY_BUFFER, d->vertexVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d->indexVboId);
}

void destroyVBOData(VBOData *d)
{
	freeVBOData(d);
	glDeleteBuffers(1, &d->vertexVboId);
	glDeleteBuffers(1, &d->indexVboId);
}

void freeVBOData(VBOData *d)
{
	if (d->dataAvailable == true) {
		free(d->v);
		free(d->n);
		free(d->i);
		d->dataAvailable = false;
	}
}

void doubleVBODataSize(VBOData *d)
{
	d->size *= 2;
	d->v = realloc(d->v, d->size * glfSize * 3);
	d->n = realloc(d->n, d->size * glfSize * 3);
	assert(d->n != NULL);
	assert(d->v != NULL);
}

void doubleVBODataIndexSize(VBOData *d)
{
	d->indexSize *= 2;
	d->i = realloc(d->i, d->indexSize * sizeof(GLuint));
	assert(d->i != NULL);
}

void insertVertexVBOData(VBOData *d, GLfloat *vertex, GLfloat *normal)
{
	if (d->front*3 >= d->size-3)
		doubleVBODataSize(d);
	memcpy(&d->v[d->front*3], vertex, 3*glfSize);
	memcpy(&d->n[d->front*3], normal, 3*glfSize);
	d->front++;
}

void insertIndexVBOData(VBOData *d, GLuint index)
{
	if (d->indexFront == d->indexSize)
		doubleVBODataIndexSize(d);
	d->i[d->indexFront++] = index;
}

void insertUniqueVBOData(VBOData *d, GLfloat *vertex, GLfloat *normal)
{
	d->dataAvailable = true;
	if (remove_duplicates) {
		GLuint index = vertexIsInVBOData(d, vertex, normal);
		if (index == -1) {
			insertIndexVBOData(d, d->front);
			insertVertexVBOData(d, vertex, normal);
		} else {
			insertIndexVBOData(d, index);
		}
	} else {
		insertIndexVBOData(d, d->front);
		insertVertexVBOData(d, vertex, normal);
	}
}

GLint vertexIsInVBOData(VBOData *d, GLfloat *vertex, GLfloat *normal)
{
	int i;
	for (i=0; i<d->front; ++i) {
		if (d->v[i*3] == vertex[0] && d->v[i*3+1] == vertex[1] && d->v[i*3+2] == vertex[2] &&
			d->n[i*3] == normal[0] && d->n[i*3+2] == normal[1] && d->n[i*3+2] == normal[2]) {
			return i;
		}
	}
	return -1;
}
