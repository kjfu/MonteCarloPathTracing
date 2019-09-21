#include "LightSource.h"
#include "Sampler.h"

#include <cmath>
#include <QDebug>
using namespace std;



void SphereLight::setLight(QVector3D i_center, QVector3D i_emssion, float i_radius)
{
    float PI = 3.1415926535f;
    this->center = i_center;
    this->emission = i_emssion;
    this->radius = i_radius;
}

Ray SphereLight::generateSampleLightRay(QVector3D &point, float &length, float &cosTheta_O, float &area)
{
    float PI = 3.1415926535f;
//    QVector3D uvw = Sampler::uniformHemisphreSampler();

    QVector3D dw = (point - this->center).normalized();
    QVector3D du = QVector3D::crossProduct(dw, QVector3D(1, 0, 0));
    if (du.length() < 1e-10f)
    {
         du = QVector3D::crossProduct(dw, QVector3D(0, 1, 0)).normalized();
    }
    else
    {
        du = du.normalized();
    }
    QVector3D dv = QVector3D::crossProduct(du, dw).normalized();

    float distance = QVector3D(this->center - point).length();

    if (distance < this->radius)
    {
        area = 0;
        return  Ray();
    }
    float cosTheta = this->radius / distance;
    float h = this->radius * (1 -cosTheta);//Spherical cap
    area = 2 * PI * this->radius * h;
    float r1 = Sampler::rand01f();
    float r2 = Sampler::rand01f();

    float sampleTheta = r1 * acos(cosTheta);
    float sampleAlpha = 2 * PI * r2;
    QVector3D sampleDirection = sin(sampleTheta) * cos(sampleAlpha) * du + sin(sampleTheta) * sin(sampleAlpha) * dv + cos(sampleTheta) * dw;
    QVector3D samplePosition = this->center + sampleDirection * this->radius;
    QVector3D lightDirection = (samplePosition - point);
    length = lightDirection.length();
    lightDirection = lightDirection.normalized();
    cosTheta_O = max(0.0f, float(QVector3D::dotProduct(-lightDirection, sampleDirection)));//If cosTheta_0 < 0, set it 0
    Ray lightRay(point, lightDirection);
    lightRay.t_max = length * 0.999999f;
    lightRay.t_min = length * 0.000001f;
    return lightRay;
}


/*
 * Set the light source by inputing the center, the size, the normal, the u direction and the light emssion of the quad light source
*/
void QuadLight::setLight(QVector3D i_center, QVector2D i_size, QVector3D i_normal, QVector3D i_u, QVector3D i_emssion)
{
    this->center = i_center;
    this->emission = i_emssion;
    this->normal = i_normal.normalized();
    this->size = i_size;
    this->du = i_u.normalized();
    this->dv = QVector3D::crossProduct(this->normal, this->du).normalized();
    this->lightArea = this->size[0] * this->size[1];

}

/*
 *  Generate a sample light ray from the point to the light source
*/
Ray QuadLight::generateSampleLightRay(QVector3D &point, float &length, float &cosTheta_O, float &area)
{

    float u = Sampler::rand01f() - 0.5f;
    float v = Sampler::rand01f() - 0.5f;
    area = this->lightArea;
    QVector3D samplePosition = this->center + u * this->du * size[0] + v * this->dv * size[1];
    QVector3D direction = (samplePosition - point);
    length = direction.length();
    direction = direction.normalized();
    cosTheta_O = abs(QVector3D::dotProduct(direction, this->normal));
    Ray lightRay(point, direction);
    lightRay.t_max = length * 0.999999f;
    lightRay.t_min = length * 0.000001f;//To avoid the ray  intersect itself when checking intersecting
    return lightRay;
}


