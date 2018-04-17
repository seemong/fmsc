/**
 * display.cpp
 *
 * Abstracts Open GL display
 */

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
    glutInit(&argc, (char **) &argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow(_name.c_str());
    glutInitWindowSize(_width, _height);
    glutInitWindowPosition(_x, _y);
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
    float position[] = { 0.0, 0.0, 1.0, 0.0 }; // default is from z
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void
Display::set_redraw(void (*redraw)(Display *, void *), void * arg) {
    _redraw = redraw;
    _redraw_arg = arg;
}

void
Display::predraw() {
}

void
Display::redraw() {
    this->_redraw(this, _redraw_arg);
}

void
Display::postdraw() {
}

void
Display::set_ortho(int x, int y, int width, int height) {
}


void
Display::set_perspective(float fov, float aspect, float zNear, float zFar) {
}

void
Display::lookAt(float eyex, float eyey, float eyez,
    float centerx, float centery, float centerz,
    float upx, float upy, float upz) {
}

void
Display::set_position(float x, float y, float z, float w) {
}