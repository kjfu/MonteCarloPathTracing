#include "mainwindow.h"
#include <QApplication>

#include <sstream>
#include <string>
#include <map>
#include <QDebug>


#include <QVector3D>
#include <QVector3D>
#include <QColor>


#include "Scene.h"
#include "Camera.h"
#include "PathTracer.h"
#include "LightSource.h"
using namespace  std;

void scene01_room(PathTracer &tracer, MainWindow &w, int ssp, int spl,int maxdepth, string openpath, string savepath);
void scene02_cup(PathTracer &tracer, MainWindow &w, int ssp, int spl, int maxdepth, string openpath, string savepath);
void scene03_VeachMIS(PathTracer &tracer, MainWindow &w, int ssp,int spl, int maxdepth,  string openpath, string savepath);
void myscene_IronMan(PathTracer &tracer, MainWindow &w, int ssp, int spl,int maxdepth, string openpath, string savepath);

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    PathTracer tracer;
    srand(time(nullptr));

/*------------------ Uncomment this, then you can render the scene without the gui-------------------------*/
//    string openpath="/Users/fukejie/Library/Mobile Documents/com~apple~CloudDocs/Computer Graphics/note&scenes/Scene02/room.obj";
//    string savepath="/Users/fukejie/Documents/ACG_FuKejie_11824029_update/scenes/Scene01/myRoom0413_test.png";
//    int ssp = 1;
//    int spl = 1;
//    int maxdepth =5;
//    scene01_room(tracer, w, ssp, spl, maxdepth, openpath, savepath);
//    scene02_cup(tracer, w, ssp, spl, maxdepth, openpath, savepath);
//    scene03_VeachMIS(tracer, w, ssp, spl,maxdepth, openpath, savepath);
//    myscene_IronMan(tracer, w, ssp, spl,maxdepth, openpath, savepath);
/*----------------- Uncomment this, then you can render the scene without the gui -------------------*/

    w.show();
    return a.exec();
}
void scene01_room(PathTracer &tracer, MainWindow &w, int ssp, int spl,int maxdepth, string openpath, string savepath)
{
        Scene scene;
        scene.loadObjects(openpath);
        scene.camera.initialize(QVector3D(0, 0, 4.0f), QVector3D(0, 0, 0), QVector3D(0,1,0));
        scene.camera.setViewPort(50,512,512);
        SphereLight light_scene2_01;
        light_scene2_01.setLight(QVector3D(0.0, 1.589, -1.274), QVector3D(50,50,40), 0.2f);
        scene.lights.push_back(&light_scene2_01);
        tracer.scene = &scene;
        w.setDisplaySize(512, 512);
        tracer.setAmbienLight(QVector3D(1,1,1));

        tracer.setNumSamplesPerPixel(ssp);
        tracer.setNumSamplesPerLightSoure(spl);
        tracer.setTraceDepeth(maxdepth);
        tracer.render();
        w.loadImage(tracer.image);
        w.saveImage(savepath);

}

void scene02_cup(PathTracer &tracer, MainWindow &w, int ssp, int spl, int maxdepth, string openpath, string savepath)
{
        tracer.scene = new Scene;
        tracer.scene->loadObjects(openpath);
        tracer.scene->camera.initialize(QVector3D(0.0, 0.64f, 0.52f), QVector3D(0, 0.4f, 0.3f), QVector3D(0, 1, 0));
        tracer.scene->camera.setViewPort(60, 512, 512);
        QuadLight light0;
        light0.setLight(QVector3D(-2.758771896f, 1.5246f, 0), QVector2D(1, 1), QVector3D(1,0,0), QVector3D(0, 1, 0), QVector3D(40.0f, 40.0f, 40.0f));
        tracer.scene->lights.push_back(&light0);
        tracer.setAmbienLight(QVector3D(0.0f,0.0f,0.0f));
        w.setDisplaySize(512, 512);

        tracer.setNumSamplesPerPixel(ssp);
        tracer.setNumSamplesPerLightSoure(spl);
        tracer.setTraceDepeth(maxdepth);
        tracer.render();
        w.loadImage(tracer.image);
        w.saveImage(savepath);
}

void scene03_VeachMIS(PathTracer &tracer, MainWindow &w, int ssp,int spl, int maxdepth, string openpath, string savepath)
{
        Scene scene3;
        scene3.loadObjects(openpath);
        scene3.camera.initialize(QVector3D(0,2,15), QVector3D(0,1.69521,14.0476), QVector3D(0.0,0.952421,-0.304787));
        scene3.camera.setViewPort(28, 1152, 864);
        SphereLight light_scene3_01;
        light_scene3_01.setLight(QVector3D(-10, 10, 4), QVector3D(800,800,800), 0.5f);
        scene3.lights.push_back(&light_scene3_01);
        SphereLight light_scene3_02;
        light_scene3_02.setLight(QVector3D(3.75, 0, 0), QVector3D(901.803,901.803,901.803), 0.033f);
        scene3.lights.push_back(&light_scene3_02);
        SphereLight light_scene3_03;
        light_scene3_03.setLight(QVector3D(1.25,0,0), QVector3D(100,100,100), 0.1f);
        scene3.lights.push_back(&light_scene3_03);
        SphereLight light_scene3_04;
        light_scene3_04.setLight(QVector3D(-1.25,0,0), QVector3D(11.1111,11.1111,11.1111), 0.3f);
        scene3.lights.push_back(&light_scene3_04);
        SphereLight light_scene3_05;
        light_scene3_05.setLight(QVector3D(-3.75,0,0), QVector3D(1.23457,1.23457,1.234571), 0.9f);
        scene3.lights.push_back(&light_scene3_05);
        tracer.scene = &scene3;
        w.setDisplaySize(1152, 864);
        tracer.setAmbienLight(QVector3D(1,1,1));

        tracer.setNumSamplesPerPixel(ssp);
        tracer.setNumSamplesPerLightSoure(spl);
        tracer.setTraceDepeth(maxdepth);
        tracer.render();
        w.loadImage(tracer.image);
        w.saveImage(savepath);
}

void myscene_IronMan(PathTracer &tracer, MainWindow &w, int ssp, int spl,int maxdepth, string openpath, string savepath)
{
    tracer.scene = new Scene;
    tracer.scene->loadObjects(openpath);
    tracer.scene->camera.initialize(QVector3D(0.0, 110.0f, 260.0f), QVector3D(0, 110.0f, 259.9f), QVector3D(0, 1, 0));
    tracer.scene->camera.setViewPort(60, 768, 512);
    SphereLight light3;
    light3.setLight(QVector3D(-150, 110, 200),QVector3D(190,190,190), 20.0f);
    tracer.scene->lights.push_back(&light3);
    tracer.setAmbienLight(QVector3D(0.2f,0.2f,0.2f));
    w.setDisplaySize(768,512);

    tracer.setNumSamplesPerPixel(ssp);
    tracer.setNumSamplesPerLightSoure(spl);
    tracer.setTraceDepeth(maxdepth);
    tracer.render();
    w.loadImage(tracer.image);
    w.saveImage(savepath);

}
