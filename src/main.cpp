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
#include "scoped_timer.h"
#include "style.h"
#include <chrono>
#include <thread>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "heron.h"
#include "saves.h"
#include "stb_image_write.h"

void save_framebuffer_to_image(const char* filename, int width, int height) {
  std::vector<unsigned char> pixels(width * height * 3);

  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

  std::vector<unsigned char> flippedPixels(width * height * 3);
  for (int y = 0; y < height; ++y) {
    memcpy(&flippedPixels[y * width * 3],
           &pixels[(height - 1 - y) * width * 3],
           width * 3);
  }

  stbi_write_png(filename, width, height, 3, flippedPixels.data(), width * 3);
}

const int TARGET_FPS = 60;
const int FRAME_TIME = 1000 / TARGET_FPS;

constexpr float grid_size = 1.0f;
constexpr float snap_threshold = 0.2f;

float distance_squared(const glm::vec2& a, const glm::vec2& b) {
  const glm::vec2 diff = a - b;
  return diff.x * diff.x + diff.y * diff.y;
}

int max_fps = 60;
bool unlock_fps = true;

void limit_fps(int target_fps, bool unlock) {
  static auto last_frame_time = std::chrono::high_resolution_clock::now();

  if (!unlock) {
    int frame_time_ms = 1000 / target_fps;
    auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_frame_time).count();

    if (elapsed_time < frame_time_ms) {
      std::this_thread::sleep_for(std::chrono::milliseconds(frame_time_ms - elapsed_time));
    }

    last_frame_time = std::chrono::high_resolution_clock::now();
  } else {
    last_frame_time = std::chrono::high_resolution_clock::now(); // Reset bez limitu
  }
}



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
  const double ndc_x = 2.0f * mouse_x / window_width - 1.0f;
  const double ndc_y = 1.0f - (2.0f * mouse_y) / window_height;

  const double world_x = ndc_x * (10.0f * zoom) * (static_cast<float>(window_width) / static_cast<float>(window_height))
    + camera_pos.x;
  const double world_y = ndc_y * (10.0f * zoom) + camera_pos.y;

  return {
    world_x,
    world_y
  };
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

  GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Wzor Herona - Tymon Wozniak 3TP", nullptr,
                                        nullptr);
  if (!window) return -1;

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) return -1;
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSwapInterval(0);

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

  auto grid_model = glm::mat4(1.0f);
  auto triangle_model = glm::mat4(1.0f);

  double previous_time = 0;
  double current_time = 0;
  double timer = 0;
  double previous_time_delta = 0;
  
  bool want_vsync = true;
  bool is_vsync = false;
  
  HeronSteps steps = {3.0f, 4.0f, 5.0f};
  steps.calculate();
  
  while (!glfwWindowShouldClose(window)) {
    current_time = glfwGetTime();
    timer = current_time - previous_time;
    float delta_time = static_cast<float>(current_time) - static_cast<float>(previous_time_delta);
    previous_time_delta = current_time;
    
    process_input(window);

    glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    glm::vec2 mouse_position = {mouse_x, mouse_y};
    mouseWorldPos = screen_to_world(mouse_x, mouse_y);    

    const float aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
    glm::mat4 projection = glm::ortho(-10.0f * zoom * aspect_ratio, 10.0f * zoom * aspect_ratio, -10.0f * zoom,
                                      10.0f * zoom, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_pos, 0.0f));


    if (want_vsync && !is_vsync) {
      glfwSwapInterval(1);
    } else if (!want_vsync && is_vsync) {
      glfwSwapInterval(0);
    }

    {
      if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {

        if (!isRightMousePressed) {
          isRightMousePressed = true;
          lastMouseX = mouse_position.x;
          lastMouseY = mouse_position.y;
        }
        else {
          double delta_x = mouse_position.x - lastMouseX;
          double delta_y = mouse_position.y - lastMouseY;

          camera_pos.x -= static_cast<float>(delta_x) * zoom * aspect_ratio / static_cast<float>(window_width) * 20.0f;
          camera_pos.y += static_cast<float>(delta_y) * zoom / static_cast<float>(window_height) * 20.0f;

          lastMouseX = mouse_position.x;
          lastMouseY = mouse_position.y;
        }
      }
      else {
        isRightMousePressed = false;
      }
    }


    
    {
      if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!dragging_vertex) {
          for (int i = 0; i < 3; ++i) {
            if (distance_squared(mouseWorldPos, triangle.get_vertices()[i]) < 0.09f) {
              dragging_vertex = true;
              selected_vertex = i;
              break;
            }
          }
        } else if (selected_vertex != -1) {
          if (glm::vec2 snapped_pos = snap_to_grid(mouseWorldPos); snapped_pos != triangle.get_vertices()[selected_vertex]) {
            triangle.move_vertex(selected_vertex, snapped_pos);
          }
        }
      }
      else if (dragging_vertex) {
        dragging_vertex = false;
        selected_vertex = -1;
      }
    }

    renderer.set_color(grid_color);
    renderer.draw_grid(projection, view, grid_model);

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

    bool saveScreenshot = false;
    bool saveScene = false;
    bool loadScene = false;
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("Plik")) {
        if (ImGui::MenuItem("Zapisz zrzut ekranu", "Ctrl+S")) {
          saveScreenshot = true;
        }
        if (ImGui::MenuItem("Zapisz scene", "Ctrl+Shift+S")) {
          saveScene = true;
        }
        if (ImGui::MenuItem("Wczytaj scene", "Ctrl+O")) {
          loadScene = true;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
    
    if (saveScreenshot) {
      save_framebuffer_to_image("screenshot.png", window_width, window_height);
      saveScreenshot = false;
      std::cout << "INFO: Saved screenshot as screenshot.png\n";
    }
    
    if (saveScene) {
      save_scene_to_file("scene.json", triangle, triangle_color);
      saveScene = false;
      std::cout << "INFO: Saved scene to scene.json\n";
    }

    if (loadScene) {
      load_scene_from_file("scene.json", triangle, triangle_color);
      loadScene = false;
      std::cout << "INFO: Loaded scene from scene.json\n" ;
    }

    auto sides = triangle.get_sides();

    {
      ImGui::Begin("Wlasciwosci");
      ImGui::InputFloat("Bok A", &sides[0], 0.1f, 1.0f, "%.2f");
      ImGui::InputFloat("Bok B", &sides[1], 0.1f, 1.0f, "%.2f");
      ImGui::InputFloat("Bok C", &sides[2], 0.1f, 1.0f, "%.2f");
      ImGui::Separator();
      ImGui::Text("Pole trojkata: %.2f", triangle.get_area());
      ImGui::Separator();
      for (auto vertex : triangle.get_vertices()) {
        ImGui::Text("(%.2f %.2f)", vertex.x, vertex.y);
      }
      ImGui::End();
    }
 
    render_heron_steps_panel(steps);

    {
      ImGui::Begin("Kolory");
      ImGui::ColorEdit4("Tlo", glm::value_ptr(background_color));
      ImGui::ColorEdit4("Siatka", glm::value_ptr(grid_color));
      ImGui::ColorEdit4("Trojkat", glm::value_ptr(triangle_color));
      ImGui::ColorEdit4("Wierzcholek", glm::value_ptr(triangle_vertex_color));
      ImGui::ColorEdit4("Wybrany wierzcholek", glm::value_ptr(triangle_vertex_selected_color));
      ImGui::End();
    }
    
    {
      ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_AlwaysAutoResize);
      
      ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
      ImGui::Text("Frame Time: %.2f", delta_time);
      
      ImGui::Separator();
      
      ImGui::Text("Drivers: %s", glGetString(GL_VERSION));
      ImGui::Text("Shader: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
      
      ImGui::Separator();
      
      ImGui::Checkbox("VSync", &is_vsync);
      ImGui::Checkbox("Unlock FPS", &unlock_fps);
      if (!unlock_fps) {
        ImGui::SliderInt("Max FPS", &max_fps, 15, 240);
        ImGui::Text("Target FPS: %d", max_fps);
      } else {
        ImGui::Text("Target FPS: Unlimited");
      }
      
      ImGui::End();
    }

    {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    glfwSwapBuffers(window);
    glfwPollEvents();     
    
    if (!unlock_fps) limit_fps(max_fps, unlock_fps);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}
