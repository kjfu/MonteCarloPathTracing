#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QVector3D>
#include <QVector2D>

#include "Ray.h"
#include "Intersection.h"
#include "Triangle.h"

class LightSource
{
public:
    LightSource() {}
    virtual ~LightSource() {}

    /*
     *  virtual function for generating a sample light ray from the point to the light source
    */
    virtual Ray generateSampleLightRay(QVector3D &point,float &length,float &cosTheta_O, float &area) = 0;

    QVector3D center;
    QVector3D emission;

};

class SphereLight: public LightSource
{
public:
    /*
     * Set the light source by inputing the center, the light emssion and the radius of the sphere light source
    */
    void setLight(QVector3D i_center, QVector3D i_emssion, float i_radius);

    /*
     *  Generate a sample light ray from the point to the light source
    */
    virtual Ray generateSampleLightRay(QVector3D &point, float &length, float &cosTheta_O, float &area);

    float radius;// The radius of the sphere light source
};

class  QuadLight: public LightSource
{
public:
    /*
     * Set the light source by inputing the center, the size, the normal, the u direction and the light emssion of the quad light source
    */
    void setLight(QVector3D i_center, QVector2D i_size, QVector3D i_normal,QVector3D i_u, QVector3D i_emssion);

    /*
     *  Generate a sample light ray from the point to the light source
    */
    virtual Ray generateSampleLightRay(QVector3D &point, float &length, float &cosTheta_O,float &area);

    QVector3D normal;// The normal of the quad light source
    QVector3D du;//The uint u direction of the quad light source
    QVector3D dv;//The uint v direction of the quad light source
    QVector2D size;//The size of the quad light by use a 2d vector [a, b]
    float lightArea;
};

#endif // LIGHTSOURCE_H
