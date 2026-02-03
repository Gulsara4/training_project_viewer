#include "glwidget.h"

#include <QPainter>
#include <string>
#define GL_SILENCE_DEPRECATION
using namespace s21;
GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  projection_strategy = std::make_unique<Parallel_strategy>();
  loadSettings();
}

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();

  glEnable(GL_DEPTH_TEST);
  glClearColor(background_color.redF(), background_color.greenF(),
               background_color.blueF(), background_color.alphaF());
}

void GLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect = float(w) / float(h ? h : 1);
  glOrtho(-1 * aspect, 1 * aspect, -1, 1, -10, 10);
}

void GLWidget::paintGL() {
  glClearColor(background_color.redF(), background_color.greenF(),
               background_color.blueF(), 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // проекция

  QMatrix4x4 projection;
  float aspect = float(width()) / float(height());
  projection = projection_strategy->create(aspect);
  glLoadMatrixf(projection.constData());

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  QMatrix4x4 model;
  model.setToIdentity();
  model *= translationMatrix();
  model *= rotationMatrix();
  model *= scalingMatrix();

  glMultMatrixf(model.constData());

  // glScaled(scale, scale, scale);
  // glRotated(angleX, 1.0, 0.0, 0.0);
  // glRotated(angleY, 0.0, 1.0, 0.0);
  // glRotated(angleZ, 0.0, 0.0, 1.0);

  // линии
  if (line_type == 1) {
    glLineStipple(1, 0x25);
    glEnable(GL_LINE_STIPPLE);
  } else if (line_type == 0) {
    glDisable(GL_LINE_STIPPLE);
  }

  glColor3d(line_color.redF(), line_color.greenF(), line_color.blueF());
  glLineWidth(line_size);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
  glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, indices.data());
  glDisableClientState(GL_VERTEX_ARRAY);

  if (vertex_type != 2) {
    // точки
    glPointSize(vertex_size);
    if (vertex_type == 1) glEnable(GL_POINT_SMOOTH);
    glColor3d(vertex_color.redF(), vertex_color.greenF(), vertex_color.blueF());

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
    glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);

    if (vertex_type == 1) glDisable(GL_POINT_SMOOTH);
  }
}

// ======== МЫШКА =========

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastMousePos.x();
  int dy = event->y() - lastMousePos.y();

  if (event->buttons() & Qt::LeftButton) {
    angleX += dy * 0.1f;
    angleY += dx * 0.1f;
    emit rotationChanged(angleX, angleY);
    update();  // перерисовать
  }

  lastMousePos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  scale += event->angleDelta().y() / 1200.0;  // шаг зума
  if (scale < 0.1) scale = 0.1;
  set_scale(scale);
  emit scaleChanged(scale);
  update();
}

void GLWidget::read_file(const std::string &filename) {
  m_model_.set_filename(filename);
  m_model_.load_model();
  vertices = m_model_.get_v();
  indices = m_model_.get_i();
}

QMatrix4x4 GLWidget::rotationMatrix() const {
  return m_model_.rotationMatrix(angleX, angleY, angleZ);
}

QMatrix4x4 GLWidget::scalingMatrix() const {
  return m_model_.scalingMatrix(scale);
}

QMatrix4x4 GLWidget::translationMatrix() const {
  return m_model_.translationMatrix(translateX, translateY, translateZ - 5);
}

void GLWidget::saveSettings() {
  auto &settings = Settings_manager::instance().settings();

  settings.beginGroup("ViewSettings");

  settings.setValue("vertex_type", vertex_type);
  settings.setValue("vertex_color", vertex_color);
  settings.setValue("vertex_size", vertex_size);

  settings.setValue("line_type", line_type);
  settings.setValue("line_color", line_color);
  settings.setValue("line_size", line_size);

  settings.setValue("background_color", background_color);
  settings.setValue("perspectiveProjection", perspectiveProjection);

  settings.endGroup();
}

void GLWidget::loadSettings() {
  auto &settings = Settings_manager::instance().settings();
  settings.beginGroup("ViewSettings");

  vertex_type = settings.value("vertex_type", 0).toInt();
  vertex_color =
      settings.value("vertex_color", QColor(Qt::red)).value<QColor>();
  vertex_size = settings.value("vertex_size", 0.01).toDouble();

  line_type = settings.value("line_type", 0).toInt();
  line_color = settings.value("line_color", QColor(Qt::green)).value<QColor>();
  line_size = settings.value("line_size", 0.01).toDouble();

  background_color =
      settings.value("background_color", QColor::fromRgbF(0.1f, 0.1f, 0.1f))
          .value<QColor>();
  bool p = settings.value("perspectiveProjection", false).toBool();
  if (p)
    projection_strategy = std::make_unique<Centrall_strategy>();
  else
    projection_strategy = std::make_unique<Parallel_strategy>();

  settings.endGroup();
}

void GLWidget::saveBmp() {
  QString path = QFileDialog::getSaveFileName(
      this, tr("Save as BMP"), "screenshot.bmp", "Bitmap Image (*.bmp)");
  if (!path.isEmpty()) {
    QImage img = grabFramebuffer();
    img.save(path, "BMP");
  }
}

void GLWidget::saveJpeg() {
  QString path = QFileDialog::getSaveFileName(
      this, tr("Save as JPEG"), "screenshot.jpeg", "JPEG Image (*.jpeg *.jpg)");
  if (!path.isEmpty()) {
    QImage img = grabFramebuffer();
    img.save(path, "JPEG");
  }
}

void GLWidget::saveGif() {
  frameCount_ = 0;
  gif_ = new QGifImage();
  gif_->setDefaultDelay(100);  // 10 кадров в секунду

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &GLWidget::captureFrame);
  timer_->start(100);

  QMessageBox::information(this, "Recording GIF",
                           "GIF recording started (will capture ~5s).");
}

void GLWidget::captureFrame() {
  QPixmap pix = QPixmap::fromImage(grabFramebuffer());
  QPixmap scaledPix = pix.scaled(QSize(640, 480), Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation);
  gif_->addFrame(scaledPix.toImage());

  frameCount_++;

  if (frameCount_ >= 50) {  // ~5 секунд
    timer_->stop();

    QString path = QFileDialog::getSaveFileName(
        this, tr("Save as GIF"), "animation.gif", "GIF Image (*.gif)");
    if (!path.isEmpty()) gif_->save(path);

    QMessageBox::information(this, "GIF Saved",
                             "Animation saved successfully!");

    delete timer_;
    delete gif_;
    timer_ = nullptr;
    gif_ = nullptr;
  }
}
