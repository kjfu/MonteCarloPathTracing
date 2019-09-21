#include "Display.h"
#include "Camera.h"

#include <QPainter>
#include <QDebug>
#include <QPixmap>

Display::Display(QWidget *parent) : QWidget(parent)
{
//     this->pathTracer.scene = new Scene;
    //    resize(width, height);
}

void Display::initialize()
{

    if (this->pathTracer.scene == nullptr)
    {
        this->pathTracer.scene = new Scene;
    }
    else
    {
        delete this->pathTracer.scene;
        this->pathTracer.scene = new Scene;
    }

    if (this->image != nullptr)
    {
        this->image = nullptr;
        this->pathTracer.releaseImage();
    }

    this->pathTracerHaveSet = false;
    this->cameraHaveSet = false;
    this->objectsHaveLoad = false;
    this->canSave = false;
}

void Display::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setWindow(width, height, -width, -height);

    //painter.setViewport(width/2, height/2, width, -height);

    if (this->image !=nullptr)
    {
        for (int i=0; i<this->width; i++)
        {
            for(int j=0; j<this->height; j++)
            {
                painter.setPen(image[i][j]);
                painter.drawPoint(i,j);
            }
        }
      //  save();
    }

    painter.end();
}

void Display::loadImage(QColor **&img)
{
    this->image = img;
}

void Display::save()
{
    QPixmap outImage(width, height);

    QPainter painter(&outImage);
    painter.setWindow(width, height, -width, -height);
    if (this->image !=nullptr)
    {

        for (int i=0; i<this->width; i++)
        {
            for(int j=0; j<this->height; j++)
            {
                painter.setPen(image[i][j]);
                painter.drawPoint(i,j);
            }
        }

        painter.end();

        outImage.save(this->imageSavePath.data(),"PNG");
    }
}

void Display::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
    resize(width, height);
}

void Display::setImageSavePath(string imagePath)
{
    this->imageSavePath = imagePath;
}
