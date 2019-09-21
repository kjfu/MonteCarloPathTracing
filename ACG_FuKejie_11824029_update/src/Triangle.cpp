#include "Triangle.h"

#include <cmath>


Triangle::Triangle()
{

}



Triangle::Triangle(QVector3D *vertPos, QVector3D *vertNorm, Material *mtl)
{
    initialize(vertPos, vertNorm, mtl);
}

void Triangle::initialize(QVector3D *vertPos, QVector3D *vertNorm, Material *mtl)
{
    this->vertexPosition[0] = vertPos[0];
    this->vertexPosition[1] = vertPos[1];
    this->vertexPosition[2] = vertPos[2];
    this->vertexNormal[0] = vertNorm[0];
    this->vertexNormal[1] = vertNorm[1];
    this->vertexNormal[2] = vertNorm[2];
    this->material = mtl;
    this->origin = vertexPosition[0];
    this->e1 = vertexPosition[1] - vertexPosition[0];
    this->e2 = vertexPosition[2] - vertexPosition[0];
}


/*
 * Update the AABBox of the triangle
*/
void Triangle::updateAABBox()
{
    this->aabbox.initialize(this->vertexPosition[0], this->vertexPosition[0]);// Set the top-right vertex and the bottom the vertex both the first vertex of the triangle

    for (int i=1; i<3; i++)
    {
        this->aabbox.expand(vertexPosition[i]);
    }
}


/*
 * Check whether or not the ray intersects the triangle
 * return with the t (distance of the ray) and normal(the normal at the  intersect point)
*/
bool Triangle::intersect(const Ray &r, Intersection &intersection)
{
    float u, v;
    float t_temp = 0;
    QVector3D pVector= QVector3D::crossProduct(r.direction, this->e2);
    float det = QVector3D::dotProduct(this->e1, pVector);
    if (abs(det) <1e-8f)
    {
        return  false;
    }
    float inverseDet = 1.0f / det;
    QVector3D tvec = r.origin - this->vertexPosition[0];
    u = QVector3D::dotProduct(tvec,pVector) * inverseDet;
    if (u < 0 || u > 1)
    {
        return  false;
    }
    QVector3D qVector = QVector3D::crossProduct(tvec, this->e1);
    v = QVector3D::dotProduct(r.direction, qVector) * inverseDet;
    if (v<0||u+v>1)
    {
        return  false;
    }
    t_temp = QVector3D::dotProduct(this->e2, qVector) * inverseDet;
    if (t_temp > r.t_min && t_temp < r.t_max)
    {
        intersection.t = t_temp;
        intersection.normal= (this->vertexNormal[0] + u * (this->vertexNormal[1] - this->vertexNormal[0])
                + v * (this->vertexNormal[2] -this->vertexNormal[0])).normalized();// Calculate the normal of the intersect point
        intersection.material = this->material;
        return  true;
    }
    return false;

}

