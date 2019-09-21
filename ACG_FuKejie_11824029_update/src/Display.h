#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include "Scene.h"
#include <QColor>
#include <string>

#include "PathTracer.h"

using namespace std;
class Display : public QWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = nullptr);

    void initialize();
    void paintEvent(QPaintEvent *event);
    void loadImage(QColor **&img);
    void save();
    void setSize(int width, int height);
    void setImageSavePath(string imgPath);

    QColor **image = nullptr;
    string imageSavePath;

    PathTracer pathTracer;

    bool pathTracerHaveSet = false;
    bool cameraHaveSet = false;
    bool objectsHaveLoad = false;
    bool canSave = false;
private:
    int width = 512;
    int height = 512;
};

#endif // DISPLAY_H
