#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QString>
#include <format>
#include <string>

#include "glwidget.h"
#include "viewer_facade.h"
using namespace s21;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void connect_spin_and_slider_movement();
  void connect_spin_and_slider_rotation();
  void connect_spin_and_slider_scale();
  void setup_default_values();
 private slots:
  void on_pushButton_open_file();

  void on_pb_bg_c();
  void on_pb_v_c();
  void on_pb_l_c();

  void on_movex_clicked(double val);
  void on_movey_clicked(double val);
  void on_movez_clicked(double val);

  void on_rotateX_changed(double val);
  void on_rotateY_changed(double val);
  void on_rotateZ_changed(double val);
  void on_scale_changed(double val);

  void on_vertex_size_change(double val);
  void on_line_size_change(double val);

  void on_vertex_type_change(int type);
  void on_line_type_change(int type);
  void on_change_projection(int type);

 private:
  Ui::MainWindow *ui;
  QString currentFile;
  GLWidget *g;
  Viewer_facade *v_facade;
};

#endif  // MAINWINDOW_H
