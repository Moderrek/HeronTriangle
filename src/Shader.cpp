#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "renderer.h"

ShaderProgramSource Shader::parse_shader(const std::string& filepath) {
  std::ifstream stream{ filepath };

  enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    // change shader mode
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        // set mode to vertex
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos) {
        // set mode to fragment
        type = ShaderType::FRAGMENT;
      }
      else {
        type = ShaderType::NONE;
      }
      continue;
    }
    // append source code
    if (type == ShaderType::NONE) continue;
    ss[(int)type] << line << '\n';
  }

  return {
    .vertex= ss[0].str(),
    .fragment= ss[1].str()
  };
}


Shader::Shader(const std::string& filepath) {
  const ShaderProgramSource source = parse_shader(filepath);
  m_renderer_id = create_shader(source.vertex, source.fragment);
}

Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
  m_filepath = name;
  m_renderer_id = create_shader(vertexSrc, fragmentSrc);
}

Shader::~Shader() {
  GLCall(glDeleteProgram(m_renderer_id));
}

void Shader::bind() const {
GLCall(glUseProgram(m_renderer_id));
}

void Shader::unbind() const {
GLCall(glUseProgram(0));
}

void Shader::set_uniform_1i(const std::string& name, int v0) const {
  GLCall(glUniform1i(get_uniform_location(name), v0));
}

void Shader::set_uniform_4f(const std::string& name, const glm::vec4& value) const {
  GLCall(glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w));
}

void Shader::set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3) const {
  GLCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

void Shader::set_uniform_mat4f(const std::string& name, const glm::mat4& matrix) const {
  GLCall(glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::compile_shader(unsigned int type, const std::string& source) {
  GLCall(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : (type == GL_FRAGMENT_SHADER ? "fragment" : "unknown")) << " shader!\n";
    std::cout << message << '\n';
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}


unsigned int Shader::create_shader(const std::string& vertex, const std::string& fragment) {
  GLCall(const unsigned int program = glCreateProgram());
  const unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex);
  //assert(vs != 0);
  const unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment);
  //assert(fs != 0);
  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));
  return program;
}

int Shader::get_uniform_location(const std::string& name) const {
  if (m_uniform_location_cache.contains(name))
    return m_uniform_location_cache[name];
  GLCall(const int location = glGetUniformLocation(m_renderer_id, name.c_str()));
  if (location == -1)
    std::cout << "[Shader] " << m_filepath << " Warning: cannot find uniform " << name << '\n';
  m_uniform_location_cache[name] = location;
  return location;
}
