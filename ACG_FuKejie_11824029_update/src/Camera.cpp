#include <cmath>

#include "Camera.h"

#include "Sampler.h"

#include <QDebug>

Camera::Camera()
{

}

/*
 * Initialize the camera by inputing the position , the lookat, the up direction of the camera
*/
void Camera::initialize(QVector3D myPosition, QVector3D myLookAt, QVector3D myUp)
{
    this->position = myPosition;
    this->lookAt = myLookAt;
    this->up = myUp;
    this->right = QVector3D::crossProduct(this->lookAt - this->position, this->up).normalized();
    this->up = QVector3D::crossProduct(this->right, this->lookAt - this->position).normalized();
}

/*
 * Set the view port, the screen center is at the lookat position
*/
void Camera::setViewPort(float myFovY, float myWidth, float myHeight)
{
    float PI = 3.14159265358979f;
    this->width = myWidth;
    this->height = myHeight;
    this->fovy = myFovY;
    this->halfClipPortHeight = tan(this->fovy * PI / 360.0f) * (this->lookAt - this->position).length();
    this->halfClipPortWidth = this->halfClipPortHeight / this->height * this->width;
}


/*
 *  Generate a simple ray at the pixel(x,y) at the screen space
 *  x is at [0, width), y is at [0, height), both x and y are integer
 *  The pixel space is [0, width-1] x [0, height-1]
 *
*/

Ray Camera::generateSampleRay(int x, int y)
{

    float screenCenterX = this->width / 2.0f;
    float screenCenterY = this->height / 2.0f;

    QVector3D samplePoint = this->lookAt+
                         + 2.0f * (float(x) + Sampler::rand01f() - screenCenterX) / width * halfClipPortWidth * this->right
                         + 2.0f * (float(y) + Sampler::rand01f() - screenCenterY) / height * halfClipPortHeight * this->up;


    QVector3D rayDirection(samplePoint - this->position);
    Ray sampleRay(this->position, rayDirection.normalized());
    return  sampleRay;
}
