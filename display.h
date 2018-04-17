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
    void (*_redraw)();
    
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
    void predraw();
    
    /**
     * Flip the double buffer
     * */
    void postdraw();
    
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
    
    void get_event();
};
 
#endif // DISPLAY_H
