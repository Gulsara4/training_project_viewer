#include <gtest/gtest.h>

#include <cmath>

#include "../model/model.h"

// Масштабирование
TEST(ModelAffineTest, ScalingMatrix) {
  Model m;
  QMatrix4x4 mat = m.scalingMatrix(2.0);
  EXPECT_NEAR(mat(0, 0), 2.0, 1e-6);
  EXPECT_NEAR(mat(1, 1), 2.0, 1e-6);
  EXPECT_NEAR(mat(2, 2), 2.0, 1e-6);
}

// Поворот вокруг X
TEST(ModelAffineTest, RotationMatrixX) {
  Model m;
  double angle = M_PI / 2;  // 90 градусов
  QMatrix4x4 mat = m.rotationMatrix(angle, 0, 0);

  EXPECT_NEAR(mat(1, 1), std::cos(angle), 1e-6);
  EXPECT_NEAR(mat(1, 2), -std::sin(angle), 1e-6);
  EXPECT_NEAR(mat(2, 1), std::sin(angle), 1e-6);
  EXPECT_NEAR(mat(2, 2), std::cos(angle), 1e-6);
}

// Поворот вокруг Y
TEST(ModelAffineTest, RotationMatrixY) {
  Model m;
  double angle = M_PI;
  QMatrix4x4 mat = m.rotationMatrix(0, angle, 0);

  EXPECT_NEAR(mat(0, 0), std::cos(angle), 1e-6);
  EXPECT_NEAR(mat(0, 2), std::sin(angle), 1e-6);
  EXPECT_NEAR(mat(2, 0), -std::sin(angle), 1e-6);
  EXPECT_NEAR(mat(2, 2), std::cos(angle), 1e-6);
}

// Поворот вокруг Z
TEST(ModelAffineTest, RotationMatrixZ) {
  Model m;
  double angle = M_PI / 4;  // 45 градусов
  QMatrix4x4 mat = m.rotationMatrix(0, 0, angle);

  EXPECT_NEAR(mat(0, 0), std::cos(angle), 1e-6);
  EXPECT_NEAR(mat(0, 1), -std::sin(angle), 1e-6);
  EXPECT_NEAR(mat(1, 0), std::sin(angle), 1e-6);
  EXPECT_NEAR(mat(1, 1), std::cos(angle), 1e-6);
}

// Перенос
TEST(ModelAffineTest, TranslationMatrix) {
  Model m;
  QMatrix4x4 mat = m.translationMatrix(1.0, 2.0, 3.0);
  EXPECT_NEAR(mat(0, 3), 1.0, 1e-6);
  EXPECT_NEAR(mat(1, 3), 2.0, 1e-6);
  EXPECT_NEAR(mat(2, 3), 3.0, 1e-6);
}

// Комбинированное преобразование
TEST(ModelAffineTest, CombinedTransformation) {
  Model m;
  QMatrix4x4 result = m.translationMatrix(1, 2, 3) * m.scalingMatrix(2.0);
  EXPECT_NEAR(result(0, 0), 2.0, 1e-6);
  EXPECT_NEAR(result(1, 1), 2.0, 1e-6);
  EXPECT_NEAR(result(2, 2), 2.0, 1e-6);
  EXPECT_NEAR(result(0, 3), 1.0, 1e-6);
}
