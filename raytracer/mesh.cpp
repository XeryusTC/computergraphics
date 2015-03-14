#include <string>
#include <vector>

#include "glm.h"
#include "mesh.h"
#include "triangle.h"
#include "triple.h"

Mesh::Mesh(std::string filename, float scale, Point pos)
{
    model = glmReadOBJ(const_cast<char*>(filename.c_str()));
	glmReverseWinding(model);
	glmFacetNormals(model);
	glmVertexNormals(model, 90);
    glmUnitize(model);
    glmScale(model, scale);

    SmoothTriangle *t;
    Point a, b, c;
	Vector d, e, f;
    for (unsigned int i=0; i<model->numtriangles; ++i) {
		a = modelDataToPoint(model->vertices, model->triangles[i].vindices[0], pos);
		b = modelDataToPoint(model->vertices, model->triangles[i].vindices[1], pos);
		c = modelDataToPoint(model->vertices, model->triangles[i].vindices[2], pos);
		d = modelDataToPoint(model->normals,  model->triangles[i].nindices[0]);
		e = modelDataToPoint(model->normals,  model->triangles[i].nindices[1]);
		f = modelDataToPoint(model->normals,  model->triangles[i].nindices[2]);
        t = new SmoothTriangle(a, b, c, d, e, f);
        triangles.push_back(t);
    }
    cout << "Model from " << filename << " with " << triangles.size() << " triangles loaded." << endl;
}

Mesh::~Mesh()
{
    glmDelete(model);
}

Hit Mesh::intersect(const Ray &ray)
{
    Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
    bool has_hit = false;
    for (unsigned int i=0; i<triangles.size(); ++i) {
        Hit hit(triangles[i]->intersect(ray));
        if (hit.t < min_hit.t) {
            min_hit = hit;
            has_hit = true;
        }
    }
    if (has_hit)
        return min_hit;
    return Hit::NO_HIT();
}

Point Mesh::modelDataToPoint(float *array, unsigned int idx, Point offset)
{
	return Vector(array[idx*3] + offset.x,
			array[idx*3+1] + offset.y,
			array[idx*3+2] + offset.z);
}
