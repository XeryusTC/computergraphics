#include <string>
#include <vector>

#include "glm.h"
#include "mesh.h"
#include "triangle.h"
#include "triple.h"

Mesh::Mesh(std::string filename, float scale, Point pos)
{
    model = glmReadOBJ(const_cast<char*>(filename.c_str()));
	glmFacetNormals(model);
	glmVertexNormals(model, 90);
    glmUnitize(model);
    glmScale(model, scale);

    SmoothTriangle *t;
    Point a, b, c;
	Vector d, e, f;
    Material *m;
    GLMgroup *group = model->groups;
    while (group) {
        m = modelDataToMaterial(group->material);
        for (unsigned int i=0; i<group->numtriangles; ++i) {
            a = modelDataToPoint(model->vertices,
                    model->triangles[group->triangles[i]].vindices[0], pos);
            b = modelDataToPoint(model->vertices,
                    model->triangles[group->triangles[i]].vindices[1], pos);
            c = modelDataToPoint(model->vertices,
                    model->triangles[group->triangles[i]].vindices[2], pos);
            d = modelDataToPoint(model->normals,
                    model->triangles[group->triangles[i]].nindices[0]).normalized();
            e = modelDataToPoint(model->normals,
                    model->triangles[group->triangles[i]].nindices[1]).normalized();
            f = modelDataToPoint(model->normals,
                    model->triangles[group->triangles[i]].nindices[2]).normalized();
            t = new SmoothTriangle(a, b, c, d, e, f);
            t->material = m;
            triangles.push_back(t);
        }
        group = group->next;
    }
    cout << "Model from " << filename << " with " << triangles.size() << " triangles loaded." << endl;
}

Mesh::~Mesh()
{
    glmDelete(model);
}

Hit Mesh::intersect(const Ray &ray)
{
    Hit min_hit(std::numeric_limits<double>::infinity(), Vector(), material);
    bool has_hit = false;
    for (unsigned int i=0; i<triangles.size(); ++i) {
        Hit hit(triangles[i]->intersect(ray));
        if (hit.t < min_hit.t) {
            min_hit = hit;
            has_hit = true;
        }
    }
    if (has_hit) {
        if (min_hit.m == NULL)
            return Hit(min_hit.t, min_hit.N, material);
        return min_hit;
    }
    return Hit::NO_HIT();
}

Point Mesh::modelDataToPoint(float *array, unsigned int idx, Point offset)
{
	return Vector(array[idx*3] + offset.x,
			array[idx*3+1] + offset.y,
			array[idx*3+2] + offset.z);
}

Material* Mesh::modelDataToMaterial(unsigned int idx)
{
    if (idx == 0)
        return NULL;

    cout << "Loaded material\n";
    Material *m = new Material();
    m->color = Color(model->materials[idx].diffuse[3], model->materials[idx].diffuse[1],
              model->materials[idx].diffuse[2]);
    m->kd = (m->color.r + m->color.g + m->color.b) / 3.0;
    m->ka = model->materials[idx].ambient[1];
    m->ks = model->materials[idx].specular[1];
    m->n  = model->materials[idx].shininess;

    return m;
}
