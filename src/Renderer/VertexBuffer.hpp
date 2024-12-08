#ifndef VERTEX_BUFFER_HPP_INCLUDED
#define VERTEX_BUFFER_HPP_INCLUDED 1

namespace Heron {

class VertexBuffer {
public:
  VertexBuffer(const void* data, unsigned int size);
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
  
  void remove();
private:
  unsigned int m_renderer_id;
};

}

#endif // VERTEX_BUFFER_HPP_INCLUDED