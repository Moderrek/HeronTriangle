#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED 1

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Heron {

  struct ShaderProgramSource {
    std::string vertex;
    std::string fragment;
  };
  
  class Shader {
  public:
    Shader(const std::string& filepath);
    Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();
    
    void bind() const;
    void unbind() const;

    void remove();
    
    void set_uniform_1i(const std::string& name, int v0) const;
    
    void set_uniform_4f(const std::string& name, const glm::vec4& value) const;
    void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    
    void set_uniform_mat4f(const std::string& name, const glm::mat4& matrix) const;
    
    static ShaderProgramSource parse_shader(const std::string& filepath);
  private:
    unsigned int m_renderer_id;
    
    std::string m_filepath;
    mutable std::unordered_map<std::string, int> m_uniform_location_cache;
  
    unsigned int compile_shader(unsigned int type, const std::string& source);
    unsigned int create_shader(const std::string& vertex, const std::string& fragment);
    
    int get_uniform_location(const std::string& name) const;
  };

}

#endif // SHADER_HPP_INCLUDED