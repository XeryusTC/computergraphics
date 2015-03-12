#include <string>
#include "object.h"

Color Object::surfaceColor(const Point hit)
{
    if (material->texture)
        return material->texture->colorAt(0, 0);
    return material->color;
}
