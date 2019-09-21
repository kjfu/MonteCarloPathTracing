#ifndef AABBOX_H
#define AABBOX_H

// From QT
#include <QVector3D>

#include "Ray.h"

class   AABBox // Axis-Aligned Bounding Box
{
public:
    AABBox();
    AABBox(QVector3D min, QVector3D max): bottom_left(min), top_right(max){}

    /*
     *  Initialize this AABBox
    */
    void initialize( const QVector3D min, const QVector3D max);


    /*
     * Expand to fit a vertex
     */
    void expand(const QVector3D &vertex);

    /*
     * Expand to fit another AABBox
    */
    void expand(const AABBox &aabbox);

    /*
     * Estimate whether the ray hit the AABBox
    */
    bool isHit(const Ray &ray);

    QVector3D bottom_left;//The vertex at the bottom-left of the AABBox (min)
    QVector3D top_right;//The vertex at the bottom-left of the AABBox (max)
};

#endif // AABBOX_H
