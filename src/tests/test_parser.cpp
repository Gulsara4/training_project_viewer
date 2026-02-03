#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <vector>

#include "../model/model.h"

class ModelParserTest : public ::testing::Test {
 protected:
  void SetUp() override { model = new Model(); }

  void TearDown() override { delete model; }

  Model *model;
  std::vector<double> vertices;
  std::vector<unsigned int> indices;
};

// Тест на парсинг простого файла с одним треугольником
TEST_F(ModelParserTest, ParsesSimpleTriangle) {
  // Создаем временный .obj файл
  std::string test_obj = R"(v 1.0 2.0 3.0
v 4.0 5.0 6.0
v 7.0 8.0 9.0
f 1 2 3)";

  std::ofstream file("test_triangle.obj");
  file << test_obj;
  file.close();

  model->read_obj_fast("test_triangle.obj", vertices, indices);

  EXPECT_EQ(vertices.size(), 9);  // 3 вершины × 3 координаты

  EXPECT_EQ(indices.size(), 6);

  // Очистка
  std::remove("test_triangle.obj");
}

// Тест на парсинг файла с квадратом
TEST_F(ModelParserTest, ParsesSquare) {
  std::string test_obj = R"(v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 1.0 1.0 0.0
v 0.0 1.0 0.0
f 1 2 3 4)";

  std::ofstream file("test_square.obj");
  file << test_obj;
  file.close();

  model->read_obj_fast("test_square.obj", vertices, indices);

  // 4 вершины
  EXPECT_EQ(vertices.size(), 12);

  // Квадрат: 4 стороны × 2 индекса = 8 индексов
  EXPECT_EQ(indices.size(), 8);

  std::remove("test_square.obj");
}

// Тест на обработку отрицательных индексов в faces
TEST_F(ModelParserTest, HandlesNegativeIndices) {
  std::string test_obj = R"(v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 1.0 1.0 0.0
f -3 -2 -1)";

  std::ofstream file("test_negative.obj");
  file << test_obj;
  file.close();

  model->read_obj_fast("test_negative.obj", vertices, indices);

  // Должен создать корректные индексы
  EXPECT_FALSE(indices.empty());

  std::remove("test_negative.obj");
}

// Тест на обработку файла с пробелами и табуляциями
TEST_F(ModelParserTest, HandlesWhitespace) {
  std::string test_obj = R"(v    1.0   2.0   3.0  
    v   4.0    5.0    6.0
v 7.0 8.0 9.0
f   1   2 3)";

  std::ofstream file("test_whitespace.obj");
  file << test_obj;
  file.close();

  model->read_obj_fast("test_whitespace.obj", vertices, indices);

  EXPECT_EQ(vertices.size(), 9);
  EXPECT_EQ(indices.size(), 6);

  std::remove("test_whitespace.obj");
}

// Тест на обработку пустого файла
TEST_F(ModelParserTest, HandlesEmptyFile) {
  std::ofstream file("test_empty.obj");
  file.close();

  model->read_obj_fast("test_empty.obj", vertices, indices);

  EXPECT_TRUE(vertices.empty());
  EXPECT_TRUE(indices.empty());

  std::remove("test_empty.obj");
}

// Тест на обработку файла только с вершинами
TEST_F(ModelParserTest, HandlesVerticesOnly) {
  std::string test_obj = R"(v 1.0 2.0 3.0
v 4.0 5.0 6.0
v 7.0 8.0 9.0)";

  std::ofstream file("test_vertices_only.obj");
  file << test_obj;
  file.close();

  model->read_obj_fast("test_vertices_only.obj", vertices, indices);

  EXPECT_EQ(vertices.size(), 9);
  EXPECT_TRUE(indices.empty());

  std::remove("test_vertices_only.obj");
}

// Тест на нормализацию координат
TEST_F(ModelParserTest, NormalizesCoordinates) {
  std::string test_obj = R"(v -1.0 -1.0 -1.0
v 1.0 1.0 1.0
f 1 2)";

  std::ofstream file("test_normalization.obj");
  file << test_obj;
  file.close();

  model->read_obj_fast("test_normalization.obj", vertices, indices);

  // После нормализации координаты должны быть в диапазоне [-1, 1]
  for (size_t i = 0; i < vertices.size(); ++i) {
    EXPECT_GE(vertices[i], -1.0);
    EXPECT_LE(vertices[i], 1.0);
  }

  std::remove("test_normalization.obj");
}

// Тест на обработку некорректного формата файла
TEST_F(ModelParserTest, ThrowsOnInvalidFormat) {
  EXPECT_THROW(
      { model->read_obj_fast("test.txt", vertices, indices); },
      std::runtime_error);
}

// Тест на обработку несуществующего файла
TEST_F(ModelParserTest, ThrowsOnNonExistentFile) {
  EXPECT_THROW(
      { model->read_obj_fast("nonexistent.obj", vertices, indices); },
      std::runtime_error);
}

// Тест на парсинг чисел с плавающей точкой
TEST_F(ModelParserTest, ParsesFloatNumbers) {
  std::string test_obj = R"(v 1.5 -2.75 0.123
v -0.5 3.1415 -2.718
f 1 2)";

  std::ofstream file("test_floats.obj");
  file << test_obj;
  file.close();

  model->read_obj_fast("test_floats.obj", vertices, indices);

  EXPECT_EQ(vertices.size(), 6);
  EXPECT_EQ(indices.size(), 4);

  std::remove("test_floats.obj");
}

TEST_F(ModelParserTest, IgnoresComments) {
  std::string test_obj = R"(# This is a comment
v 1.0 2.0 3.0
# Another comment
v 4.0 5.0 6.0
f 1 2)";

  std::ofstream file("test_comments.obj");
  file << test_obj;
  file.close();

  model->read_obj_fast("test_comments.obj", vertices, indices);

  EXPECT_EQ(vertices.size(), 6);
  EXPECT_EQ(indices.size(), 4);

  std::remove("test_comments.obj");
}
