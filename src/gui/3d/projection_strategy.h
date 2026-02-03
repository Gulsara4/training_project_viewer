#ifndef PROJECTION_STRATEGY_H
#define PROJECTION_STRATEGY_H

// pattern Strategy

#include <QMatrix4x4>
namespace s21 {
class Projection_strategy {
 public:
  virtual ~Projection_strategy() = default;
  virtual QMatrix4x4 create(float aspect) const = 0;
};

class Parallel_strategy : public Projection_strategy {
 public:
  QMatrix4x4 create(float aspect) const override {
    QMatrix4x4 projection;
    projection.ortho(-aspect * 2, aspect * 2, -2.0, 2.0, 1.5, 20.0);
    return projection;
  }
};

class Centrall_strategy : public Projection_strategy {
 public:
  QMatrix4x4 create(float aspect) const override {
    QMatrix4x4 projection;
    projection.frustum(-aspect, aspect, -1.0, 1.0, 1.5, 20.0);
    return projection;
  }
};
}  // namespace s21

#endif  // PROJECTION_STRATEGY_H