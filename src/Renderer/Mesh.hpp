#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED 1

#include <vector>

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class Mesh {
public:
  Mesh();
private:
  VertexBuffer m_vertex_buffer;
  VertexBufferLayout m_vertex_buffer_layout;
  IndexBuffer m_index_buffer;
};

#endif // MESH_HPP_INCLUDED