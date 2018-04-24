/**
 * display.cpp
 *
 * Abstracts Open GL display
 */

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <cassert>
#include "display.h"

// The list of displays
list<Display *> Display::_displays;

Display::Display(string name, int x, int y, int width, int height):
    _name(name), _x(x), _y(y), _width(width), _height(height) {
    _displays.push_back(this);
}

Display::~Display() {
    _displays.remove(this);
}

void
Display::redraw_all() {
    for(list<Display *>::iterator it = _displays.begin();
        it != _displays.end(); it++) {
        (*it)->predraw();
        (*it)->redraw();
        (*it)->postdraw();
    }
}

void
Display::create(int argc, char ** argv) {
    // create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(_width, _height);
    glutInitWindowPosition(_x, _y);
    glutCreateWindow(_name.c_str());
    glutDisplayFunc(redraw_all);

    // set window properties
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    // set up lighting
    glEnable(GL_LIGHTING);
    // set up light 0
    float ambient[] = {0.15, 0.15, 0.15, 1.0}; // default is dim white
    float diffuse[] = {1.0, 1.0, 1.0, 1.0 };   // default is white light
    float position[] = { 5.0, 5.0, 5.0, 1.0 }; // default is from z
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void
Display::do_event() {
    glutMainLoopEvent();
}

void
Display::post_redisplay() {
    glutPostRedisplay();
}

void
Display::set_redraw(void (*redraw)(Display *, void *), void * arg) {
    _redraw = redraw;
    _redraw_arg = arg;
}

void
Display::predraw() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void
Display::redraw() {
    this->_redraw(this, _redraw_arg);
}

void
Display::postdraw() {
    glutSwapBuffers();
}

void
Display::set_ortho(float left, float right,
    float bottom, float top, float near, float far) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, near, far);
}

void
Display::set_perspective(float fovy, float aspect, float zNear, float zFar) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, zNear, zFar);
}

void
Display::lookAt(float eyex, float eyey, float eyez,
    float centerx, float centery, float centerz,
    float upx, float upy, float upz) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz) ;
}

void
Display::set_light_position(float x, float y, float z, float w) {
    GLfloat position[] = { x, y, z, w };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void
Display::draw_solid_sphere(float radius, int slices, int stacks,
    float x, float y, float z, float r, float g, float b) {
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(x, y, z);
    glutSolidSphere(radius, slices, stacks);
    glPopMatrix();
}

void
Display::draw_solid_cube(float size, float x, float y, float z,
    float r, float g, float b) {
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(x, y, z);
    glutSolidCube(size);
    glPopMatrix();
}

/**
 * Draw a wire sphere
 */
void
Display::draw_wire_sphere(float radius, int slices, int stacks,
    float x, float y, float z, float r, float g, float b) {
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(x, y, z);
    glutWireSphere(radius, slices, stacks);
    glPopMatrix();
}

void
Display::draw_lines(shared_ptr<float> vertices, int num_vertices,
        shared_ptr<int> indices, int num_indices,
        shared_ptr<float> normals, float r, float g, float b,
        int line_size) {
    glColor3f(r, g, b);
    glLineWidth(line_size);
    glEnableClientState(GL_VERTEX_ARRAY);

    // setup vertices
    // VBO vbo(vertices, num_vertices * 3);
    glVertexPointer(3, GL_FLOAT, 0, vertices.get());

    // setup normals
    glEnableClientState(GL_NORMAL_ARRAY);
    // VBO vbonorm(normals, num_vertices * 3);
    glNormalPointer(GL_FLOAT, 0, normals.get());

    glDrawElements(GL_LINES, num_indices, GL_UNSIGNED_INT,
           indices.get());
 
    // clean up
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void
Display::draw_triangle_strip(shared_ptr<float> vertices, int num_vertices,
        shared_ptr<int> indices, int num_indices,
        shared_ptr<float> normals, float r, float g, float b) {
    glColor3f(r, g, b);
    glEnableClientState(GL_VERTEX_ARRAY);

    // setup vertices
    // VBO vbo(vertices, num_vertices * 3);
    glVertexPointer(3, GL_FLOAT, 0, vertices.get());

    // setup normals
    glEnableClientState(GL_NORMAL_ARRAY);
    // VBO vbonorm(normals, num_vertices * 3);
    glNormalPointer(GL_FLOAT, 0, normals.get());

    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT,
           indices.get());
 
    // clean up
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void 
Display::draw_triangle_strip_vbo(shared_ptr<VBO> vertices_vbo, 
        shared_ptr<int> indices, int num_indices,
        shared_ptr<VBO> normals_vbo, float r, float g, float b) {
    glColor3f(r, g, b);
    
    vertices_vbo->bind();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // setup normals
    normals_vbo->bind();
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, 0);

    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT,
           indices.get());
 
    // clean up
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    vertices_vbo->unbind();
    normals_vbo->unbind();
}


VBO::VBO(float * data, int size) {
    glGenBuffers(1, &_vbo);
    glBufferData(_vbo, size, (const void *) data, GL_STATIC_DRAW);
}

VBO::~VBO() {
    glDeleteBuffers(1, &_vbo);
}

void
VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void 
VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
