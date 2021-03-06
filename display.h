/**
 * display.h -
 *
 * Display object abstracts OpenGL
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/freeglut.h>
#include <list>
#include <string>
#include <memory>
using namespace std;

/**
 *  The VBO class abstracts OpenGL vertex buffer objects
 */
class VBO {
protected:
    unsigned int _vbo;
    unsigned int _target;
    
public:
    /**
     * Create a VBO with size in bytes
     */
    VBO(void * data, int size, unsigned int target);
    virtual ~VBO();
    
    inline unsigned int get_vbo() { return _vbo; }
    
    void bind();
    void unbind();
};

class VertexVBO : public VBO {
protected:
    
public:
    /**
     * Create a GL_VERTEX_ARRAY VBO with size in bytes
     */
    VertexVBO(float * data, int size);
};

class IndexVBO : public VBO {
protected: 
    
public:
    /**
     * Create a GL_ELEMENT_ARRAY VBO with size in bytes
     */
    IndexVBO(int * data, int size);
};

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

    /**
     * Draw a set of vertices
     */
    void draw_vertices(float * vertices, float * indices, float * normals,
        float * color, int size, string type);

public:
    /**
     * Construct a display object with name, position and size
     */
    Display(string name, int x, int y, int width, int height);
    ~Display();
    
    /**
     * Init must be called before using any openGL function
     */
    static void Init(int argc, char ** argv);

    /**
     * Create window and initialize OpenGL
     */
    void create();

    /**
     * Do one loops worth of events
     */
    void do_event();

    /**
     * Post a redisplay request
     */
    void post_redisplay();

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
     * Set eye location. 
     * The lookAt function must be called inside redraw
     * because predraw resets the identity matrix each time
     */
    void lookAt(float eyex, float eyey, float eyez,
        float centerx, float centery, float centerz,
        float upx, float upy, float upz);

    /**
     * Set light position
     */
    void set_light_position(float x, float y, float z, float w);

    /**
     * Draw a solid sphere
     */
    void draw_solid_sphere(float radius, int slices, int stacks,
        float x, float y, float z, float r, float g, float b);

    /**
     * Draw a wire sphere
     */
    void draw_wire_sphere(float radius, int slices, int stacks,
        float x, float y, float z, float r, float g, float b);

    /**
     * Draw a solid sphere
     */
    void draw_solid_cube(float size, float x, float y, float z,
        float r, float g, float b);

    /**
     * Draw a set of lines
     */
    void draw_lines(shared_ptr<float> vertices, int num_vertices,
        shared_ptr<int> indices, int num_indices,
        shared_ptr<float> normals, float r, float g, float b,
        int line_size);
        
   /**
     * Draw a triangle strip
     */
    void draw_triangle_strip(shared_ptr<float> vertices, int num_vertices,
        shared_ptr<int> indices, int num_indices,
        shared_ptr<float> normals, float r, float g, float b);
        
   /**
     * Draw a triangle strip with vbos
     */
    void draw_triangle_strip_vbo(shared_ptr<VertexVBO> vertices_vbo, 
        shared_ptr<int> indices, int num_indices,
        shared_ptr<VertexVBO> normals_vbo, float r, float g, float b);
        
    void draw_lines_vbo(shared_ptr<VertexVBO> vertices_vbo, 
        shared_ptr<int> indices, int num_indices,
        shared_ptr<VertexVBO> normals_vbo, float r, float g, float b);
};


#endif // DISPLAY_H
