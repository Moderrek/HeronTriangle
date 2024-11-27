#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "triangle.h"
#include "renderer.h"
#include "style.h"

constexpr float grid_size = 1.0f;
constexpr float snap_threshold = 0.2f;


glm::vec2 snap_to_grid(const glm::vec2& position) {
  glm::vec2 snapped_position = position;

  const float grid_x = round(position.x / grid_size) * grid_size;
  const float grid_y = round(position.y / grid_size) * grid_size;

  if (fabs(position.x - grid_x) < snap_threshold) {
    snapped_position.x = grid_x;
  }

  if (fabs(position.y - grid_y) < snap_threshold) {
    snapped_position.y = grid_y;
  }

  return snapped_position;
}

int window_width = 1000, window_height = 800;

float zoom = 0.5f;

glm::vec2 camera_pos(0.0f, 0.0f);

glm::vec2 mouseWorldPos;


bool isRightMousePressed = false;
double lastMouseX, lastMouseY;

bool dragging_vertex = false;
int selected_vertex = -1;

static glm::vec2 screen_to_world(const double mouse_x, const double mouse_y) {
  const float ndc_x = 2.0f * mouse_x / window_width - 1.0f; 
  const float ndc_y = 1.0f - (2.0f * mouse_y) / window_height;

  const float world_x = (ndc_x * (10.0f * zoom)) * ((float)window_width / (float)window_height) + camera_pos.x;
  const float world_y = (ndc_y * (10.0f * zoom)) + camera_pos.y;

  return {world_x, world_y};
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
  window_width = width;
  window_height = height;
  glViewport(0, 0, window_width, window_height);
}

void scroll_callback(GLFWwindow* window, [[maybe_unused]] double x_offset, const double y_offset) {
  zoom -= y_offset * 0.1f;
  zoom = std::max(zoom, 0.1f);
  zoom = std::min(zoom, 10.0f);
}

void process_input(GLFWwindow* window) {
  const float move_speed = 0.05f * zoom;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera_pos.y += move_speed;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera_pos.y -= move_speed;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera_pos.x -= move_speed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera_pos.x += move_speed;
}

int main() {
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Wzor Herona - Tymon Wozniak 3TP", nullptr, nullptr);
  if (!window) return -1;

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) return -1;
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  setup_im_gui_style();
  setup_im_gui_fonts();

  Triangle triangle(3.0f, 4.0f, 5.0f);
  Renderer renderer;
  renderer.init();

  auto background_color = glm::vec4(0.98f, 0.98f, 0.98f, 1.0f);
  auto grid_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  auto triangle_color = glm::vec4(0.0f, 0.16f, 1.0f, 1.0f);
  auto triangle_vertex_color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
  auto triangle_vertex_selected_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    

    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    mouseWorldPos = screen_to_world(mouse_x, mouse_y);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
      double current_mouse_x, current_mouse_y;
      glfwGetCursorPos(window, &current_mouse_x, &current_mouse_y);

      if (!isRightMousePressed) {
        isRightMousePressed = true;
        lastMouseX = current_mouse_x;
        lastMouseY = current_mouse_y;
      } else {
        double delta_x = current_mouse_x - lastMouseX;
        double delta_y = current_mouse_y - lastMouseY;

        float aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
        camera_pos.x -= static_cast<float>(delta_x) * zoom * aspect_ratio / window_width * 20.0f;
        camera_pos.y += static_cast<float>(delta_y) * zoom / window_height * 20.0f;

        lastMouseX = current_mouse_x;
        lastMouseY = current_mouse_y;
      }
    } else {
      isRightMousePressed = false; 
    }

    
    const float aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
    glm::mat4 projection = glm::ortho(-10.0f * zoom * aspect_ratio, 10.0f * zoom * aspect_ratio, -10.0f * zoom,
                                      10.0f * zoom, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_pos, 0.0f));


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      double mouse_x, mouse_y;
      glfwGetCursorPos(window, &mouse_x, &mouse_y);
      glm::vec2 world_pos = screen_to_world(mouse_x, mouse_y);

      if (!dragging_vertex) {
        for (int i = 0; i < 3; ++i) {
          if (glm::distance(world_pos, triangle.get_vertices()[i]) < 0.3f) {
            dragging_vertex = true;
            selected_vertex = i;
            break;
          }
        }
      }
      else if (selected_vertex != -1) {
        triangle.move_vertex(selected_vertex, snap_to_grid(world_pos));
      }
    }
    else if (dragging_vertex) {
      dragging_vertex = false;
      selected_vertex = -1;
    }

    auto grid_model = glm::mat4(1.0f);
    renderer.set_color(grid_color);
    renderer.draw_grid(projection, view, grid_model);
    
    auto triangle_model = glm::mat4(1.0f);
    renderer.set_color(triangle_color);
    renderer.draw_triangle(triangle, projection, view, triangle_model);
    
    for (int i = 0; i < 3; ++i) {
      bool selected = dragging_vertex && i == selected_vertex;
      renderer.set_color(selected ? triangle_vertex_selected_color : triangle_vertex_color);
      renderer.draw_circle(triangle.get_vertices()[i], 0.15f, projection, view);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      ImGui::Begin("Wlasciwosci");
      auto sides = triangle.get_sides();
      ImGui::SliderFloat("Bok A", &sides[0], 0.1f, 10.0f);
      ImGui::SliderFloat("Bok B", &sides[1], 0.1f, 10.0f);
      ImGui::SliderFloat("Bok C", &sides[2], 0.1f, 10.0f);
      ImGui::Text("Pole trojkata: %.2f", triangle.get_area());
      if (ImGui::Button("Stworz trojkat")) {
        triangle = Triangle(sides[0], sides[1], sides[2]);
      }
      ImGui::End();
    }

    {
      ImGui::Begin("Kolory");
      ImGui::ColorEdit4("Tlo", glm::value_ptr(background_color));
      ImGui::ColorEdit4("Siatka", glm::value_ptr(grid_color));
      ImGui::ColorEdit4("Trojkat", glm::value_ptr(triangle_color));
      ImGui::ColorEdit4("Wierzcholek", glm::value_ptr(triangle_vertex_color));
      ImGui::ColorEdit4("Wybrany wierzcholek", glm::value_ptr(triangle_vertex_selected_color));
      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}
