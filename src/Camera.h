#pragma once
#include <imgui_impl_glfw.h>

#include "Shader.h"
#include "glm/vec2.hpp"

class Camera {
public:
  Camera(const glm::vec2& window_size, const glm::vec2& position);
  ~Camera();
  
  void set_matrix(const Shader& shader, const std::string& uniform_projection_name, const std::string& uniform_view_name) const;
  
  void update_matrix(const glm::vec2& window_size);
  void process_inputs(GLFWwindow* window, float delta_time);

  static void setup_scroll(GLFWwindow* window);
  static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
  
  [[nodiscard]] glm::vec2 get_position() const;
  [[nodiscard]] float get_zoom() const;
  [[nodiscard]] glm::mat4 get_view() const;
  [[nodiscard]] glm::mat4 get_projection() const;
  
  void set_position(const glm::vec2& position);
  void set_zoom(float zoom);
  
private:
  glm::vec2 m_position;
  float m_zoom;
  glm::mat4 m_projection;
  glm::mat4 m_view;
  glm::vec2 m_last_window_size;
    
};
