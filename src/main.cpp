#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Triangle.hpp"
#include "Renderer.hpp"
#include "ScopedTimer.hpp"
#include "ImGuiStyle.hpp"
#include <chrono>
#include <thread>

#include "Camera.hpp"
#include "HeronPanel.hpp"
#include "Saves.hpp"
#include "Screenshot.hpp"

const int TARGET_FPS = 60;
const int FRAME_TIME = 1000 / TARGET_FPS;

constexpr float grid_size = 1.0f;
constexpr float snap_threshold = 0.2f;

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
  }
  else {
    last_frame_time = std::chrono::high_resolution_clock::now();
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

bool isRightMousePressed = false;
double lastMouseX, lastMouseY;

bool dragging_vertex = false;
int selected_vertex = -1;

static glm::vec2 screen_to_world(const Camera& cam, const glm::vec2& window_size, const glm::vec2& mouse_pos) {
  const glm::vec2& cam_pos = cam.get_position();
  const float zoom = cam.get_zoom();

  const double ndc_x = 2.0f * mouse_pos.x / window_size.x - 1.0f;
  const double ndc_y = 1.0f - (2.0f * mouse_pos.y) / window_size.y;

  const double world_x = ndc_x * (10.0f * zoom) * (static_cast<float>(window_size.x) / static_cast<float>(window_size.
      y))
    + cam_pos.x;
  const double world_y = ndc_y * (10.0f * zoom) + cam_pos.y;

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

int main() {
  std::cout << "HeronTriangle v1.0.1 created by Tymon Wozniak (https://github.com/Moderrek)\nRunning on " 
    << HERON_PLATFORM_NAME << '-' << HERON_MODE << '\n';

  if (!glfwInit()) {
    std::cerr << "FATAL: Failed to initialize GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  std::cout << "INFO: Initialized GLFW\n";

  GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Heron Triangle Visualizer", nullptr,
                                        nullptr);
  if (!window) {
    std::cerr << "FATAL: Failed to open GLFW window.\n";
    return -1;
  }

  std::cout << "INFO: Created GLFW Window\n";

  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "FATAL: Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
    return -1;
  }
  std::cout << "INFO: Initialized GLEW and OpenGL\n";

  std::cout << "Drivers: OpenGL " << glGetString(GL_VERSION) << '\n';
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSwapInterval(0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  setup_im_gui_style();
  setup_im_gui_fonts();
  std::cout << "INFO: Created ImGui Context\n";

  {
    Triangle triangle(3.0f, 4.0f, 5.0f);
    Renderer renderer;
    renderer.init();
    std::cout << "INFO: Initialized renderer\n";

    glm::vec2 window_size = {window_width, window_height};
    Camera camera{window_size, {0, 0}};
    Camera::setup_scroll(window);

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

    std::cout << "INFO: Starting game loop\n";
    while (!glfwWindowShouldClose(window)) {
      // Calculate delta time
      current_time = glfwGetTime();
      timer = current_time - previous_time;
      double delta_time = current_time - previous_time_delta;
      previous_time_delta = current_time;

  
      camera.process_inputs(window, delta_time);

      // Calculate mouse position
      double mouse_x, mouse_y;
      glfwGetCursorPos(window, &mouse_x, &mouse_y);
      glm::vec2 mouse_pos = {mouse_x, mouse_y};
      glm::vec2 mouse_world_pos = screen_to_world(camera, window_size, mouse_pos);

      const float aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);

      // VSync toggler
      if (want_vsync && !is_vsync) {
        std::cout << "INFO: Enabled V-Sync\n";
        glfwSwapInterval(1);
        is_vsync = true;
      }
      if (!want_vsync && is_vsync) {
        std::cout << "INFO: Disabled V-Sync\n";
        glfwSwapInterval(0);
        is_vsync = false;
      }

      {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
          if (!isRightMousePressed) {
            isRightMousePressed = true;
            lastMouseX = mouse_pos.x;
            lastMouseY = mouse_pos.y;
          }
          else {
            double delta_x = mouse_pos.x - lastMouseX;
            double delta_y = mouse_pos.y - lastMouseY;

            glm::vec2 cam_pos = camera.get_position();
            const float cam_zoom = camera.get_zoom();

            cam_pos.x -= static_cast<float>(delta_x) * cam_zoom * aspect_ratio / static_cast<float>(window_width) *
              20.0f;
            cam_pos.y += static_cast<float>(delta_y) * cam_zoom / static_cast<float>(window_height) * 20.0f;

            camera.set_position(cam_pos);
            camera.update_matrix({window_width, window_height});

            lastMouseX = mouse_pos.x;
            lastMouseY = mouse_pos.y;
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
              if (distance_squared(mouse_world_pos, triangle.get_vertices()[i]) < 0.09f) {
                dragging_vertex = true;
                selected_vertex = i;
                break;
              }
            }
          }
          else if (selected_vertex != -1) {
            if (glm::vec2 snapped_pos = snap_to_grid(mouse_world_pos); snapped_pos != triangle.get_vertices()[
              selected_vertex]) {
              triangle.move_vertex(selected_vertex, snapped_pos);
            }
          }
        }
        else if (dragging_vertex) {
          dragging_vertex = false;
          selected_vertex = -1;
        }
      }

      // Rendering
      glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      renderer.set_color(grid_color);
      renderer.draw_grid(camera.get_projection(), camera.get_view(), grid_model);

      renderer.set_color(triangle_color);
      renderer.draw_triangle(triangle, camera.get_projection(), camera.get_view(), triangle_model);

      for (int i = 0; i < 3; ++i) {
        bool selected = dragging_vertex && i == selected_vertex;
        renderer.set_color(selected ? triangle_vertex_selected_color : triangle_vertex_color);
        renderer.draw_circle(triangle.get_vertices()[i], 0.15f, camera.get_projection(), camera.get_view());
      }

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      bool save_screenshot = false;
      bool save_scene = false;
      bool load_scene = false;
      bool exit = false;
      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("Save screenshot", "Ctrl+S")) {
            save_screenshot = true;
          }
          if (ImGui::MenuItem("Save scene", "Ctrl+Shift+S")) {
            save_scene = true;
          }
          if (ImGui::MenuItem("Load scene", "Ctrl+O")) {
            load_scene = true;
          }
          if (ImGui::MenuItem("Exit", "Alt+F4")) {
            exit = true;
          }
          ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
      }

      if (save_screenshot) {
        Screenshot::save("screenshot.png", window_width, window_height);
        save_screenshot = false;
        std::cout << "INFO: Saved screenshot as screenshot.png\n";
      }

      if (save_scene) {
        Saves::save_to_file("scene.json", triangle, triangle_color);
        save_scene = false;
        std::cout << "INFO: Saved scene to scene.json\n";
      }

      if (load_scene) {
        Saves::load_from_file("scene.json", &triangle, &triangle_color);
        load_scene = false;
        std::cout << "INFO: Loaded scene from scene.json\n";
      }

      if (exit) {
        exit = false;
        std::cout << "INFO: Exiting\n";
        glfwSetWindowShouldClose(window, true);
      }

      auto sides = triangle.get_sides();

      {
        ImGui::Begin("Properties");
        ImGui::InputFloat("Side A", &sides[0], 0.1f, 1.0f, "%.2f");
        ImGui::InputFloat("Side B", &sides[1], 0.1f, 1.0f, "%.2f");
        ImGui::InputFloat("Side C", &sides[2], 0.1f, 1.0f, "%.2f");
        ImGui::Separator();
        ImGui::Text("Triangle Area: %.2f", triangle.get_area());
        ImGui::Separator();
        for (auto vertex : triangle.get_vertices()) {
          ImGui::Text("(%.2f %.2f)", vertex.x, vertex.y);
        }
        ImGui::End();
      } // ImGui Properties

      render_heron_steps_panel(steps);

      {
        ImGui::Begin("Colors");
        ImGui::ColorEdit4("Background", glm::value_ptr(background_color));
        ImGui::ColorEdit4("Grid", glm::value_ptr(grid_color));
        ImGui::ColorEdit4("Triangle", glm::value_ptr(triangle_color));
        ImGui::ColorEdit4("Vertex", glm::value_ptr(triangle_vertex_color));
        ImGui::ColorEdit4("Selected Vertex", glm::value_ptr(triangle_vertex_selected_color));
        ImGui::End();
      } // ImGui Colors

      {
        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        ImGui::Text("Frame Time: %.2f", delta_time);

        ImGui::Separator();

        ImGui::Checkbox("VSync", &want_vsync);
        ImGui::Checkbox("Unlock FPS", &unlock_fps);
        if (!unlock_fps) {
          ImGui::SliderInt("Max FPS", &max_fps, 15, 240);
          ImGui::Text("Target FPS: %d", max_fps);
        }
        else {
          ImGui::Text("Target FPS: Unlimited");
        }

        ImGui::End();
      } // ImGui Debug


      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


      glfwSwapBuffers(window);
      glfwPollEvents();

      if (!unlock_fps) limit_fps(max_fps, unlock_fps);
    } // end of game loop

    std::cout << "INFO: Cleaning up...\n";
  } // end of renderer

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  if (ImGui::GetCurrentContext() != nullptr) ImGui::DestroyContext();
  glfwTerminate();
  std::cout << "INFO: Successfully cleaned up\n";
  return EXIT_SUCCESS;
}

#ifdef HERON_RELEASE

#ifdef HERON_PLATFORM_WINDOWS
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#endif