#include "AABBox.h"

#include <cmath>


using namespace std;

AABBox::AABBox()
{

}

/*
 *  Initialize this AABBox
*/
void AABBox::initialize(const QVector3D min, const QVector3D max)
{
    this->bottom_left = min;
    this->top_right = max;
}

/*
 * Expand to fit a vertex
 */
void AABBox::expand(const QVector3D &vertex)
{
    for (int i=0; i<3; i++)
    {
        if (this->bottom_left[i] > vertex[i])
        {
            this->bottom_left[i] = vertex[i];
        }
        else if (this->top_right[i] < vertex[i])
        {
            this->top_right[i] = vertex[i];
        }
    }
}

/*
 * Expand to fit another AABBox
*/
void AABBox::expand(const AABBox &aabbox)
{
    this->expand(aabbox.bottom_left);
    this->expand(aabbox.top_right);
}

/*
 * Estimate whether the ray hit the AABBox
*/
bool AABBox::isHit(const Ray &ray)
{
    float t1, t2;// The parameter of the equation: Ray = origin + t * direction
    float t_far, t_near;// The closest and farhest intersections

    if (abs(ray.direction[0]) < 1e-20f
            && (ray.origin[0] < this->bottom_left[0] || ray.origin[0] > this->top_right[0])) //Ray is parallel
    {
        return false;
    }

    t1 = (this->bottom_left[0] - ray.origin[0]) * ray.inverseDirection[0];
    t2 = (this->top_right[0] - ray.origin[0]) * ray.inverseDirection[0];
    t_far = max(t1, t2);
    t_near = min(t1, t2);


    for (int i=1; i<3; i++)
    {
        if (abs(ray.direction[i]) < 1e-20f
                && (ray.origin[i] < this->bottom_left[i] || ray.origin[i] > this->top_right[i]))// Ray is parallel
        {
            return false;
        }

        float t1 = (this->bottom_left[i] - ray.origin[i]) * ray.inverseDirection[i];
        float t2 = (this->top_right[i] - ray.origin[i]) * ray.inverseDirection[i];

        t_near = max(min(t1, t2), t_near);
        t_far = min(max(t1, t2), t_far);

        if  (t_far < t_near || t_far < ray.t_min)
        {
            return  false;
        }
    }

    return true;

}


