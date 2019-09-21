#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        setWindowTitle("Fu Kejie's Monte Carlo Path Tracer");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage(QColor **&img)
{
    ui->display->loadImage(img);
}

void MainWindow::saveImage(string imageSavePath)
{
    this->ui->display->setImageSavePath(imageSavePath);
    this->ui->display->save();
}

void MainWindow::initialize()
{
    this->ui->textBrowser->clear();
    this->ui->display->initialize();
    QString textout;
    textout = "[Fu Kejie's Path Tracer]";
    this->ui->textBrowser->append(textout);
}



void MainWindow::setDisplaySize(int width, int height)
{
    this->ui->display->setSize(width, height);
}


/*--------Path tracer set-----------*/

void MainWindow::on_pathTracer_pushButton_apply_clicked()
{
    this->ui->display->pathTracer.setNumSamplesPerPixel( this->ui->pathTracer_lineEdit_sperpixel->text().toInt() );
    this->ui->display->pathTracer.setNumSamplesPerLightSoure( this->ui->pathTracer_lineEdit_sperlight->text().toInt() );
    this->ui->display->pathTracer.setTraceDepeth( this->ui->pathTracer_lineEdit_maxdepth->text().toInt());
    this->ui->display->pathTracer.setAmbienLight( QVector3D(this->ui->pathTracer_lineEdit_ambient_x->text().toFloat(),
                                                           this->ui->pathTracer_lineEdit_ambient_y->text().toFloat(),
                                                           this->ui->pathTracer_lineEdit_ambient_z->text().toFloat()) );
    QString textout;
    textout = ">>>  Set Path Tracer: Samples Per Pixel = " + QString::number(this->ui->display->pathTracer.numPixelSamples)       + ", "
                                    "Samples Per Light = " + QString::number(this->ui->display->pathTracer.numDirectLightSamples) + ", "
                                        "Maximum Depth = " + QString::number(this->ui->display->pathTracer.maxDepth)              + ", "
                                       "Ambient light = [" + QString::number(this->ui->display->pathTracer.ambientLight[0])       + ", "
                                                           + QString::number(this->ui->display->pathTracer.ambientLight[1])       + ", "
                                                           + QString::number(this->ui->display->pathTracer.ambientLight[2])       + "];";
    this->ui->textBrowser->append(textout);
    this->ui->display->pathTracerHaveSet = true;
}

/*--------Quad Light set-------*/
void MainWindow::on_quadLight_pushButton_add_clicked()
{
    QuadLight *quadlight = new QuadLight;
    QVector3D center(this->ui->quadLight_lineEdit_center_x->text().toFloat(),
                     this->ui->quadLight_lineEdit_center_y->text().toFloat(),
                     this->ui->quadLight_lineEdit_center_z->text().toFloat());

    QVector3D normal(this->ui->quadLight_lineEdit_normal_x->text().toFloat(),
                     this->ui->quadLight_lineEdit_normal_y->text().toFloat(),
                     this->ui->quadLight_lineEdit_normal_z->text().toFloat());

    QVector3D up(this->ui->quadLight_lineEdit_up_x->text().toFloat(),
                 this->ui->quadLight_lineEdit_up_y->text().toFloat(),
                 this->ui->quadLight_lineEdit_up_z->text().toFloat());

    QVector2D size(this->ui->quadLight_lineEdit_size_w->text().toFloat(),
                   this->ui->quadLight_lineEdit_size_h->text().toFloat());

    QVector3D le(this->ui->quadLight_lineEdit_le_x->text().toFloat(),
                 this->ui->quadLight_lineEdit_le_y->text().toFloat(),
                 this->ui->quadLight_lineEdit_le_z->text().toFloat());

    quadlight->setLight(center, size, normal, up,le);
    this->ui->display->pathTracer.scene->lights.push_back(quadlight);

    QString textout;
    textout = ">>>  Add Light"+QString::number(this->ui->display->pathTracer.scene->lights.size()-1)+": Type = Quad, "
              "Center = [" + QString::number(center[0]) + ", " + QString::number(center[1]) + ", " + QString::number(center[2]) + "], "
              "Normal = [" + QString::number(normal[0]) + ", " + QString::number(normal[1]) + ", " + QString::number(normal[2]) + "], "
                  "Up = [" + QString::number(up[0])     + ", " + QString::number(up[1])     + ", " + QString::number(up[2])     + "], "
                "Size = [" + QString::number(size[0])   + ", " + QString::number(size[1])   + "], "
                  "le = [" + QString::number(le[0])     + ", " + QString::number(le[1])     + ", " + QString::number(le[2])     + "];";
    this->ui->textBrowser->append(textout);
}


/*-----------Sphere Light set-----------------------*/
void MainWindow::on_sphereLight_pushButton_add_clicked()
{

    SphereLight *sphereLight = new SphereLight;
    QVector3D position(this->ui->sphereLight_lineEdit_position_x->text().toFloat(),
                       this->ui->sphereLight_lineEdit_position_y->text().toFloat(),
                       this->ui->sphereLight_lineEdit_position_z->text().toFloat());

    QVector3D le(this->ui->sphereLight_lineEdit_le_x->text().toFloat(),
                 this->ui->sphereLight_lineEdit_le_y->text().toFloat(),
                 this->ui->sphereLight_lineEdit_le_z->text().toFloat());

    float radius = this->ui->sphereLight_lineEdit_radius->text().toFloat();
    sphereLight->setLight(position, le, radius);
    this->ui->display->pathTracer.scene->lights.push_back(sphereLight);

    QString textout;
    textout = ">>>  Add Light"+QString::number(this->ui->display->pathTracer.scene->lights.size()-1)+": Type = Sphere, "
              "position = [" + QString::number(position[0]) + ", " + QString::number(position[1]) + ", " + QString::number(position[2]) + "], "
                    "le = [" + QString::number(le[0])       + ", " + QString::number(le[1])       + ", " + QString::number(le[2])       + "], "
                "Radius = " +   QString::number(radius)     + ";";
    this->ui->textBrowser->append(textout);
}

/*---------------Camera set----------------------*/
void MainWindow::on_camera_pushButton_apply_clicked()
{

    QVector3D position(this->ui->camera_lineEdit_position_x->text().toFloat(),
                       this->ui->camera_lineEdit_position_y->text().toFloat(),
                       this->ui->camera_lineEdit_position_z->text().toFloat());

    QVector3D lookat(this->ui->camera_lineEdit_lookat_x->text().toFloat(),
                     this->ui->camera_lineEdit_lookat_y->text().toFloat(),
                     this->ui->camera_lineEdit_lookat_z->text().toFloat());

    QVector3D up(this->ui->camera_lineEdit_up_x->text().toFloat(),
                 this->ui->camera_lineEdit_up_y->text().toFloat(),
                 this->ui->camera_lineEdit_up_z->text().toFloat());
    float fovy = this->ui->camera_lineEdit_fovy->text().toFloat();
    float width = this->ui->camera_lineEdit_width->text().toFloat();
    float height = this->ui->camera_lineEdit_height->text().toFloat();
    ui->display->pathTracer.scene->camera.initialize(position, lookat, up);
    ui->display->pathTracer.scene->camera.setViewPort(fovy, width, height);

    this->ui->display->setSize(int(width), int(height));

    QString textout;
    textout = ">>>  Set Camera: Position =[" + QString::number(position[0]) + ", " + QString::number(position[1]) + ", " + QString::number(position[2])+"], "
                                "Lookat = [" + QString::number(lookat[0])   + ", " + QString::number(lookat[1])   + ", " + QString::number(lookat[2]) + "], "
                                "Up = ["     + QString::number(up[0])       + ", " + QString::number(up[1])       + ", " + QString::number(up[2])     + "], "
                                "FovY = "+ QString::number(fovy) +"°, Width = " + QString::number(width)+" Pixel, Height = " + QString::number(height) + " Pixel;";
    this->ui->textBrowser->append(textout);

    this->ui->display->cameraHaveSet = true;
}


/*-------------------scene01---------------------------*/
void MainWindow::on_scene01_pushButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(nullptr,tr("Open Scene01: room.obj"), "", tr("room.obj(room.obj)"));
    if (filepath.length()<5)
    {
        return;
    }
    this->initialize();
    QString textout;
    textout = ">>> Scene01 default:";
    this->ui->textBrowser->append(textout);
    this->ui->display->pathTracer.scene->loadObjects(filepath.toStdString());

    this->ui->display->objectsHaveLoad = true;

    textout = ">>>  Objects: Path = [" + filepath +"], Triangles =" + QString::number(this->ui->display->pathTracer.scene->triangles.size())+";";
    this->ui->textBrowser->append(textout);

    //Set Light
    this->ui->sphereLight_lineEdit_position_x->setText("0.0");
    this->ui->sphereLight_lineEdit_position_y->setText("1.589");
    this->ui->sphereLight_lineEdit_position_z->setText("-1.274");
    this->ui->sphereLight_lineEdit_le_x->setText("50");
    this->ui->sphereLight_lineEdit_le_y->setText("50");
    this->ui->sphereLight_lineEdit_le_z->setText("40");
    this->ui->sphereLight_lineEdit_radius->setText("0.2");
    this->on_sphereLight_pushButton_add_clicked();

    // Set camera
    this->ui->camera_lineEdit_position_x->setText("0");
    this->ui->camera_lineEdit_position_y->setText("0");
    this->ui->camera_lineEdit_position_z->setText("4");
    this->ui->camera_lineEdit_lookat_x->setText("0");
    this->ui->camera_lineEdit_lookat_y->setText("0");
    this->ui->camera_lineEdit_lookat_z->setText("0");
    this->ui->camera_lineEdit_up_x->setText("0");
    this->ui->camera_lineEdit_up_y->setText("1.0");
    this->ui->camera_lineEdit_up_z->setText("0");
    this->ui->camera_lineEdit_fovy->setText("50.0");
    this->ui->camera_lineEdit_width->setText("512");
    this->ui->camera_lineEdit_height->setText("512");
    this->on_camera_pushButton_apply_clicked();


    // set path tracer
    this->ui->pathTracer_lineEdit_sperpixel->setText("25");
    this->ui->pathTracer_lineEdit_sperlight->setText("25");
    this->ui->pathTracer_lineEdit_maxdepth->setText("10");
    this->ui->pathTracer_lineEdit_ambient_x->setText("1.0");
    this->ui->pathTracer_lineEdit_ambient_y->setText("1.0");
    this->ui->pathTracer_lineEdit_ambient_z->setText("1.0");
    this->on_pathTracer_pushButton_apply_clicked();
}


/*-------------------scene02---------------------------*/
void MainWindow::on_scene02_pushButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(nullptr,tr("Open Scene02: cup.obj"), "", tr("cup.obj(cup.obj)"));
    if (filepath.length()<5)
    {
        return;
    }
    this->initialize();
    QString textout;
    textout = ">>> Scene02 default:";
    this->ui->textBrowser->append(textout);
    this->ui->display->pathTracer.scene->loadObjects(filepath.toStdString());

        this->ui->display->objectsHaveLoad = true;

    textout = ">>>  Objects: Path = [" + filepath + "], Triangles =" + QString::number(this->ui->display->pathTracer.scene->triangles.size())+";";
    this->ui->textBrowser->append(textout);

    //Set Light
    this->ui->quadLight_lineEdit_center_x->setText("-2.758771896");
    this->ui->quadLight_lineEdit_center_y->setText("1.5246");
    this->ui->quadLight_lineEdit_center_z->setText("0.0");
    this->ui->quadLight_lineEdit_normal_x->setText("1.0");
    this->ui->quadLight_lineEdit_normal_y->setText("0.0");
    this->ui->quadLight_lineEdit_normal_z->setText("0.0");
    this->ui->quadLight_lineEdit_le_x->setText("40");
    this->ui->quadLight_lineEdit_le_y->setText("40");
    this->ui->quadLight_lineEdit_le_z->setText("40");
    this->ui->quadLight_lineEdit_size_w->setText("1.0");
    this->ui->quadLight_lineEdit_size_h->setText("1.0");
    this->ui->quadLight_lineEdit_up_x->setText("0.0");
    this->ui->quadLight_lineEdit_up_y->setText("1.0");
    this->ui->quadLight_lineEdit_up_z->setText("0.0");
    this->on_quadLight_pushButton_add_clicked();

    // Set camera
    this->ui->camera_lineEdit_position_x->setText("0.0");
    this->ui->camera_lineEdit_position_y->setText("0.64");
    this->ui->camera_lineEdit_position_z->setText("0.52");
    this->ui->camera_lineEdit_lookat_x->setText("0.0");
    this->ui->camera_lineEdit_lookat_y->setText("0.4");
    this->ui->camera_lineEdit_lookat_z->setText("0.3");
    this->ui->camera_lineEdit_up_x->setText("0");
    this->ui->camera_lineEdit_up_y->setText("1.0");
    this->ui->camera_lineEdit_up_z->setText("0");
    this->ui->camera_lineEdit_fovy->setText("60.0");
    this->ui->camera_lineEdit_width->setText("512");
    this->ui->camera_lineEdit_height->setText("512");
    this->on_camera_pushButton_apply_clicked();


    // set path tracer
    this->ui->pathTracer_lineEdit_sperpixel->setText("25");
    this->ui->pathTracer_lineEdit_sperlight->setText("25");
    this->ui->pathTracer_lineEdit_maxdepth->setText("10");
    this->ui->pathTracer_lineEdit_ambient_x->setText("0.0");
    this->ui->pathTracer_lineEdit_ambient_y->setText("0.0");
    this->ui->pathTracer_lineEdit_ambient_z->setText("0.0");
    this->on_pathTracer_pushButton_apply_clicked();

}

/*------------------scene03-------------------------*/
void MainWindow::on_scene03_pushButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(nullptr,tr("Open Scene03: VeachMIS.obj"), "", tr("VeachMIS.obj(VeachMIS.obj)"));
    if (filepath.length()<5)
    {
        return;
    }
    this->initialize();
    QString textout;
    textout = ">>> Scene03 default:";
    this->ui->textBrowser->append(textout);
    this->ui->display->pathTracer.scene->loadObjects(filepath.toStdString());

        this->ui->display->objectsHaveLoad = true;

    textout = ">>>  Objects: Path = [" + filepath +"], Triangles =" + QString::number(this->ui->display->pathTracer.scene->triangles.size())+";";
    this->ui->textBrowser->append(textout);

    //Set Light
    this->ui->sphereLight_lineEdit_position_x->setText("-10.0");
    this->ui->sphereLight_lineEdit_position_y->setText("10.0");
    this->ui->sphereLight_lineEdit_position_z->setText("4.0");
    this->ui->sphereLight_lineEdit_le_x->setText("800");
    this->ui->sphereLight_lineEdit_le_y->setText("800");
    this->ui->sphereLight_lineEdit_le_z->setText("800");
    this->ui->sphereLight_lineEdit_radius->setText("0.5");
    this->on_sphereLight_pushButton_add_clicked();

    this->ui->sphereLight_lineEdit_position_x->setText("3.75");
    this->ui->sphereLight_lineEdit_position_y->setText("0.0");
    this->ui->sphereLight_lineEdit_position_z->setText("0.0");
    this->ui->sphereLight_lineEdit_le_x->setText("901.803");
    this->ui->sphereLight_lineEdit_le_y->setText("901.803");
    this->ui->sphereLight_lineEdit_le_z->setText("801.803");
    this->ui->sphereLight_lineEdit_radius->setText("0.033");
    this->on_sphereLight_pushButton_add_clicked();

    this->ui->sphereLight_lineEdit_position_x->setText("1.25");
    this->ui->sphereLight_lineEdit_position_y->setText("0.0");
    this->ui->sphereLight_lineEdit_position_z->setText("0.0");
    this->ui->sphereLight_lineEdit_le_x->setText("100");
    this->ui->sphereLight_lineEdit_le_y->setText("100");
    this->ui->sphereLight_lineEdit_le_z->setText("100");
    this->ui->sphereLight_lineEdit_radius->setText("0.1");
    this->on_sphereLight_pushButton_add_clicked();

    this->ui->sphereLight_lineEdit_position_x->setText("-1.25");
    this->ui->sphereLight_lineEdit_position_y->setText("0.0");
    this->ui->sphereLight_lineEdit_position_z->setText("0.0");
    this->ui->sphereLight_lineEdit_le_x->setText("11.1111");
    this->ui->sphereLight_lineEdit_le_y->setText("11.1111");
    this->ui->sphereLight_lineEdit_le_z->setText("11.1111");
    this->ui->sphereLight_lineEdit_radius->setText("0.3");
    this->on_sphereLight_pushButton_add_clicked();

    this->ui->sphereLight_lineEdit_position_x->setText("-3.75");
    this->ui->sphereLight_lineEdit_position_y->setText("0.0");
    this->ui->sphereLight_lineEdit_position_z->setText("0.0");
    this->ui->sphereLight_lineEdit_le_x->setText("1.23457");
    this->ui->sphereLight_lineEdit_le_y->setText("1.23457");
    this->ui->sphereLight_lineEdit_le_z->setText("1.23457");
    this->ui->sphereLight_lineEdit_radius->setText("0.9");
    this->on_sphereLight_pushButton_add_clicked();


    // Set camera
    this->ui->camera_lineEdit_position_x->setText("0.0");
    this->ui->camera_lineEdit_position_y->setText("2.0");
    this->ui->camera_lineEdit_position_z->setText("15.0");
    this->ui->camera_lineEdit_lookat_x->setText("0.0");
    this->ui->camera_lineEdit_lookat_y->setText("1.69521");
    this->ui->camera_lineEdit_lookat_z->setText("14.0476");
    this->ui->camera_lineEdit_up_x->setText("0.0");
    this->ui->camera_lineEdit_up_y->setText("0.952421");
    this->ui->camera_lineEdit_up_z->setText("-0.304787");
    this->ui->camera_lineEdit_fovy->setText("28");
    this->ui->camera_lineEdit_width->setText("1152");
    this->ui->camera_lineEdit_height->setText("864");
    this->on_camera_pushButton_apply_clicked();


    // set path tracer
    this->ui->pathTracer_lineEdit_sperpixel->setText("25");
    this->ui->pathTracer_lineEdit_sperlight->setText("25");
    this->ui->pathTracer_lineEdit_maxdepth->setText("10");
    this->ui->pathTracer_lineEdit_ambient_x->setText("1.0");
    this->ui->pathTracer_lineEdit_ambient_y->setText("1.0");
    this->ui->pathTracer_lineEdit_ambient_z->setText("1.0");
    this->on_pathTracer_pushButton_apply_clicked();
}

void MainWindow::on_render_pushButton_clicked()
{
    if (this->ui->display->objectsHaveLoad ==false)
    {
        this->ui->textBrowser->append(">>> Rendering failed: Please load objects, and try again!");
        return;
    }

    if (this->ui->display->cameraHaveSet ==false)
    {
        this->ui->textBrowser->append(">>> Rendering failed: Please set camera, and try again!");
        return;
    }

    if (this->ui->display->pathTracerHaveSet ==false)
    {
        this->ui->textBrowser->append(">>> Rendering failed: Please set path tracer, and try again!");
        return;
    }

    srand(time(nullptr));
    this->ui->textBrowser->append(">>> Now is rendering......");
    this->ui->display->pathTracer.render();
    this->ui->display->image = this->ui->display->pathTracer.image;
    this->ui->textBrowser->append(">>> Rendering successed!");
    this->ui->display->canSave = true;
    this->ui->display->update();


}

void MainWindow::on_actionScene01_room_triggered()
{
    this->on_scene01_pushButton_clicked();
}

void MainWindow::on_actionScene02_cup_triggered()
{
    this->on_scene02_pushButton_clicked();
}


void MainWindow::on_actionScene03_VeachMIS_triggered()
{
    this->on_scene03_pushButton_clicked();
}

void MainWindow::on_myscene_pushButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(nullptr,tr("My Scene: IronMan.obj"), "", tr("IronMan.obj(IronMan.obj)"));
    if (filepath.length()<5)
    {
        return;
    }
    this->initialize();
    QString textout;
    textout = ">>> Scene01 default:";
    this->ui->textBrowser->append(textout);
    this->ui->display->pathTracer.scene->loadObjects(filepath.toStdString());

    this->ui->display->objectsHaveLoad = true;

    textout = ">>>  Objects: Path = [" + filepath +"], Triangles =" + QString::number(this->ui->display->pathTracer.scene->triangles.size())+";";
    this->ui->textBrowser->append(textout);

    //Set Light
    this->ui->sphereLight_lineEdit_position_x->setText("-150");
    this->ui->sphereLight_lineEdit_position_y->setText("110");
    this->ui->sphereLight_lineEdit_position_z->setText("200");
    this->ui->sphereLight_lineEdit_le_x->setText("190");
    this->ui->sphereLight_lineEdit_le_y->setText("190");
    this->ui->sphereLight_lineEdit_le_z->setText("190");
    this->ui->sphereLight_lineEdit_radius->setText("20");
    this->on_sphereLight_pushButton_add_clicked();

    // Set camera
    this->ui->camera_lineEdit_position_x->setText("0.0");
    this->ui->camera_lineEdit_position_y->setText("110.0");
    this->ui->camera_lineEdit_position_z->setText("260");
    this->ui->camera_lineEdit_lookat_x->setText("0.0");
    this->ui->camera_lineEdit_lookat_y->setText("110.0");
    this->ui->camera_lineEdit_lookat_z->setText("259.9");
    this->ui->camera_lineEdit_up_x->setText("0");
    this->ui->camera_lineEdit_up_y->setText("1.0");
    this->ui->camera_lineEdit_up_z->setText("0");
    this->ui->camera_lineEdit_fovy->setText("60.0");
    this->ui->camera_lineEdit_width->setText("768");
    this->ui->camera_lineEdit_height->setText("512");
    this->on_camera_pushButton_apply_clicked();


    // set path tracer
    this->ui->pathTracer_lineEdit_sperpixel->setText("25");
    this->ui->pathTracer_lineEdit_sperlight->setText("25");
    this->ui->pathTracer_lineEdit_maxdepth->setText("10");
    this->ui->pathTracer_lineEdit_ambient_x->setText("0.2");
    this->ui->pathTracer_lineEdit_ambient_y->setText("0.2");
    this->ui->pathTracer_lineEdit_ambient_z->setText("0.2");
    this->on_pathTracer_pushButton_apply_clicked();

}

void MainWindow::on_actionMy_Scene_Iron_Man_triggered()
{
    this->on_myscene_pushButton_clicked();
}

void MainWindow::on_yourscene_pushButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(nullptr,tr("Load obj: *.obj"), "", tr("OBJ File(*.obj)"));
    if (filepath.length()<5)
    {
        return;
    }
    this->initialize();
    QString textout;
    textout = ">>>Set your scene:";
    this->ui->textBrowser->append(textout);
    this->ui->display->pathTracer.scene->loadObjects(filepath.toStdString());

    this->ui->display->objectsHaveLoad = true;

    textout = ">>>  Objects: Path = [" + filepath +"], Triangles =" + QString::number(this->ui->display->pathTracer.scene->triangles.size())+";";
    this->ui->textBrowser->append(textout);
}

void MainWindow::on_actionLoad_triggered()
{
    this->on_yourscene_pushButton_clicked();
}

void MainWindow::on_actionSave_as_triggered()
{
        if (this->ui->display->canSave == false)
        {
            this->ui->textBrowser->append(">>> Saving failed: Please render first, and save image later!");
            return;
        }
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save as: *.png"),"/home",tr("PNG File(*.png)"));
        if (fileName.length()<3)
        {
            return;
        }
        this->ui->display->imageSavePath =  fileName.toStdString();
        this->ui->display->save();
        QString textout = ">>>  Save image: Path = [" + fileName + "];";
        this->ui->textBrowser->append(textout);
}
