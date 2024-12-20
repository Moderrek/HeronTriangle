﻿#pragma once

#include "platform.hpp"

#include <string>

#include "Triangle.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

#ifdef HERON_PLATFORM_WINDOWS

#define ASSERT(x) if (!(x)) __debugbreak()

#else

#define ASSERT(x) 

#endif

#define GLCall(x) Renderer::GLClearErrors();\
x;\
ASSERT(Renderer::GLCheckError(#x, __FILE__, __LINE__))\


class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void init();
  void draw_grid(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const;
  void draw_circle(const glm::vec2& position, float radius, const glm::mat4& projection, const glm::mat4& view) const;
  void draw_triangle(const Triangle& triangle, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const;
  
  void update_triangle_buffer(const Triangle& triangle) const;
  
  void set_color(const glm::vec4& color) const;
  
  int get_uniform_location(const std::string& name) const;
  
  static void GLClearErrors();
  static bool GLCheckError(const char* function, const char* file, int line);
  
private:
  mutable std::unordered_map<std::string, int> uniform_cache;

  unsigned int gridVAO, gridVBO;
  unsigned int triangleVAO, triangleVBO;
  unsigned int circleVAO, circleVBO;
  
  unsigned int shaderProgram;
  
  mutable glm::vec4 last_color;
  
  void setup_grid();
  void setup_triangle();
  void setup_circle();
  void load_shaders();
};
