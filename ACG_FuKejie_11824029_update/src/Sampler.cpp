#include "Sampler.h"

#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

/*
 * Get a random number betwwen [0,1]
*/
float Sampler::rand01f()
{

    return  float(rand())/float(RAND_MAX);
}
/*
 * Uniformly sample a point on a unit disk
*/
QVector2D Sampler::unitDiskSampler()
{
    float PI = 3.1415926535f;

    // Generate 2 random float point number
    float rd1 = rand01f();
    float rd2 = rand01f();

    float r = sqrt(rd1);
    float theta = 2 * PI * rd2;
    float x = r * cos(theta);
    float y = r * sin(theta);
    return QVector2D(x, y);

}

/*
 * Uniformly sample a point on hemisphre
*/
QVector3D Sampler::uniformHemisphreSampler()
{
    float PI = 3.1415926535f;
    // Generate 2 random float point number
    float rd1 = rand01f();
    float rd2 = rand01f();

    float x = cos(2 * PI * rd2) * sqrt(1 - rd1 * rd1);
    float y = sin(2 * PI * rd2) * sqrt(1 - rd1 * rd1);
    float z = rd1;

    return QVector3D(x, y, z);

}

/*
 * Cosine weighted simple a point on  hemisphere
 * Malley’s method
*/
QVector3D Sampler::cosineWeighedHemisphereSampler(float &pdf)
{
    float PI = 3.1415926535f;
    QVector2D xy = unitDiskSampler();
    float z=sqrt(max(0.0f, 1.0f - QVector2D::dotProduct(xy,xy)));
    pdf = z / PI;
    return QVector3D(xy, z);
}




/*
 * Phone Lobe sample a point on hemisphere
*/
QVector3D Sampler::phongLobeHemisphereSampler(float n, float &pdf)
{
    float PI = 3.1415926535f;
    // Generate 2 random float point number
    float rd1 = rand01f();
    float rd2 = rand01f();
    float theta = acos(pow(rd1, 1.0f/float(n+1)));
    float x = cos(2 * PI * rd2) * sin(theta);
    float y = sin(2 * PI * rd2) * sin(theta);
    float z = cos(theta);
    pdf = (n + 1)/ (2 * PI) * pow(z, n);
    return QVector3D(x, y, z);
}

/*
 * Sample a diffuse path
*/
Ray Sampler::diffuseImportantceSampler(const QVector3D &postion, const QVector3D &normal, float &pdf)
{
    QVector3D up = normal;
    QVector3D front;
    if (abs(up[0]) < 1e-5f)//The up direction and direction (1,0,0) are orthogonal
    {
        front = QVector3D::crossProduct(QVector3D(1,0,0), up).normalized();
    }
    else
    {
        front = QVector3D::crossProduct(QVector3D(0,1,0), up).normalized();
    }
    QVector3D right = QVector3D::crossProduct(front, normal).normalized();

    QVector3D uvw = cosineWeighedHemisphereSampler(pdf);
    QVector3D direction(uvw[0] * right + uvw[1] * front + uvw[2]* up);
    return Ray(postion, direction.normalized());
}

/*
 * Sample a specular path
*/
Ray Sampler::specularImportanceSampler(const QVector3D &postion, const QVector3D &normal, const QVector3D &ray2point, float ns, float &pdf)
{
    QVector3D uvw = phongLobeHemisphereSampler(ns, pdf);
    QVector3D up  = (ray2point + 2 * abs(QVector3D::dotProduct(normal, ray2point)) * normal).normalized();
    QVector3D front;
    if (abs(up[0]) < 1e-5f)//The up direction and direction (1,0,0) are orthogonal
    {
        front = QVector3D::crossProduct(QVector3D(1,0,0), up).normalized();
    }
    else
    {
        front = QVector3D::crossProduct(QVector3D(0,1,0), up).normalized();
    }
    QVector3D right = QVector3D::crossProduct(front, normal).normalized();
    QVector3D direction( uvw[0] * right + uvw[1] * front + uvw[2]* up);
    if (QVector3D::dotProduct(direction, normal)<0)//The sample direction is reverse from the normal
    {

        direction = -uvw[0] * right - uvw[1] * front + uvw[2]* up;
    }
    direction.normalized();
    return Ray(postion, direction);
}


Ray Sampler::uniformSampler(const QVector3D &postion, const QVector3D &normal)
{
    QVector3D up = normal;
    QVector3D front;
    if (abs(up[0]) < 1e-5f)//The up direction and direction (1,0,0) are orthogonal
    {
        front = QVector3D::crossProduct(QVector3D(1,0,0), up).normalized();
    }
    else
    {
        front = QVector3D::crossProduct(QVector3D(0,1,0), up).normalized();
    }

    QVector3D right = QVector3D::crossProduct(front, normal).normalized();

    QVector3D uvw = uniformHemisphreSampler();
    QVector3D direction(uvw[0] * right + uvw[1] * front + uvw[2]* up);
    return Ray(postion, direction.normalized());
}

