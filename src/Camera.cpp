#include "Camera.hpp"

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

static Camera* g_camera = nullptr;

Camera::Camera(const glm::vec2& window_size, const glm::vec2& position) {
  m_position = position;
  m_zoom = 1.0f;
  m_last_window_size = window_size;
  update_matrix(window_size);
  g_camera = this;
}

Camera::~Camera() {
  if (g_camera == this) g_camera = nullptr;
}

void Camera::set_matrix(const Shader& shader, const std::string& uniform_projection_name,
                        const std::string& uniform_view_name) const {
  shader.set_uniform_mat4f(uniform_projection_name, m_projection);
  shader.set_uniform_mat4f(uniform_view_name, m_view);
}

void Camera::update_matrix(const glm::vec2& window_size) {
  m_last_window_size = window_size;
  const float aspect_ratio = window_size.x / window_size.y;
  m_view = glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f));
  m_projection = glm::ortho(-10.0f * m_zoom * aspect_ratio, 10.0f * m_zoom * aspect_ratio, -10.0f * m_zoom,
                            10.0f * m_zoom, -1.0f, 1.0f);
}

void Camera::process_inputs(GLFWwindow* window, const float delta_time) {
  const float move_speed = 5.0f * m_zoom * delta_time;
  bool modified_position = false;

  // Movement XY
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    m_position.y += move_speed;
    modified_position = true;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    m_position.y -= move_speed;
    modified_position = true;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    m_position.x -= move_speed;
    modified_position = true;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    m_position.x += move_speed;
    modified_position = true;
  }
  // Zoom
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    m_zoom -= move_speed;
    modified_position = true;
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    m_zoom += move_speed;
    modified_position = true;
  }
  
  if (modified_position) update_matrix(m_last_window_size);
}


void Camera::scroll_callback(GLFWwindow* window, double x_offset, const double y_offset) {
  if (!g_camera) return;
  if (y_offset == 0) return;
  g_camera->m_zoom -= 0.1f * y_offset;
  g_camera->m_zoom = std::max(g_camera->m_zoom, 0.1f);
  g_camera->m_zoom = std::min(g_camera->m_zoom, 10.0f);
  g_camera->update_matrix(g_camera->m_last_window_size);
}

void Camera::setup_scroll(GLFWwindow* window) {
  glfwSetScrollCallback(window, scroll_callback);
}

glm::vec2 Camera::get_position() const {
  return m_position;
}

float Camera::get_zoom() const {
  return m_zoom;
}

glm::mat4 Camera::get_view() const {
  return m_view;
}

glm::mat4 Camera::get_projection() const {
  return m_projection;
}

void Camera::set_position(const glm::vec2& position) {
  m_position = position;
}

void Camera::set_zoom(const float zoom) {
  m_zoom = zoom;
}
