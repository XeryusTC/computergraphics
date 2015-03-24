#if defined(__APPLE__)&& defined(__MACH__)
#include <GLUT/glut.h>
#elif defined(_WIN32)
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include "lodepng.h"
#include "texture.h"

GLuint initTexture(char *filename) {
    unsigned char *buffer, *image;
    size_t buffersize, imagesize;
    GLuint texName;
    LodePNG_Decoder decoder;

    LodePNG_loadFile(&buffer, &buffersize, filename);
    LodePNG_Decoder_init(&decoder);
    decoder.infoRaw.color.colorType = 6;
    LodePNG_decode(&decoder, &image, &imagesize, buffer, buffersize);
    if (decoder.error) {
        printf("Error reading in png image: %d\n", decoder.error);
        exit(1);
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &texName);
        glBindTexture(GL_TEXTURE_2D, texName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, decoder.infoPng.width,
                decoder.infoPng.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    return texName;
}
