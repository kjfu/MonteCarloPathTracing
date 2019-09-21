#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QVector3D>

#include "Material.h"

class Intersection
{
public:
    Material *material;//The material of the intersect point
    QVector3D normal;//The normal of the point on the surface
    float t;// The distance of the ray path
};
#endif // INTERSECTION_H
