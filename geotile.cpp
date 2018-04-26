/**
 * geotile.cpp
 * 
 * Implement terrain tiles
 */
 
#include <iostream>
#include "geotile.h"
using namespace std;

GeoTile::GeoTile(int xsize, int ysize, float left, float bottom,
        float right, float top, shared_ptr<float> vertices) :
        _xsize(xsize), _ysize(ysize), _left(left), _bottom(bottom),
        _right(right), _top(top), _vertices(vertices),
        _face_mesh(shared_ptr<FaceRectangleMesh>(0)) {
}

shared_ptr<FaceRectangleMesh>
GeoTile::get_face_mesh() {
    if (_face_mesh.get() != 0)
        return _face_mesh;
        
    cout << "Making new tile" << endl;
    _face_mesh = shared_ptr<FaceRectangleMesh>(new FaceRectangleMesh(_vertices, 
        _xsize, _ysize));
    return _face_mesh;
}
