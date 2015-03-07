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

extern ScreenInfo screen;
extern Camera cam;
extern CONTROL_MODE rotate_mode;
extern float rotx, roty;

GLMmodel *obj;
GLfloat model_light_pos[4] = {100.0, 600.0, 200.0, 0.0};
GLfloat model_light_color[3] = {1.0, 1.0, 1.0};

GLuint vboId, vboIndices;

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
    // Make sure that there is a model loaded
    if (!obj)
        loadModel(filename);

    // Create VBO for vertices and normals
    glGenBuffersARB(1, &vboId);
    glBindBufferARB(GL_ARRAY_BUFFER, vboId);
    // Load vertex data
    glBufferDataARB(GL_ARRAY_BUFFER, (obj->numvertices+1)*3*sizeof(GLfloat),
            obj->vertices, GL_STATIC_DRAW);

    // Load normal data

    // Load index data
    glGenBuffers(1, &vboIndices);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * obj->numtriangles,
            NULL, GL_STATIC_DRAW);
    for (i=0; i<obj->numtriangles; ++i) {
        glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER, i*3*sizeof(GLuint),
                3*sizeof(GLuint), obj->triangles[i].vindices);
    }
}

void glmDrawVBO(void)
{
    glBindBufferARB(GL_ARRAY_BUFFER, vboId);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, vboIndices);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glDrawElements(GL_TRIANGLES, obj->numtriangles*3, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER, 0);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void glmDestroyVBO(void)
{
    glDeleteBuffersARB(1, &vboId);
    glDeleteBuffersARB(1, &vboIndices);
}
