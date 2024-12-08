#ifndef SAVES_HPP_INCLUDED
#define SAVES_HPP_INCLUDED 1

#include "Triangle.hpp"

#include <string>

class Saves {
public:
  static void save_to_file(const std::string& filename, const Triangle& triangle, const glm::vec4& triangle_color);
  static void load_from_file(const std::string& filename, Triangle* triangle, glm::vec4* triangle_color);
};

#endif // SAVES_HPP_INCLUDED