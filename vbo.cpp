/**
 * vbo.cpp
 * 
 */

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <cassert>
#include "vbo.h"

VertexBufferObject::VertexBufferObject(float * data, int size) {
    glGenBuffers(1, &_vbo);
    glBufferData(_vbo, size, (const void *) data, GL_STATIC_DRAW);
}

VertexBufferObject::~VertexBufferObject() {
    glDeleteBuffers(1, &_vbo);
}

void
VertexBufferObject::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void 
VertexBufferObject::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
