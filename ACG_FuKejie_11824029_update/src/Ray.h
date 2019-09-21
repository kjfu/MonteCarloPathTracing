#ifndef RAY_H
#define RAY_H

#include <QVector3D>

class Ray
{
public:
    Ray();

    Ray(const QVector3D &i_origin, const QVector3D &i_direction)
    {
        initialize(i_origin, i_direction);
    }

    void initialize(const QVector3D &i_origin, const QVector3D &i_direction);

    //Ray = origin + t * direction
    QVector3D origin;// Origin
    QVector3D direction;// Direction
    float t_min = 0;//The intersect distance must larger than t_min
    float t_max = 3.4e38f;
    QVector3D inverseDirection;// inverseDirection = 1 / direction

};

#endif // RAY_H
