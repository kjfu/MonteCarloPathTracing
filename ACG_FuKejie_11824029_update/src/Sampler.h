#ifndef SAMPLER_H
#define SAMPLER_H

#include <QVector3D>
#include <QVector2D>

#include "Ray.h"

namespace Sampler
{

float rand01f();

/*
 * Uniformly sample a point on a unit disk
*/
QVector2D unitDiskSampler();

/*
 * Uniformly sample a point on hemisphre
*/
QVector3D uniformHemisphreSampler();

/*
 * Cosine weighted simple a point on hemisphere, for diffuse ray's direction sampling
*/
QVector3D cosineWeighedHemisphereSampler(float &pdf);

/*
 * Phone Lobe sample a point on hemisphere,for specular ray's direction sampling
*/
QVector3D phongLobeHemisphereSampler(float n, float &pdf);

/*
 * Sample a diffuse path
*/
Ray diffuseImportantceSampler(const QVector3D &postion, const QVector3D &normal, float &pdf);

/*
 * Sample a specular path
*/
Ray specularImportanceSampler(const QVector3D &postion, const QVector3D &normal, const QVector3D &ray2point, float ns, float &pdf);

/*
 * Uniform Sampler
*/
Ray uniformSampler(const QVector3D &postion, const QVector3D &normal);

}

#endif // SAMPLER_H
