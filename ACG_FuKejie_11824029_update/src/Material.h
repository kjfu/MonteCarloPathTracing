#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>


class Material
{
public:
    Material(){}
    QVector3D Ka;// The Ka statement specifies the ambient reflectivity
    QVector3D Kd;// The Kd statement specifies the diffuse reflectivitys
    QVector3D Ks;// The Ks statement specifies the specular reflectivity
    QVector3D Tf;// The Tf statement specifies the transmission filter
    float Ni;// The Ni specifies the optical density for the surface.
    float Ns;// The Ns specifies the specular exponent for the current material.

    QVector3D Ke;
};

#endif // MATERIAL_H
