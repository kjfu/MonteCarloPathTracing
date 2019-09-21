#include "Ray.h"

Ray::Ray()
{

}

void Ray::initialize(const QVector3D &i_origin, const QVector3D &i_direction)
{
    this->origin = i_origin;
    this->direction = i_direction;
    for (int i=0; i<3; i++)
    {
        this->inverseDirection[i] = 1.0f / this->direction[i];
    }
}


