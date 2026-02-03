#include "model.h"

#include <sstream>
#include <stdexcept>
// Быстрый float-парсер

double Model::fast_atof(const char *&p) {
  while (*p == ' ' || *p == '\t') ++p;
  bool neg = false;
  if (*p == '-') {
    neg = true;
    ++p;
  }

  double value = 0.0;
  while (*p >= '0' && *p <= '9') {
    value = value * 10.0 + (*p - '0');
    ++p;
  }

  if (*p == '.') {
    ++p;
    double frac = 1.0;
    while (*p >= '0' && *p <= '9') {
      frac *= 0.1;
      value += (*p - '0') * frac;
      ++p;
    }
  }

  return neg ? -value : value;
}

void Model::read_obj_fast(const std::string &filename,
                          std::vector<double> &vertices,
                          std::vector<unsigned int> &indices) {
  if (filename.find(".obj") == std::string::npos)
    throw std::runtime_error("Invalid format file! Need .obj!");

  // открытие файла и перемещение курсора сразу в конец (std::ios::ate)
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file.is_open())

    throw std::runtime_error("cannot open file: " + filename);

  // читаем всё в память
  std::streamsize size = file.tellg();  // получаем размер файла, курсор в конце
  file.seekg(0, std::ios::beg);  // возвращаем курсор в начало файла
  std::vector<char> buffer(size + 1);  // создаем буффер
  file.read(buffer.data(), size);  // читаем весь файл в буффер
  buffer[size] = '\0';
  file.close();

  const char *ptr = buffer.data();
  const char *end = ptr + size;

  std::vector<Vertex> tmpVerts;
  tmpVerts.reserve(size / 40);

  std::vector<unsigned int> tmpIndices;
  tmpIndices.reserve(size / 10);

  std::vector<std::vector<unsigned int>> faces;
  faces.reserve(size / 100);

  double minX = 1e300, minY = 1e300, minZ = 1e300;
  double maxX = -1e300, maxY = -1e300, maxZ = -1e300;

  while (ptr < end) {  // пропускаем пробельные символы
    while (ptr < end &&
           (*ptr == ' ' || *ptr == '\n' || *ptr == '\r' || *ptr == '\t'))
      ++ptr;
    if (ptr >= end) break;

    if (*ptr == 'v' && ptr[1] == ' ') {
      ptr += 2;
      double x = fast_atof(ptr);
      double y = fast_atof(ptr);
      double z = fast_atof(ptr);
      tmpVerts.push_back({x, y, z});
      vertices_count_++;
      minX = std::min(minX, x);
      maxX = std::max(maxX, x);
      minY = std::min(minY, y);
      maxY = std::max(maxY, y);
      minZ = std::min(minZ, z);
      maxZ = std::max(maxZ, z);
    } else if (*ptr == 'f' && ptr[1] == ' ') {
      ptr += 2;
      std::vector<unsigned int> face;

      while (ptr < end && *ptr != '\n' && *ptr != '\r') {
        while (*ptr == ' ' || *ptr == '\t') ++ptr;
        if (ptr >= end || *ptr == '\n' || *ptr == '\r') break;

        bool neg = false;
        if (*ptr == '-') {
          neg = true;
          ++ptr;
        }

        unsigned int value = 0;
        while (*ptr >= '0' && *ptr <= '9') {
          value = value * 10 + (*ptr - '0');
          ++ptr;
        }

        if (value > 0) {
          unsigned int idx =
              neg ? static_cast<unsigned int>(tmpVerts.size() - value)
                  : value - 1;
          if (idx < tmpVerts.size()) face.push_back(idx);
        }

        // пропускаем остаток токена
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' &&
               *ptr != '\r')
          ++ptr;
      }
      if (face.size() > 1) {
        faces.push_back(std::move(face));
      }
    }

    while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
    while (ptr < end && (*ptr == '\n' || *ptr == '\r')) ++ptr;
  }

  if (tmpVerts.empty()) return;

  // нормализация
  double centerX = (minX + maxX) * 0.5;
  double centerY = (minY + maxY) * 0.5;
  double centerZ = (minZ + maxZ) * 0.5;
  double maxSize = std::max({maxX - minX, maxY - minY, maxZ - minZ});
  double scale = (maxSize > 0.0) ? 2.0 / maxSize : 1.0;

  vertices.resize(tmpVerts.size() * 3);
  for (size_t i = 0; i < tmpVerts.size(); ++i) {
    vertices[i * 3 + 0] = (tmpVerts[i].x - centerX) * scale;
    vertices[i * 3 + 1] = (tmpVerts[i].y - centerY) * scale;
    vertices[i * 3 + 2] = (tmpVerts[i].z - centerZ) * scale;
  }

  indices.clear();
  indices.reserve(faces.size() * 6);
  for (const auto &f : faces) {
    for (size_t j = 0; j < f.size(); ++j) {
      indices.push_back(f[j]);
      indices.push_back(f[(j + 1) % f.size()]);
    }
  }
}

void Model::rend_file(const std::string &filename) {
  read_obj_fast(filename, vertices, indices);
}

Model::Model() {
  vertices = {};
  indices = {};
  vertices_count_ = 0;
  indices_count_ = 0;
  file_name_ = "No file";
}
void Model::load_model() { read_obj_fast(file_name_, vertices, indices); }

QMatrix4x4 Model::rotationMatrix(float angleX, float angleY,
                                 float angleZ) const {
  QMatrix4x4 rx, ry, rz;
  rx.setToIdentity();
  ry.setToIdentity();
  rz.setToIdentity();
  rx(1, 1) = cos(angleX);
  rx(1, 2) = -sin(angleX);
  rx(2, 1) = sin(angleX);
  rx(2, 2) = cos(angleX);
  ry(0, 0) = cos(angleY);
  ry(0, 2) = sin(angleY);
  ry(2, 0) = -sin(angleY);
  ry(2, 2) = cos(angleY);
  rz(0, 0) = cos(angleZ);
  rz(0, 1) = -sin(angleZ);
  rz(1, 0) = sin(angleZ);
  rz(1, 1) = cos(angleZ);
  return rz * ry * rx;
}

QMatrix4x4 Model::scalingMatrix(float scale) const {
  QMatrix4x4 m;
  m.setToIdentity();
  m(0, 0) = scale;
  m(1, 1) = scale;
  m(2, 2) = scale;
  return m;
}

QMatrix4x4 Model::translationMatrix(float translateX, float translateY,
                                    float translateZ) const {
  QMatrix4x4 m;
  m.setToIdentity();
  m(0, 3) = translateX;
  m(1, 3) = translateY;
  m(2, 3) = translateZ;
  return m;
}
