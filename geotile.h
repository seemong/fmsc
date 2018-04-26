/**
 * geotile.h
 * 
 * Tiles of terrain data that can be made into drawable meshes
 */

#ifndef GEOTILE_H
#define GEOTILE_H

#include <memory>
#include "mesh.h"
using namespace std;

/**
 *  GeoTile represents a slice of vertex data read from a GeoFile
 */
class GeoTile {
protected:
    int _xsize;
    int _ysize;
    float _left;
    float _bottom;
    float _right;
    float _top;
    shared_ptr<float> _vertices;
    shared_ptr<FaceRectangleMesh> _face_mesh;

public:
    GeoTile(int xsize, int ysize, float left, float bottom,
        float right, float top, shared_ptr<float> vertices);

    inline int get_xsize() { return _xsize; }
    inline int get_ysize() { return _ysize; }
    inline float get_left() { return _left; }
    inline float get_bottom() { return _bottom; }
    inline float get_right() { return _right; }
    inline float get_top() { return _top; }
    inline shared_ptr<float> get_vertices() { return _vertices; }

    inline float * get_vertex(int xoff, int yoff) {
        return _vertices.get() + (xoff + yoff * _xsize) * 3;
    }
    
    shared_ptr<FaceRectangleMesh> get_face_mesh();
};

#endif // GEOTILE_H
