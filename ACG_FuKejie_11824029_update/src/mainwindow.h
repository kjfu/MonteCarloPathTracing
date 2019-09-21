#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QColor>
#include <string>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadImage(QColor **&img);
    void saveImage(string imageSavePath);
    void initialize();

    void setDisplaySize(int width, int height);
private slots:

    void on_quadLight_pushButton_add_clicked();

    void on_sphereLight_pushButton_add_clicked();

    void on_camera_pushButton_apply_clicked();

    void on_scene01_pushButton_clicked();

    void on_pathTracer_pushButton_apply_clicked();

    void on_scene02_pushButton_clicked();

    void on_scene03_pushButton_clicked();

    void on_render_pushButton_clicked();

    void on_actionScene01_room_triggered();

    void on_actionScene02_cup_triggered();

    void on_actionScene03_VeachMIS_triggered();

    void on_myscene_pushButton_clicked();

    void on_actionMy_Scene_Iron_Man_triggered();

    void on_yourscene_pushButton_clicked();

    void on_actionLoad_triggered();

    void on_actionSave_as_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
