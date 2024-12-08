#include "Application.hpp"

namespace Heron {

  Application::Application() : m_window{"Heron Triangle", 1600, 900}, m_renderer{} {

  }

  Application::~Application() {

  }

  void Application::close() {
    m_running = false;
  }

}