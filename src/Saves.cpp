#include "Saves.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

void Saves::save_to_file(const std::string& filename, const Triangle& triangle, const glm::vec4& triangle_color) {
  nlohmann::json scene;

  scene["triangle"]["vertices"] = {
    {triangle.get_vertices()[0].x, triangle.get_vertices()[0].y},
    {triangle.get_vertices()[1].x, triangle.get_vertices()[1].y},
    {triangle.get_vertices()[2].x, triangle.get_vertices()[2].y}
  };

  scene["triangle"]["color"] = {triangle_color.r, triangle_color.g, triangle_color.b, triangle_color.a};

  std::ofstream file(filename);
  file << scene.dump(2);
}



void Saves::load_from_file(const std::string& filename, Triangle* triangle, glm::vec4* triangle_color) {
  std::ifstream file(filename);
  nlohmann::json scene;
  file >> scene;

  // Load triangle verticies position
  auto vertices = scene["triangle"]["vertices"];
  triangle->set_vertices({
      glm::vec2(vertices[0][0], vertices[0][1]),
      glm::vec2(vertices[1][0], vertices[1][1]),
      glm::vec2(vertices[2][0], vertices[2][1])
  });

  // Load triangle color
  auto color = scene["triangle"]["color"];
  *triangle_color = glm::vec4(color[0], color[1], color[2], color[3]);
}