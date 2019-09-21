#ifndef KDTREE_H
#define KDTREE_H

#include <QVector>

#include "Triangle.h"
#include "AABBox.h"
#include "Ray.h"

class treeNode
{
public:
    treeNode():isLeaf(false), left(nullptr), right(nullptr){}

    /*
     * Construct the treeNode
    */
    void construct(QVector<Triangle*> &tris, AABBox &oldBox, int depth);

    /*
     *  Check whether or not the ray intersects the triangles, furthermore find nearest intersection if intersects
    */
    bool intersect(const Ray &r, Intersection &intersection);

    /*
     * Check whether or not the point of the object can see the light source
     * (That means between the point and the light source there are no objects)
    */
    bool seeLightSource(const Ray &r, const float length);

    bool isLeaf;// Whether or not the treeNode is a leaf
    int splitAxis;// The split axis: 0 for x, 1 for y, 2 for z
    int nTrianglesIn;// The number of triangles in this node

    treeNode *left;
    treeNode *right;
    QVector<Triangle *> triangles;
    AABBox aabbox;

};

#endif // KDTREE_H
