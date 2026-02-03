#ifndef VIEWER_FACADE_H
#define VIEWER_FACADE_H

#include <QColor>
#include <QString>
#include <stdexcept>

#include "glwidget.h"
// pattern facade
namespace s21 {
class Viewer_facade {
 public:
  Viewer_facade(GLWidget *g) : gl_widget(g) {}

  bool loadModel(const QString &file_path) {
    try {
      gl_widget->read_file(file_path.toStdString());
      return true;
    } catch (const std::exception &e) {
      last_error = e.what();
      return false;
    }
  }

  void update_View() { gl_widget->update(); }
  void set_line_color(const QColor &c) { gl_widget->set_line_color(c); }
  void set_vertex_color(const QColor &c) { gl_widget->set_vertex_color(c); }
  void set_background_color(const QColor &c) {
    gl_widget->set_background_color(c);
  }

  void set_projection(int type) { gl_widget->set_projection(type); }
  void setScale(double s) { gl_widget->set_scale(s); }

  std::pair<size_t, size_t> getCounts() const {
    return gl_widget->get_v_i_count();
  }

  QString getLastError() const { return QString::fromStdString(last_error); }

 private:
  GLWidget *gl_widget;
  std::string last_error;
};
}  // namespace s21
#endif  // VIEWER_FACADE_H
