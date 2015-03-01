#include "camera.h"
#include "image.h"

Image Camera::getImage()
{
	return Image(width, height);
}
