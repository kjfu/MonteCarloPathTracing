#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <QVector3D>
#include <QColor>

#include "Scene.h"
#include "Ray.h"
class PathTracer
{
public:
    PathTracer():scene(nullptr), numPixelSamples(1), numDirectLightSamples(1), ambientLight(1,1,1), terminateProbility(0.9f), maxDepth(5)
    {
    }

    Scene *scene;
    int numPixelSamples;// The number of simples at a pixel
    int numDirectLightSamples;// The numberof simples for Monte Carlo  Direct Illumination
    QVector3D  ambientLight; //The ambient light
    float terminateProbility;//The probility to terminate when current >maxDepth;
    int maxDepth = 5;//The max depeth when it needs to check whether to terminate or not
    QColor **image;

    /*
     * Render the scene, and save the color at the **image
    */
    void render();

    /*
     * Release the data stored at the **image
    */
    void releaseImage();

    /*
     *  Set the number of the sample rays per pixel
    */
    void setNumSamplesPerPixel(int n);

    /*
     * Set the number of sample rays per light source
   */
    void setNumSamplesPerLightSoure(int n);

    /*
     * Set the ambient light
    */
    void setAmbienLight(QVector3D l);

    /*
     * Set max trace depth
    */
    void setTraceDepeth(int n);

    /*
     * Key function:
     * Cenerate a samole ray from the current intersection,and trace the ray, calculate the illumintion!
     * Return the imllumination for the former path
    */
    QVector3D traceRay(Ray &ray, int depth);

    /*
     * Key function:
     * Calculate direct illumination from the light source
     *
    */
    QVector3D directIllumination(Ray &ray, Intersection &intersection);

};

#endif // PATHTRACER_H
