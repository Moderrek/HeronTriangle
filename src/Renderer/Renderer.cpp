#include <iostream>

#include "Renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <glm/glm.hpp>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"
#include "Camera.hpp"

namespace Heron {

  struct Renderer2DData {
    std::shared_ptr<Shader> shader;

    std::shared_ptr<VertexArray> grid_vertex_array;
    std::shared_ptr<VertexBuffer> grid_vertex_buffer;
    std::shared_ptr<VertexBufferLayout> grid_vertex_buffer_layout;

    std::shared_ptr<VertexArray> circle_vertex_array;
    std::shared_ptr<VertexBuffer> circle_vertex_buffer;
    std::shared_ptr<VertexBufferLayout> circle_vertex_buffer_layout;

    std::shared_ptr<VertexArray> triangle_vertex_array;
    std::shared_ptr<VertexBuffer> triangle_vertex_buffer;
    std::shared_ptr<VertexBufferLayout> triangle_vertex_buffer_layout;
  };

  static Renderer2DData s_data;

  void Renderer2D::Init() {
    std::cout << "INFO: [Renderer2D] Init\n";
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

    std::cout << "INFO: [Renderer2D] Compiling shaders\n";

    s_data.shader = std::make_shared<Shader>("General Shader", vertexShaderSource, fragmentShaderSource);
    std::cout << "INFO: [Renderer2D] Generating meshes\n";
    Renderer2D::GenerateGrid();
    Renderer2D::GenerateCircle(1, 32);
    Renderer2D::GenerateTriangle();
  }

  void Renderer2D::Shutdown() {
    std::cout << "INFO: [Renderer2D] Shutdown\n";
    s_data.shader->remove();

    s_data.grid_vertex_array->remove();
    s_data.triangle_vertex_array->remove();
    s_data.circle_vertex_array->remove();

    s_data.grid_vertex_buffer->remove();
    s_data.triangle_vertex_buffer->remove();
    s_data.circle_vertex_buffer->remove();
  }

  void Renderer2D::BeginScene(const Camera2D& camera) {

  }

  void Renderer2D::EndScene() {

  }

  void Renderer2D::SetColor(const glm::vec4& color) {
    s_data.shader->bind();
    s_data.shader->set_uniform_4f("u_color", color);
  }

  void Heron::Renderer2D::DrawTriangle(const Triangle &triangle, const glm::mat4 &projection, const glm::mat4 &view, const glm::mat4 &model)
  {
    s_data.shader->bind();
    s_data.shader->set_uniform_mat4f("projection", projection);
    s_data.shader->set_uniform_mat4f("view", view);
    s_data.shader->set_uniform_mat4f("model", model);

    if (triangle.is_update_needed()) {
        UpdateTriangleBuffer(triangle);
    }

    s_data.triangle_vertex_array->bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    s_data.triangle_vertex_array->unbind();
  }

  void Renderer2D::UpdateTriangleBuffer(const Triangle& triangle) {
    s_data.triangle_vertex_buffer->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * 3, triangle.get_vertices().data());
    s_data.triangle_vertex_buffer->unbind();
  }

  void Renderer2D::GenerateGrid() {
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

    s_data.grid_vertex_array = std::make_shared<VertexArray>();
    s_data.grid_vertex_buffer = std::make_shared<VertexBuffer>(grid_lines.data(), grid_lines.size() * sizeof(float));
    s_data.grid_vertex_buffer_layout = std::make_shared<VertexBufferLayout>();
    s_data.grid_vertex_buffer_layout->push<float>(2);

    s_data.grid_vertex_array->add_buffer(*s_data.grid_vertex_buffer, *s_data.grid_vertex_buffer_layout);
  }

  void Renderer2D::GenerateTriangle() {
    std::vector<glm::vec2> triangle_vertices = {
      {0.0f, 1.0f},
      {-1.0f, -1.0f},
      {1.0f, -1.0f} 
    };

    s_data.triangle_vertex_array = std::make_shared<VertexArray>();
    s_data.triangle_vertex_buffer = std::make_shared<VertexBuffer>(triangle_vertices.data(), sizeof(glm::vec2) * 3);
    auto triangle_layout = std::make_shared<VertexBufferLayout>();
    triangle_layout->push<float>(2);

    s_data.triangle_vertex_array->add_buffer(*s_data.triangle_vertex_buffer, *triangle_layout);
  }

  void Renderer2D::GenerateCircle(float radius, int segments) {
    std::vector<float> circle_vertices;
    circle_vertices.push_back(0.0f);
    circle_vertices.push_back(0.0f);

    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
        circle_vertices.push_back(cos(theta) * radius);
        circle_vertices.push_back(sin(theta) * radius);
    }

    auto circle_vao = std::make_shared<VertexArray>();
    auto circle_vbo = std::make_shared<VertexBuffer>(circle_vertices.data(), circle_vertices.size() * sizeof(float));
    auto circle_layout = std::make_shared<VertexBufferLayout>();
    circle_layout->push<float>(2);

    circle_vao->add_buffer(*circle_vbo, *circle_layout);

    s_data.circle_vertex_array = circle_vao;
    s_data.circle_vertex_buffer = circle_vbo;
  }

  void Renderer2D::DrawGrid(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) {
    s_data.shader->bind();
    s_data.shader->set_uniform_mat4f("projection", projection);
    s_data.shader->set_uniform_mat4f("view", view);
    s_data.shader->set_uniform_mat4f("model", model);

    s_data.grid_vertex_array->bind();
    glDrawArrays(GL_LINES, 0, 168);
    s_data.grid_vertex_array->unbind();
  }

  void Renderer2D::DrawCircle(const glm::vec2& position, float radius, const glm::mat4& projection, const glm::mat4& view) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f)); 
    model = glm::scale(model, glm::vec3(radius, radius, 1.0f)); 

    s_data.shader->bind();
    s_data.shader->set_uniform_mat4f("projection", projection);
    s_data.shader->set_uniform_mat4f("view", view);
    s_data.shader->set_uniform_mat4f("model", model);

    s_data.circle_vertex_array->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 34);
    s_data.circle_vertex_array->unbind();
  }

  void Renderer2D::GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
  }

  bool Renderer2D::GLCheckError(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
      std::cout << "[OpenGL] ERROR: (0x" << std::hex << error << ") " << std::dec << function << " " << file << ":" << line << '\n';
      return false;
    }
    return true;
  }
  
}

float distance_squared(const glm::vec2& a, const glm::vec2& b) {
  const glm::vec2 diff = a - b;
  return diff.x * diff.x + diff.y * diff.y;
}
