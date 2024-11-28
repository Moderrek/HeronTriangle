#pragma once

#include <glm/glm.hpp>

class Triangle {
public:
  Triangle(float a, float b, float c);
  
  void update_vertices();
  void move_vertex(int index, glm::vec2 pos);
  
  [[nodiscard]] bool is_valid() const;
  
  void set_vertices(const std::vector<glm::vec2>& vertices);
  
  [[nodiscard]] const std::vector<glm::vec2>& get_vertices() const;
  [[nodiscard]] const std::vector<float>& get_sides() const;
  [[nodiscard]] float get_area() const;
  [[nodisacrd]] bool is_update_needed() const;
  void reset_update_flag();
  
private:
  float a, b, c;
  float area = 0;
  std::vector<glm::vec2> vertices;
  std::vector<float> sides;
  
  bool needs_update;
  
  void update_sides();  
  void update_area();
};
