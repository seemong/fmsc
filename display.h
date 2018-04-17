/**
 * display.h -
 *
 * Display object abstracts OpenGL
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glut.h>
#include <list>
#include <string>
using namespace std;

/**
 * The Display class abstracts OpenGL windowing
 */
class Display {
protected:
    string _name;
    int _x, _y, _width, _height;
    void (*_redraw)(Display *, void *);
    void * _redraw_arg;

    /**
     * Holds all the displays
     */
    static list<Display *> _displays;

    /**
     *  Redraw all displays
     */
    static void redraw_all();

    /**
     * Do buffer clearing before drawing
     */
    virtual void predraw();

    /**
     * Flip the double buffer
     * */
    virtual void postdraw();

    /**
     * Redraw this window. Should be overridden by child class
     * to provide custom drawing
     */
    virtual void redraw();

public:
    /**
     * Construct a display object with name, position and size
     */
    Display(string name, int x, int y, int width, int height);
    ~Display();

    /**
     * Create window and initialize OpenGL
     */
    void create(int argc, char ** argv);

    /**
     * Set the redraw function for this display
     */
    void set_redraw(void (*redraw)(Display *, void *), void * arg);

    /**
     * Set orthographic projection
     */
    void set_ortho(float left, float right,
        float bottom, float top, float near, float far);

    /**
     * Set perspective
     */
    void set_perspective(float fovy, float aspect, float zNear, float zFar);

    /**
     * Set eye location
     */
    void lookAt(float eyex, float eyey, float eyez,
        float centerx, float centery, float centerz,
        float upx, float upy, float upz);

    /**
     * Set light position
     */
    void set_position(float x, float y, float z, float w);
};

#endif // DISPLAY_H
