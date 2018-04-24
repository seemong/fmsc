/**
 * mesh.h
 * 
 * Mesh objects define a set of vertices, indices and normals
 */

#ifndef MESH_H
#define MESH_h

#include <memory>
#include <list>
#include "display.h"
using namespace std;

class IndexStrip {
protected:
    shared_ptr<int> _indices;
    int _number_of_indices;
    
public:
    IndexStrip(shared_ptr<int> indices, int number_of_indices) :
        _indices(indices), _number_of_indices(number_of_indices) {
    }
    
    inline shared_ptr<int> get_indices() { return _indices; }
    inline int get_number_of_indices() { return _number_of_indices; }
};

/**
 * Mesh class holds vertices. Indices and normals are set by
 * the subclasses
 */
class Mesh {
protected:
    shared_ptr<float> _vertices;
    int _number_of_vertices;
    list<IndexStrip> _index_list;
    shared_ptr<float> _normals;
    
    shared_ptr<VBO> _vertex_vbo;
    shared_ptr<VBO> _normals_vbo;
    
public:
    Mesh(shared_ptr<float> vertices, int number_of_vertices, 
        list<IndexStrip> index_strip, shared_ptr<float> normals);
    
    inline shared_ptr<float> get_vertices() { return _vertices; }
    inline int get_number_of_vertices() { return _number_of_vertices; }
    
    inline list<IndexStrip> get_index_list() { return _index_list; }
    inline int get_index_list_size() { return _index_list.size(); }
    
    inline shared_ptr<float> get_normals() { return _normals; }
    inline int get_number_of_normals() { return _number_of_vertices; }
    
    inline shared_ptr<VBO> get_vertex_vbo() { return _vertex_vbo; }
    inline shared_ptr<VBO> get_normals_vbo() { return _normals_vbo; }
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

list<IndexStrip> make_faces(int _xsize, int _ysize) ;

#endif
