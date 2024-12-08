#ifndef VERTEX_ARRAY_HPP_INCLUDED
#define VERTEX_ARRAY_HPP_INCLUDED 1

#include "VertexBuffer.hpp"


namespace Heron {

class VertexBufferLayout;

class VertexArray {
public:
  VertexArray();
  ~VertexArray();

  void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

  void bind() const;
  void unbind() const;

  void remove();
private:
  unsigned int m_renderer_id;
};

}

#endif // VERTEX_ARRAY_HPP_INCLUDED
