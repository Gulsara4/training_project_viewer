#include "mainwindow.h"

#include <QMessageBox>
#include <stdexcept>

#include "ui_mainwindow.h"
#include "viewer_facade.h"
using namespace s21;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->tabWidget->setCurrentIndex(0);
  g = ui->openGLWidget;

  v_facade = new Viewer_facade(g);

  setup_default_values();
  double step = 0.1;
  ui->spinBox_move_x->setSingleStep(step);
  ui->spinBox_move_y->setSingleStep(step);
  ui->spinBox_move_z->setSingleStep(step);
  ui->spinBox_rotate_x->setSingleStep(step);
  ui->spinBox_rotate_y->setSingleStep(step);
  ui->spinBox_rotate_z->setSingleStep(step);
  ui->spinBox_scale->setSingleStep(step);
  // file operation
  connect(ui->pushButton_open_file, &QPushButton::clicked, this,
          &MainWindow::on_pushButton_open_file);

  // перемешение по осям координат
  connect(ui->spinBox_move_x,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_movex_clicked);
  connect(ui->spinBox_move_y,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_movey_clicked);
  connect(ui->spinBox_move_z,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_movez_clicked);
  connect(ui->horizontalSlider_move_x, &QSlider::valueChanged, this,
          &MainWindow::on_movex_clicked);
  connect(ui->horizontalSlider_move_y, &QSlider::valueChanged, this,
          &MainWindow::on_movey_clicked);
  connect(ui->horizontalSlider_move_z, &QSlider::valueChanged, this,
          &MainWindow::on_movez_clicked);
  connect_spin_and_slider_movement();

  // повороты отноистельно осей координат
  connect(ui->spinBox_rotate_x,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_rotateX_changed);
  connect(ui->spinBox_rotate_y,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_rotateY_changed);
  connect(ui->spinBox_rotate_z,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_rotateZ_changed);
  connect(ui->horizontalSlider_rotate_x, &QSlider::valueChanged, this,
          &MainWindow::on_rotateX_changed);
  connect(ui->horizontalSlider_rotate_y, &QSlider::valueChanged, this,
          &MainWindow::on_rotateY_changed);
  connect(ui->horizontalSlider_rotate_z, &QSlider::valueChanged, this,
          &MainWindow::on_rotateZ_changed);
  connect_spin_and_slider_rotation();

  // управление массштабом
  connect(ui->spinBox_scale,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_scale_changed);
  connect(ui->horizontalSlider_scale, &QSlider::valueChanged, this,
          &MainWindow::on_scale_changed);
  connect_spin_and_slider_scale();

  // управление вершинами и линиями
  connect(ui->doubleSpinBox_vertex_size,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_vertex_size_change);
  connect(ui->doubleSpinBox_line_size,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_line_size_change);
  connect(ui->comboBox_vertex_type, &QComboBox::currentIndexChanged, this,
          &MainWindow::on_vertex_type_change);
  connect(ui->comboBox_line_type, &QComboBox::currentIndexChanged, this,
          &MainWindow::on_line_type_change);

  // проекция
  connect(ui->comboBox_projection, &QComboBox::currentIndexChanged, this,
          &MainWindow::on_change_projection);

  connect(ui->pushButton_background_color, &QPushButton::clicked, this,
          &MainWindow::on_pb_bg_c);
  connect(ui->pushButton_vertex_color, &QPushButton::clicked, this,
          &MainWindow::on_pb_v_c);
  connect(ui->pushButton_line_color, &QPushButton::clicked, this,
          &MainWindow::on_pb_l_c);

  connect(ui->saveBmpButton, &QPushButton::clicked, ui->openGLWidget,
          &GLWidget::saveBmp);
  connect(ui->saveJpegButton, &QPushButton::clicked, ui->openGLWidget,
          &GLWidget::saveJpeg);
  connect(ui->saveGifButton, &QPushButton::clicked, ui->openGLWidget,
          &GLWidget::saveGif);

  QString info = "File: \nVertex: \nEdge: ";
  ui->label_file_info->setText(info);
}

MainWindow::~MainWindow() {
  delete ui;
  delete v_facade;
}

void MainWindow::connect_spin_and_slider_movement() {
  // Соединяем SpinBox -> Slider
  connect(ui->spinBox_move_x,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this](double value) {
            // Блокируем сигналы слайдера чтобы избежать рекурсии
            ui->horizontalSlider_move_x->blockSignals(true);
            ui->horizontalSlider_move_x->setValue(static_cast<int>(value));
            ui->horizontalSlider_move_x->blockSignals(false);
          });

  // Соединяем Slider -> SpinBox
  connect(ui->horizontalSlider_move_x, &QSlider::valueChanged, this,
          [this](int value) {
            ui->spinBox_move_x->blockSignals(true);
            ui->spinBox_move_x->setValue(static_cast<double>(value));
            ui->spinBox_move_x->blockSignals(false);
          });

  // Соединяем SpinBox -> Slider
  connect(ui->spinBox_move_y,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this](double value) {
            // Блокируем сигналы слайдера чтобы избежать рекурсии
            ui->horizontalSlider_move_y->blockSignals(true);
            ui->horizontalSlider_move_y->setValue(static_cast<int>(value));
            ui->horizontalSlider_move_y->blockSignals(false);
          });

  // Соединяем Slider -> SpinBox
  connect(ui->horizontalSlider_move_y, &QSlider::valueChanged, this,
          [this](int value) {
            ui->spinBox_move_y->blockSignals(true);
            ui->spinBox_move_y->setValue(static_cast<double>(value));
            ui->spinBox_move_y->blockSignals(false);
          });

  // Соединяем SpinBox -> Slider
  connect(ui->spinBox_move_z,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this](double value) {
            // Блокируем сигналы слайдера чтобы избежать рекурсии
            ui->horizontalSlider_move_z->blockSignals(true);
            ui->horizontalSlider_move_z->setValue(static_cast<int>(value));
            ui->horizontalSlider_move_z->blockSignals(false);
          });

  // Соединяем Slider -> SpinBox
  connect(ui->horizontalSlider_move_z, &QSlider::valueChanged, this,
          [this](int value) {
            ui->spinBox_move_z->blockSignals(true);
            ui->spinBox_move_z->setValue(static_cast<double>(value));
            ui->spinBox_move_z->blockSignals(false);
          });
}

void MainWindow::connect_spin_and_slider_rotation() {
  // синхронизируем изменения, если пользователь управляет мышкой
  connect(ui->openGLWidget, &GLWidget::rotationChanged, this,
          [this](double x, double y) {
            ui->spinBox_rotate_x->blockSignals(true);
            ui->spinBox_rotate_y->blockSignals(true);

            ui->spinBox_rotate_x->setValue(x);
            ui->spinBox_rotate_y->setValue(y);

            ui->spinBox_rotate_x->blockSignals(false);
            ui->spinBox_rotate_y->blockSignals(false);

            ui->horizontalSlider_rotate_x->blockSignals(true);
            ui->horizontalSlider_rotate_y->blockSignals(true);
            ui->horizontalSlider_rotate_x->setValue(x);
            ui->horizontalSlider_rotate_y->setValue(y);
            ui->horizontalSlider_rotate_x->blockSignals(false);
            ui->horizontalSlider_rotate_y->blockSignals(false);
          });

  // Соединяем SpinBox -> Slider
  connect(ui->spinBox_rotate_x,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this](double value) {
            // Блокируем сигналы слайдера чтобы избежать рекурсии
            ui->horizontalSlider_rotate_x->blockSignals(true);
            ui->horizontalSlider_rotate_x->setValue(static_cast<int>(value));
            ui->horizontalSlider_rotate_x->blockSignals(false);
          });

  // Соединяем Slider -> SpinBox
  connect(ui->horizontalSlider_rotate_x, &QSlider::valueChanged, this,
          [this](int value) {
            ui->spinBox_rotate_x->blockSignals(true);
            ui->spinBox_rotate_x->setValue(static_cast<double>(value));
            ui->spinBox_rotate_x->blockSignals(false);
          });

  // Соединяем SpinBox -> Slider
  connect(ui->spinBox_rotate_y,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this](double value) {
            // Блокируем сигналы слайдера чтобы избежать рекурсии
            ui->horizontalSlider_rotate_y->blockSignals(true);
            ui->horizontalSlider_rotate_y->setValue(static_cast<int>(value));
            ui->horizontalSlider_rotate_y->blockSignals(false);
          });

  // Соединяем Slider -> SpinBox
  connect(ui->horizontalSlider_rotate_y, &QSlider::valueChanged, this,
          [this](int value) {
            ui->spinBox_rotate_y->blockSignals(true);
            ui->spinBox_rotate_y->setValue(static_cast<double>(value));
            ui->spinBox_rotate_y->blockSignals(false);
          });

  // Соединяем SpinBox -> Slider
  connect(ui->spinBox_rotate_z,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this](double value) {
            // Блокируем сигналы слайдера чтобы избежать рекурсии
            ui->horizontalSlider_rotate_z->blockSignals(true);
            ui->horizontalSlider_rotate_z->setValue(static_cast<int>(value));
            ui->horizontalSlider_rotate_z->blockSignals(false);
          });

  // Соединяем Slider -> SpinBox
  connect(ui->horizontalSlider_rotate_z, &QSlider::valueChanged, this,
          [this](int value) {
            ui->spinBox_rotate_z->blockSignals(true);
            ui->spinBox_rotate_z->setValue(static_cast<double>(value));
            ui->spinBox_rotate_z->blockSignals(false);
          });
}

void MainWindow::connect_spin_and_slider_scale() {
  // синхронизация, если пользователь управляет масштабом с помошью колесика
  // мышки
  connect(ui->openGLWidget, &GLWidget::scaleChanged, this,
          [this](double scale) {
            ui->spinBox_scale->blockSignals(true);
            ui->horizontalSlider_scale->blockSignals(true);

            ui->spinBox_scale->setValue(scale);
            ui->horizontalSlider_scale->setValue(scale);

            ui->spinBox_scale->blockSignals(false);
            ui->horizontalSlider_scale->blockSignals(false);
          });

  // Соединяем SpinBox -> Slider
  connect(ui->spinBox_scale,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this](double value) {
            // Блокируем сигналы слайдера чтобы избежать рекурсии
            ui->horizontalSlider_scale->blockSignals(true);
            ui->horizontalSlider_scale->setValue(static_cast<int>(value));
            ui->horizontalSlider_scale->blockSignals(false);
          });

  // Соединяем Slider -> SpinBox
  connect(ui->horizontalSlider_scale, &QSlider::valueChanged, this,
          [this](int value) {
            ui->spinBox_scale->blockSignals(true);
            ui->spinBox_scale->setValue(static_cast<double>(value));
            ui->spinBox_scale->blockSignals(false);
          });
}

void MainWindow::on_pushButton_open_file() {
  currentFile = QFileDialog::getOpenFileName();

  try {
    if (currentFile.isEmpty()) return;
    if (v_facade->loadModel(currentFile)) {
      int c_v = v_facade->getCounts().first / 3;
      int c_i = v_facade->getCounts().second / 4;
      std::string file_name = currentFile.toStdString();
      std::string s =
          std::format("File:\n {}\nVertex: {}\nEdge: {}", file_name, c_v, c_i);
      QString info = QString::fromStdString(s);
      ui->label_file_info->setText(info);
      v_facade->update_View();
      ui->label_filename->setText(currentFile);
    } else {
      QMessageBox::critical(this, tr("Ошибка чтения файла"),
                            v_facade->getLastError());
    }
  } catch (const std::exception &ex) {
    QMessageBox::critical(this, tr("Ошибка чтения файла"),
                          QString::fromStdString(ex.what()));
  }
}
void MainWindow::on_pb_bg_c() {
  QColor background_color = g->get_background_color();
  QColor color =
      QColorDialog::getColor(background_color, this, "Choose background color",
                             QColorDialog::DontUseNativeDialog);
  if (color.isValid()) {
    v_facade->set_background_color(color);
    v_facade->update_View();
  }
}

void MainWindow::on_pb_v_c() {
  QColor vertex_color = g->get_vertex_color();
  QColor color =
      QColorDialog::getColor(vertex_color, this, "Choose vertex color",
                             QColorDialog::DontUseNativeDialog);
  if (color.isValid()) {
    v_facade->set_vertex_color(color);
    v_facade->update_View();
  }
}

void MainWindow::on_pb_l_c() {
  QColor line_color = g->get_line_color();
  QColor color = QColorDialog::getColor(line_color, this, "Choose line color",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid()) {
    v_facade->set_line_color(color);
    v_facade->update_View();
  }
}
void MainWindow::on_movex_clicked(double val) {
  g->set_movex(val);
  g->update();
}
void MainWindow::on_movey_clicked(double val) {
  g->set_movey(val);
  g->update();
}
void MainWindow::on_movez_clicked(double val) {
  g->set_movez(val);
  g->update();
}

void MainWindow::on_rotateX_changed(double val) {
  g->set_rotatex(val);
  g->update();
}

void MainWindow::on_rotateY_changed(double val) {
  g->set_rotatey(val);
  g->update();
}

void MainWindow::on_rotateZ_changed(double val) {
  g->set_rotatez(val);
  g->update();
}

void MainWindow::on_scale_changed(double val) {
  g->set_scale(val);
  g->update();
}

void MainWindow::on_vertex_size_change(double val) {
  g->set_v_size(val);
  g->update();
}

void MainWindow::on_vertex_type_change(int type) {
  g->set_v_type(type);
  g->update();
}

void MainWindow::on_line_size_change(double val) {
  g->set_l_size(val);
  g->update();
}

void MainWindow::on_line_type_change(int type) {
  g->set_l_type(type);
  g->update();
}

void MainWindow::on_change_projection(int type) {
  v_facade->set_projection(type);
  v_facade->update_View();
}

void MainWindow::setup_default_values() {
  ui->doubleSpinBox_line_size->setValue(g->get_l_size());
  ui->doubleSpinBox_vertex_size->setValue(g->get_v_size());

  ui->spinBox_scale->setValue(g->get_scale());
  ui->horizontalSlider_scale->setValue(g->get_scale());

  ui->comboBox_line_type->setCurrentIndex(g->get_l_type());
  ui->comboBox_vertex_type->setCurrentIndex(g->get_v_type());

  ui->comboBox_projection->setCurrentIndex(g->get_projection());
}
