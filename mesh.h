/**
 * mesh.h
 * 
 * Mesh objects define a set of vertices, indices and normals
 */

#ifndef MESH_H
#define MESH_h

#include <memory>
using namespace std;

/**
 * Mesh class holds vertices. Indices and normals are set by
 * the subclasses
 */
class Mesh {
protected:
    shared_ptr<float> _vertices;
    int _number_of_vertices;
    shared_ptr<int> _indices;
    int _number_of_indices;
    shared_ptr<float> _normals;
    
public:
    Mesh(shared_ptr<float> vertices, int number_of_vertices, 
        shared_ptr<int> indices, int number_of_indices,
        shared_ptr<float> normals);
    
    inline shared_ptr<float> get_vertices() { return _vertices; }
    inline int get_number_of_vertices() { return _number_of_vertices; }
    
    inline shared_ptr<int> get_indices() { return _indices; }
    inline int get_number_of_indices() { return _number_of_indices; }
    
    inline shared_ptr<float> get_normals() { return _normals; }
    inline int get_number_of_normals() { return _number_of_vertices; }
};

class RectangleMesh : public Mesh {
protected:
    int _xsize;
    int _ysize;
    
    /**
     * Make normal vectors using neighbor cross products
     */
    void make_normals();
    
public:
    RectangleMesh(shared_ptr<float> vertices, int xsize, int ysize);

    inline int get_xsize() { return _xsize; }
    inline int get_ysize() { return _ysize; }
};

/**
 * Wire rectangle meshes make a see through grid of wires
 */
class WireRectangleMesh : public RectangleMesh {
protected:
    void make_mesh_indices();
    
public:
   WireRectangleMesh(shared_ptr<float> vertices, int xsize, int ysize);
};

/**
 * Makes a solid face using triangle strips
 */
class FaceRectangleMesh : public RectangleMesh {
protected:
    void make_face_indices();
    
public:
    FaceRectangleMesh(shared_ptr<float> vertices, int xsize, int ysize);
};

#endif
