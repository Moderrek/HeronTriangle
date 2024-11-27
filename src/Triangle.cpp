#include "Triangle.h"

#include "Renderer.h"

Triangle::Triangle(const float a, const float b, const float c) : a(a), b(b), c(c) {
  vertices.resize(3);
  sides.resize(3);
  update_vertices();
}

void Triangle::update_vertices() {
  if (!is_valid()) return;

  vertices[0] = {0.0f, 0.0f};
  vertices[1] = {c, 0.0f};

  const float angle = acos((a * a + c * c - b * b) / (2 * a * c));
  vertices[2] = {a * cos(angle), a * sin(angle)};
  update_sides();
}

void Triangle::move_vertex(int index, glm::vec2 pos) {
  if (index < 0 || index >= 3) return;
  vertices[index] = {pos.x, pos.y};
  update_sides();
}

bool Triangle::is_valid() const {
  return (a + b > c) && (a + c > b) && (b + c > a);
}

const std::vector<glm::vec2>& Triangle::get_vertices() const {
  return vertices;
}

const std::vector<float>& Triangle::get_sides() const {
  return sides;
}

float Triangle::get_area() const {
  return area;
}

void Triangle::update_sides() {
  sides[0] = glm::distance(vertices[1], vertices[2]);
  sides[1] = glm::distance(vertices[0], vertices[2]);
  sides[2] = glm::distance(vertices[0], vertices[1]);
  update_area();
}

void Triangle::update_area() {
  const float circumference = sides[0] + sides[1] + sides[2];
  const float s = circumference / 2;
  area = sqrt(s * (s - sides[0]) * (s - sides[1]) * (s - sides[2]));
}
