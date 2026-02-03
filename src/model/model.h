#ifndef S21_MODEL_H
#define S21_MODEL_H
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <QMatrix4x4>
#include <cmath>

struct Vertex
{
    double x, y, z;
};

class Model
{
    friend class GLWidget;

public:
    Model();
    ~Model() = default;
    void rend_file(const std::string &filename);

    void set_filename(const std::string &filename) { file_name_ = filename; }
    void load_model();
    std::vector<double> &get_v() { return vertices; }
    std::vector<unsigned int> &get_i() { return indices; }
    size_t get_v_count() { return vertices_count_; }
    size_t get_i_count() { return indices_count_; }

    double fast_atof(const char *&p);
    bool find_vertex(std::stringstream &ss);
    bool find_facets(std::stringstream &ss);
    void loadObj(const std::string &filename);

    void read_obj_fast(const std::string &filename,
                       std::vector<double> &vertices,
                       std::vector<unsigned int> &indices);

    QMatrix4x4 rotationMatrix(float angleX, float angleY, float angleZ) const;
    QMatrix4x4 scalingMatrix(float scale) const;
    QMatrix4x4 translationMatrix(float translateX, float translateY, float translateZ) const;

private:
    std::string file_name_;
    std::vector<double> vertices;
    std::vector<unsigned int> indices;
    size_t vertices_count_;
    size_t indices_count_;
};

#endif
