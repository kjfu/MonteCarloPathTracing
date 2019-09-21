#include "PathTracer.h"
#include "Sampler.h"

#include <cmath>

#include <QDebug>



#include "Sampler.h"
using namespace std;



#include <QApplication>



void PathTracer::render()
{

    float screenGamma = 2.2f;
    image = new QColor*[int(this->scene->camera.width)];
    for (int i=0; i<this->scene->camera.width; i++)
    {
        image[i] =new QColor[int(this->scene->camera.height)];
    }

    for (int i=0; i<this->scene->camera.width; i++)
    {

        for (int j=0; j<this->scene->camera.height; j++)
        {


            QVector3D rgb(0,0,0);
            for (int k=0; k<numPixelSamples; k++)
            {

               Ray sampleRay = this->scene->camera.generateSampleRay(i,j);
               Intersection intersection;
               rgb += this->traceRay(sampleRay, 0) ;

               QApplication::processEvents();

            }
            rgb =rgb/float(numPixelSamples);


           for (int k=0; k<3; k++)
           {

              rgb[k] = min(pow(double(rgb[k]), 1.0/screenGamma), 0.99);
              if(rgb[k]<0)
              {
                  rgb[k]=0;
              }
           }

           image[i][j].setRgbF(rgb[0], rgb[1], rgb[2]);

        }

        if ((i+1)%8==0)
        {
            qDebug() << (i+1)/(scene->camera.width+1) * 100 <<"%";
        }
    }
}

void PathTracer::releaseImage()
{
    for (int i=0; i<this->scene->camera.width; i++)
    {
        delete []  image[i] ;
    }
    delete [] image;
}

void PathTracer::setNumSamplesPerPixel(int n)
{
    this->numPixelSamples = n;
}

void PathTracer::setNumSamplesPerLightSoure(int n)
{
     this->numDirectLightSamples = n;
}

void PathTracer::setAmbienLight(QVector3D l)
{
    this->ambientLight = l;
}

void PathTracer::setTraceDepeth(int n)
{
    this->maxDepth = n;
}

QVector3D PathTracer::traceRay(Ray &ray, int depth)
{

    if (depth > maxDepth)
    {
           return QVector3D(0, 0, 0);
    }

    Intersection intersection;

    if (this->scene->intersect(ray, intersection))
    {
        //If the rgb with the ambient light is large than [1,1,1],return it directly;
        if ( (this->ambientLight * intersection.material->Ka).length() > sqrt(3.0f))
        {
            return  ( this->ambientLight * intersection.material->Ka);
        }

        QVector3D indirectLight(0, 0, 0);
        QVector3D directLight = this->directIllumination(ray, intersection);
        QVector3D position = ray.origin + ray.direction * intersection.t * 0.99999f;


        if (intersection.material->Ni> 1.4f) // Refaraction
        {

            QVector3D normal =intersection.normal;
            float ni;
            float nt;
            float cosTheta_i = QVector3D::dotProduct(ray.direction, intersection.normal);
            if (cosTheta_i<0)//The ray is going into the material
            {
                ni = 1.0f;
                nt = intersection.material->Ni;

            }
            else
            {
                ni = intersection.material->Ni;// The ray is leaving th material
                nt = 1.0f;
                normal = -intersection.normal;

            }
            cosTheta_i = abs(cosTheta_i);
            float squreCosTheta_t = 1 - (ni/nt) * (ni/nt) * (1.0f - cosTheta_i*cosTheta_i);
            if (squreCosTheta_t >= 0)
            {
                float cosTheta_t =sqrt(squreCosTheta_t);
                QVector3D direction = (ni/nt) * ray.direction - (cosTheta_t - ni/nt * cosTheta_i) * normal;
                Ray refractionRay(position,direction.normalized());
                indirectLight += this->traceRay(refractionRay, depth+1);//Don't add depth
            }
        }
        else// Reflection
        {
            if (QVector3D::dotProduct( intersection.normal, ray.direction)>0 )
            {
                return  QVector3D(0,0,0);
            }

            float p1 = intersection.material->Kd[0] + intersection.material->Kd[1] +intersection.material->Kd[2];
            float p2 = intersection.material->Ks[0] + intersection.material->Ks[1] +intersection.material->Ks[2];
            QVector3D li;
            Ray sampleRay;


            if (Sampler::rand01f() < (p1/(p1 + p2)))
            {

                float pdf;
                sampleRay = Sampler::diffuseImportantceSampler(position, intersection.normal,pdf);
                li = this->traceRay(sampleRay, depth+1);
                indirectLight += intersection.material->Kd * li;
            }
            else
            {
                 float PI = 3.14159265354f;
                 float pdf;
                 float ns = intersection.material->Ns;
                 sampleRay  = Sampler::specularImportanceSampler(position, intersection.normal, ray.direction, intersection.material->Ns, pdf);
                 li = this->traceRay(sampleRay, depth+1);
                 QVector3D H = (-ray.direction + sampleRay.direction).normalized();
                 float brdf = pow(QVector3D::dotProduct(H, intersection.normal), ns) * (intersection.material->Ns + 8) / (8*PI);
                 indirectLight +=  li * intersection.material->Ks / pdf * brdf * QVector3D::dotProduct(intersection.normal, sampleRay.direction);
            }

       }

        return  (directLight + indirectLight + this->ambientLight * intersection.material->Ka);

    }
    else
    {
        return QVector3D(0,0,0);
    }
}

QVector3D PathTracer::directIllumination(Ray &ray, Intersection &intersection)
{

    QVector3D ld(0,0,0);
    float PI = 3.14159265354f;
    for (int i=0; i<this->scene->lights.size(); i++)
    {
        QVector3D ld_i(0,0,0);
        for (int j=0; j<this->numDirectLightSamples; j++)
        {

            Material &currentMaterial = *intersection.material;
            LightSource &currentLight  = *this->scene->lights[i];
            float length;
            float cosTheta_I;
            QVector3D position = ray.origin + ray.direction * intersection.t;
            float lightArea;
            Ray sampleRay = this->scene->lights[i]->generateSampleLightRay(position, length, cosTheta_I, lightArea);

            if (lightArea < 1e-15f)
            {
                continue;
            }

            float cosTheta_O = QVector3D::dotProduct(sampleRay.direction, intersection.normal);

            if (cosTheta_O < 0)
            {
                continue;
            }
            Intersection tmp_intersection;
            if ( this->scene->seeLightSource(sampleRay, length) /*!this->scene->intersect(sampleRay, tmp_intersection)*/)
            {

                if (currentMaterial.Kd.length() > 0)
                {
                    QVector3D brdf = currentMaterial.Kd / PI;
                    ld_i +=  currentLight.emission * lightArea *
                            (cosTheta_I * cosTheta_O) / (length * length) * brdf;
                }

                if (currentMaterial.Ks.length() > 0)
                {
                    float ns = currentMaterial.Ns;
                    QVector3D H = (sampleRay.direction - ray.direction).normalized();
                    QVector3D brdf = pow(QVector3D::dotProduct(H, intersection.normal), ns) * currentMaterial.Ks * (intersection.material->Ns + 8)/(8*PI) ;

                    ld_i +=  currentLight.emission * lightArea * brdf * (cosTheta_I * cosTheta_O) / (length * length) ;
                }
            }
        }

        if (numDirectLightSamples > 0)
        {
            ld += ld_i/float(numDirectLightSamples);
        }
    }


    return ld;
}


