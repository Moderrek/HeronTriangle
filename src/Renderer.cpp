#include <iostream>

#include "renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <glm\glm.hpp>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer() : gridVAO(0), gridVBO(0), triangleVAO(0), triangleVBO(0), circleVAO(0), circleVBO(0),
                       shaderProgram(0) {}

Renderer::~Renderer() {
  glDeleteVertexArrays(1, &gridVAO);
  glDeleteBuffers(1, &gridVBO);
  glDeleteVertexArrays(1, &triangleVAO);
  glDeleteBuffers(1, &triangleVBO);
  glDeleteProgram(shaderProgram);
}

void Renderer::init() {
  load_shaders();
  setup_grid();
  setup_triangle();
  setup_circle();
}

void Renderer::load_shaders() {
  const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        uniform mat4 projection;
        uniform mat4 view;
        uniform mat4 model;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
        }
    )";

  const char* fragmentShaderSource = R"(
        #version 330 core
        
        uniform vec4 u_color;
        
        out vec4 FragColor;
        
        void main() {
            FragColor = u_color;
        }
    )";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Renderer::setup_grid() {
  std::vector<float> grid_lines;

  for (float i = -20.0f; i <= 20.0f; i++) {
    grid_lines.push_back(i); // x1
    grid_lines.push_back(-20.0f); // y1
    grid_lines.push_back(i); // x2
    grid_lines.push_back(20.0f); // y2

    grid_lines.push_back(-20.0f); // x1
    grid_lines.push_back(i); // y1
    grid_lines.push_back(20.0f); // x2
    grid_lines.push_back(i); // y2
  }

  glGenVertexArrays(1, &gridVAO);
  glGenBuffers(1, &gridVBO);

  glBindVertexArray(gridVAO);
  glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
  glBufferData(GL_ARRAY_BUFFER, grid_lines.size() * sizeof(float), grid_lines.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::setup_triangle() {
  glGenVertexArrays(1, &triangleVAO);
  glGenBuffers(1, &triangleVBO);

  glBindVertexArray(triangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::setup_circle() {
  constexpr int segments = 32;
  std::vector<glm::vec2> circle_vertices;
  for (int i = 0; i <= segments; ++i) {
    const float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
    circle_vertices.emplace_back(cos(theta), sin(theta));
  }

  glGenVertexArrays(1, &circleVAO);
  glGenBuffers(1, &circleVBO);

  glBindVertexArray(circleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
  glBufferData(GL_ARRAY_BUFFER, circle_vertices.size() * sizeof(glm::vec2), circle_vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::draw_grid(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) {
  glUseProgram(shaderProgram);
  glUniformMatrix4fv(get_uniform_location("projection"), 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(get_uniform_location("view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(model));


  glBindVertexArray(gridVAO);
  glDrawArrays(GL_LINES, 0, (20 - (-20) + 1) * 4);
  glBindVertexArray(0);
}

void Renderer::draw_circle(const glm::vec2& position, const float radius, const glm::mat4& projection,
                           const glm::mat4& view) const {
  auto model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));
  model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

  glUseProgram(shaderProgram);
  glUniformMatrix4fv(get_uniform_location("projection"), 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(get_uniform_location("view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(model));

  glBindVertexArray(circleVAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 32);
  glBindVertexArray(0);
}

void Renderer::draw_triangle(const Triangle& triangle, const glm::mat4& projection, const glm::mat4& view,
                             const glm::mat4& model) {
  glUseProgram(shaderProgram);

  glUniformMatrix4fv(get_uniform_location("projection"), 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(get_uniform_location("view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(model));

  if (triangle.is_update_needed()) {
    update_triangle_buffer(triangle);
  }

  glBindVertexArray(triangleVAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}

void Renderer::update_triangle_buffer(const Triangle& triangle) const {
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), triangle.get_vertices().data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::set_color(const glm::vec4& color) const {
  glUseProgram(shaderProgram);
  const int color_location = get_uniform_location("u_color");
  glUniform4f(color_location, color[0], color[1], color[2], color[3]);
}

int Renderer::get_uniform_location(const std::string& name) const {
  if (uniform_cache.contains(name)) {
    return uniform_cache[name];
  }
  const int location = glGetUniformLocation(shaderProgram, name.c_str());
  if (location == -1) {
    std::cerr << "WARNING: Uniform '" << name << "' not found in shader program!\n";
  }
  uniform_cache[name] = location;
  return location;
}
