#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QFileDialog>
#include <QMatrix4x4>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPainter>
#include <QSettings>
#include <QTimer>
#include <QWheelEvent>
#include <cmath>
#include <memory>
#include <vector>

#include "../../model/model.h"
#include "QtGifImage/gifimage/qgifimage.h"
#include "projection_strategy.h"
#include "settings_manager.h"

namespace s21 {
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit GLWidget(QWidget *parent = nullptr);
  ~GLWidget() { saveSettings(); };
  void read_file(const std::string &filename);
  std::pair<size_t, size_t> get_v_i_count() {
    return {m_model_.get_v_count(), m_model_.get_i_count()};
  }

  void set_movex(double val) { translateX = val; }
  void set_movey(double val) { translateY = val; }
  void set_movez(double val) { translateZ = val; }

  void set_rotatex(double val) { angleX = val; }
  void set_rotatey(double val) { angleY = val; }
  void set_rotatez(double val) { angleZ = val; }

  void set_scale(double val) {
    scale = val;
    scaleX = scale;
    scaleY = scale;
    scaleZ = scale;
  }
  double get_scale() { return scale; }

  void set_v_size(double val) { vertex_size = val; }
  void set_v_type(int type) { vertex_type = type; }
  double get_v_size() { return vertex_size; }
  int get_v_type() { return vertex_type; }

  void set_l_size(double val) { line_size = val; }
  void set_l_type(int type) { line_type = type; }
  double get_l_size() { return line_size; }
  int get_l_type() { return line_type; }
  void set_projection(int type) {
    if (type)
      projection_strategy = std::make_unique<Centrall_strategy>();
    else
      projection_strategy = std::make_unique<Parallel_strategy>();
  }

  int get_projection() {
    return (dynamic_cast<Centrall_strategy *>(projection_strategy.get()) !=
            nullptr)
               ? 1
               : 0;
  }
  void set_background_color(QColor color) { background_color = color; }
  QColor get_background_color() { return background_color; }

  void set_vertex_color(QColor color) { vertex_color = color; }

  void set_line_color(QColor color) { line_color = color; }

  QColor get_vertex_color() { return vertex_color; }

  QColor get_line_color() { return line_color; }

  void loadSettings();
  void saveSettings();
 signals:
  void rotationChanged(double angleX, double angleY);
  void scaleChanged(double scale);
  void background_color_changed();
  void vertex_color_changed();
  void line_color_changed();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  // события мышки
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

 public slots:
  void saveBmp();
  void saveJpeg();
  void saveGif();
  void captureFrame();

 private:
  QMatrix4x4 rotationMatrix() const;
  QMatrix4x4 scalingMatrix() const;
  QMatrix4x4 translationMatrix() const;
  void pro();

  double scale = 1.0;
  double angle = 0.0;  // угол

  float angleX = 0.0f;
  float angleY = 0.0f;
  float angleZ = 0.0f;
  float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
  float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;

  double line_size = 0.01;
  double vertex_size = 0.01;
  int vertex_type = 0;
  int line_type = 0;

  double rotate_limit = 360;

  QColor background_color = QColor::fromRgbF(0.1f, 0.1f, 0.1f);
  QColor vertex_color = Qt::red;
  QColor line_color = QColor::fromRgbF(0.0f, 1.0f, 0.0f);

  bool perspectiveProjection = false;

  // для отслеживания мышки
  QPoint lastMousePos;
  Model m_model_;
  std::vector<double> vertices;
  std::vector<unsigned int> indices;

  QGifImage *gif_ = nullptr;
  QTimer *timer_ = nullptr;
  int frameCount_ = 0;

  std::unique_ptr<Projection_strategy> projection_strategy;
};
}  // namespace s21
#endif  // GLWIDGET_H
