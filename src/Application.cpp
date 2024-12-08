#include "Application.hpp"

namespace Heron {

  Application::Application() : m_window{"Heron Triangle", 1600, 900} {
    Renderer2D::Init();
  }

  Application::~Application() {
    Renderer2D::Shutdown();
  }

  void Application::close() {
    m_running = false;
  }

}