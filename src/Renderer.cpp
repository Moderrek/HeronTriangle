#include <iostream>

#include "Renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <glm/glm.hpp>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer() : gridVAO(0), gridVBO(0), triangleVAO(0), triangleVBO(0), circleVAO(0), circleVBO(0),
                       shaderProgram(0) {}

Renderer::~Renderer() {
  GLCall(glDeleteVertexArrays(1, &gridVAO));
  GLCall(glDeleteBuffers(1, &gridVBO));
  GLCall(glDeleteVertexArrays(1, &triangleVAO));
  GLCall(glDeleteBuffers(1, &triangleVBO));
  GLCall(glDeleteProgram(shaderProgram));
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

  GLCall(unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER));
  GLCall(glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr));
  GLCall(glCompileShader(vertexShader));

  GLCall(unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
  GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr));
  GLCall(glCompileShader(fragmentShader));

  GLCall(shaderProgram = glCreateProgram());
  GLCall(glAttachShader(shaderProgram, vertexShader));
  GLCall(glAttachShader(shaderProgram, fragmentShader));
  GLCall(glLinkProgram(shaderProgram));

  GLCall(glDeleteShader(vertexShader));
  GLCall(glDeleteShader(fragmentShader));
}

void Renderer::setup_grid() {
  std::vector<float> grid_lines;

  for (int i = -20; i <= 20; i++) {
    grid_lines.push_back(i); // x1
    grid_lines.push_back(-20.0f); // y1
    grid_lines.push_back(i); // x2
    grid_lines.push_back(20.0f); // y2

    grid_lines.push_back(-20.0f); // x1
    grid_lines.push_back(i); // y1
    grid_lines.push_back(20.0f); // x2
    grid_lines.push_back(i); // y2
  }

  GLCall(glGenVertexArrays(1, &gridVAO));
  GLCall(glGenBuffers(1, &gridVBO));

  GLCall(glBindVertexArray(gridVAO));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, gridVBO));
  GLCall(glBufferData(GL_ARRAY_BUFFER, grid_lines.size() * sizeof(float), grid_lines.data(), GL_STATIC_DRAW));

  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
  GLCall(glEnableVertexAttribArray(0));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindVertexArray(0));
}

void Renderer::setup_triangle() {
  GLCall(glGenVertexArrays(1, &triangleVAO));
  GLCall(glGenBuffers(1, &triangleVBO));

  GLCall(glBindVertexArray(triangleVAO));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, triangleVBO));

  GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW));

  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
  GLCall(glEnableVertexAttribArray(0));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindVertexArray(0));
}

void Renderer::setup_circle() {
  constexpr int segments = 32;
  std::vector<glm::vec2> circle_vertices;
  for (int i = 0; i <= segments; ++i) {
    const float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
    circle_vertices.emplace_back(cos(theta), sin(theta));
  }

  GLCall(glGenVertexArrays(1, &circleVAO));
  GLCall(glGenBuffers(1, &circleVBO));

  GLCall(glBindVertexArray(circleVAO));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, circleVBO));
  GLCall(glBufferData(GL_ARRAY_BUFFER, circle_vertices.size() * sizeof(glm::vec2), circle_vertices.data(), GL_STATIC_DRAW));

  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0));
  GLCall(glEnableVertexAttribArray(0));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindVertexArray(0));
}

void Renderer::draw_grid(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const {
  GLCall(glUseProgram(shaderProgram));
  GLCall(glUniformMatrix4fv(get_uniform_location("projection"), 1, GL_FALSE, &projection[0][0]));
  GLCall(glUniformMatrix4fv(get_uniform_location("view"), 1, GL_FALSE, &view[0][0]));
  GLCall(glUniformMatrix4fv(get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(model)));


  GLCall(glBindVertexArray(gridVAO));
  GLCall(glDrawArrays(GL_LINES, 0, (20 - (-20) + 1) * 4));
  GLCall(glBindVertexArray(0));
}

void Renderer::draw_circle(const glm::vec2& position, const float radius, const glm::mat4& projection,
                           const glm::mat4& view) const {
  auto model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));
  model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

  GLCall(glUseProgram(shaderProgram));
  GLCall(glUniformMatrix4fv(get_uniform_location("projection"), 1, GL_FALSE, glm::value_ptr(projection)));
  GLCall(glUniformMatrix4fv(get_uniform_location("view"), 1, GL_FALSE, glm::value_ptr(view)));
  GLCall(glUniformMatrix4fv(get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(model)));

  GLCall(glBindVertexArray(circleVAO));
  GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 32));
  GLCall(glBindVertexArray(0));
}

void Renderer::draw_triangle(const Triangle& triangle, const glm::mat4& projection, const glm::mat4& view,
                             const glm::mat4& model) const {
  GLCall(glUseProgram(shaderProgram));

  GLCall(glUniformMatrix4fv(get_uniform_location("projection"), 1, GL_FALSE, &projection[0][0]));
  GLCall(glUniformMatrix4fv(get_uniform_location("view"), 1, GL_FALSE, &view[0][0]));
  GLCall(glUniformMatrix4fv(get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(model)));

  if (triangle.is_update_needed()) {
    update_triangle_buffer(triangle);
  }

  GLCall(glBindVertexArray(triangleVAO));
  GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
  GLCall(glBindVertexArray(0));
}

void Renderer::update_triangle_buffer(const Triangle& triangle) const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, triangleVBO));
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), triangle.get_vertices().data()));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Renderer::set_color(const glm::vec4& color) const {
  glUseProgram(shaderProgram);
  GLCall(const int color_location = get_uniform_location("u_color"));
  glUniform4f(color_location, color[0], color[1], color[2], color[3]);
}

int Renderer::get_uniform_location(const std::string& name) const {
  if (uniform_cache.contains(name)) {
    return uniform_cache[name];
  }
  GLCall(const int location = glGetUniformLocation(shaderProgram, name.c_str()));
  if (location == -1) {
    std::cerr << "WARNING: Uniform '" << name << "' not found in shader program!\n";
  }
  uniform_cache[name] = location;
  return location;
}

void Renderer::GLClearErrors() {
  while (glGetError() != GL_NO_ERROR);
}

bool Renderer::GLCheckError(const char* function, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL] ERROR: (0x" << std::hex << error << ") " << std::dec << function << " " << file << ":" << line << '\n';
    return false;
  }
  return true;
}

float distance_squared(const glm::vec2& a, const glm::vec2& b) {
  const glm::vec2 diff = a - b;
  return diff.x * diff.x + diff.y * diff.y;
}
