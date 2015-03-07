#if defined(__APPLE__)&& defined(__MACH__)
    #include <GLUT/glut.h>
#elif defined(_WIN32)
    #include <GL/freeglut.h>
#else
    #include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "glm.h"

const int glfSize = sizeof(GLfloat);

extern ScreenInfo screen;
extern Camera cam;
extern CONTROL_MODE rotate_mode;
extern float rotx, roty;

GLMmodel *obj;
GLfloat model_light_pos[4] = {100.0, 600.0, 200.0, 0.0};
GLfloat model_light_color[3] = {1.0, 1.0, 1.0};

GLuint vboId;

void displayMesh(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	cameraLookAt(cam);

	// Set rotation
	glMatrixMode(GL_MODELVIEW);
	glRotatef(rotx, 1.0, 0.0, 0.0);
	glRotatef(roty, 0.0, 1.0, 0.0);

	// Add light
	glLightfv(GL_LIGHT0, GL_POSITION, model_light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  model_light_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  model_light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, model_light_color);


    glmDrawVBO();

    if (rotate_mode == MODE_FPS)
		resetMousePosition();

    glutSwapBuffers();
    glutPostRedisplay();
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
}

void unloadModel(void)
{
    glmDelete(obj);
}

void glmInitVBO(char *filename)
{
    int i;
    int vsize, nsize; // Size of vertices and normals in VBO in bytes
    // Make sure that there is a model loaded
    if (!obj)
        loadModel(filename);

    // Create VBO for vertices and normals
    glGenBuffersARB(1, &vboId);
    glBindBufferARB(GL_ARRAY_BUFFER, vboId);
    vsize = obj->numtriangles * 9 * glfSize;
    nsize = obj->numtriangles * 9 * glfSize;
    glBufferDataARB(GL_ARRAY_BUFFER, vsize + nsize, NULL, GL_STATIC_DRAW);

    // Convert triangles to lists vertices & their normals
    GLfloat *v = malloc(vsize);
    GLfloat *n = malloc(nsize);
    for (i=0; i<obj->numtriangles; ++i) {
        memcpy(&v[i*9],   &obj->vertices[obj->triangles[i].vindices[0]*3], 3*glfSize);
        memcpy(&v[i*9+3], &obj->vertices[obj->triangles[i].vindices[1]*3], 3*glfSize);
        memcpy(&v[i*9+6], &obj->vertices[obj->triangles[i].vindices[2]*3], 3*glfSize);
        memcpy(&n[i*9],   &obj->normals[obj->triangles[i].nindices[0]*3], 3*glfSize);
        memcpy(&n[i*9+3], &obj->normals[obj->triangles[i].nindices[1]*3], 3*glfSize);
        memcpy(&n[i*9+6], &obj->normals[obj->triangles[i].nindices[2]*3], 3*glfSize);
    }
    // Store the lists of vertices and normals in the VBO
    glBufferSubDataARB(GL_ARRAY_BUFFER, 0, vsize, v);
    glBufferSubDataARB(GL_ARRAY_BUFFER, vsize, nsize, n);
    free(v);
    free(n);
}

void glmDrawVBO(void)
{
    glBindBufferARB(GL_ARRAY_BUFFER, vboId);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, obj->numtriangles * 9 * glfSize);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, obj->numtriangles*3);

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER, 0);
}

void glmDestroyVBO(void)
{
    glDeleteBuffersARB(1, &vboId);
}
