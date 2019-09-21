#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

#include <Ray.h>
class Camera
{
public:
    Camera();


    /*
     * Initialize the camera by inputing the position , the lookat, the up direction of the camera
    */
    void initialize(QVector3D myPosition, QVector3D myLookAt, QVector3D myUp);

    /*
     * Set view port
    */
    void setViewPort(float myFovY, float myWidth, float myHeight);

    /*
     *  Generate a simple ray at the (x,y) at the screen coordinate system
    */
    Ray generateSampleRay(int x, int y);

    float width;// The width of the view port
    float height;// The height of the view port

    QVector3D position;// The camera position
    QVector3D lookAt;// The position the camera look at
    QVector3D up;// The up direction of the camera
    QVector3D right;// The right direction of the camera


    float fovy;// Specifies the field of view angle, in degrees, in the y direction

    float halfClipPortWidth;//The half width of the clip port
    float halfClipPortHeight;// The half height of the clip port
};

#endif // CAMERA_H
