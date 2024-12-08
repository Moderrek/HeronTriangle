#ifndef INDEX_BUFFER_HPP_INCLUDED
#define INDEX_BUFFER_HPP_INCLUDED 1

class IndexBuffer {
public:
  IndexBuffer(const unsigned int* data, unsigned int count);
  ~IndexBuffer();

  void bind() const;
  void unbind() const;

  inline unsigned int get_count() const { return m_count; }
  
private:
    unsigned int m_renderer_id;
    unsigned int m_count;
};

#endif // INDEX_BUFFER_HPP_INCLUDED