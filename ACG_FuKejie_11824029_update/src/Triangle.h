#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector3D>

#include "AABBox.h"
#include "Ray.h"
#include "Material.h"
#include "Intersection.h"

class Triangle
{
public:
    Triangle();
    ~Triangle() {}

    Triangle(QVector3D *vertPos, QVector3D *vertNorm, Material *mtl);
    void initialize(QVector3D *vertPos, QVector3D *vertNorm, Material *mtl);


    /*
     * Update the AABBox of this triangle
    */
    void updateAABBox();

    /*
     *  Check whether or not the ray intersects the triangle
    */
    bool intersect(const Ray &r, Intersection &intersection);



    QVector3D vertexPosition[3];//The positions of the three vertices
    QVector3D vertexNormal[3];// The normal vectors of three vertices
    QVector3D origin;
    QVector3D e1;
    QVector3D e2;
    AABBox aabbox;// The AABBox of this triangle
    Material *material;

};

#endif // TRIANGLE_H
