/**
 * vbo.h --
 * 
 * Encapsulates OpenGL Vertex Buffer Objects
 */

#ifndef VBO_H
#define VBO_H

class VertexBufferObject {
protected:
    unsigned int _vbo;

public:
    VertexBufferObject(float * data, int size);
    ~VertexBufferObject();
    
    inline unsigned int get_vbo() { return _vbo; }
    
    void bind();
    void unbind();
};

#endif // VBO_H
