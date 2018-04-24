/**
 * mesh.cpp
 * 
 * Implement Mesh objects
 */

#include <iostream>
#include <stdio.h>
#include <memory>
#include <math.h>
#include "mesh.h"
using namespace std;

Mesh::Mesh(shared_ptr<float> vertices, int number_of_vertices, 
    list<IndexStrip> index_list, shared_ptr<float> normals) {
    _vertices = vertices;
    _number_of_vertices = number_of_vertices;
    _index_list = index_list;
    _normals = normals;
    
    _vertex_vbo = shared_ptr<VBO>(new VBO(vertices.get(), 
        number_of_vertices * 3));
    _normals_vbo = shared_ptr<VBO>(new VBO(normals.get(), 
        number_of_vertices * 3));
}


RectangleMesh::RectangleMesh(shared_ptr<float> vertices, 
    int xsize, int ysize) : Mesh(vertices, xsize * ysize, 
        list<IndexStrip>(), shared_ptr<float>(0)), 
    _xsize(xsize), _ysize(ysize) {
    make_normals();
}

void 
normalize(float * v, float * rv) {
    float size = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    rv[0] = v[0] / size;
    rv[1] = v[1] / size;
    rv[2] = v[2] / size;
}

void 
sub(float * v1, float * v2, float * rv) {
    rv[0] = v1[0] - v2[0];
    rv[1] = v1[1] - v2[1];
    rv[2] = v1[2] - v2[2];
}

void
cross(float * v1, float * v2, float * rv) {
    rv[0] = v1[1] * v2[2] - v1[2] * v2[1];
    rv[1] = v1[0] * v2[2] - v1[2] * v2[0];
    rv[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void get_neighbors(float * vertices, int xoff, int yoff, 
    int xsize, int ysize, float **  v1, float ** v2) {
    int n1_x = xoff, n1_y = 0;
    if (yoff < ysize - 1)
        n1_y = yoff + 1;
    else
        n1_y = yoff - 1;

    int n2_y = yoff, n2_x = 0;
    if (xoff < xsize - 1)
        n2_x = xoff + 1;
    else
        n2_x = xoff - 1;

    int v1off = n1_x + n1_y * xsize;
    *v1 = &vertices[3 * v1off];
    
    int v2off = n2_x + n2_y * xsize;
    *v2 = &vertices[3 * v2off];
}

void
RectangleMesh::make_normals() {
    _normals = shared_ptr<float>(new float[_number_of_vertices * 3]);
    for(int yoff = 0; yoff < _ysize; yoff++) { 
        for(int xoff = 0; xoff < _xsize; xoff++) { 
            float * coord = &_vertices.get()[3 * (xoff + yoff * _xsize)];
        
            float * v1, * v2;
            get_neighbors(_vertices.get(), xoff, yoff, _xsize, _ysize, 
                &v1, &v2);
            
            float s1[3], s2[3];
            sub(v1, coord, s1);
            sub(v2, coord, s2);
            
            float cp[3];
            cross(s1, s2, cp);
            
            float norm[3];
            normalize(cp, norm);
            
            int offset = 3 * (xoff + yoff * _xsize);
            if (norm[2] >= 0) {
                _normals.get()[offset] = norm[0];
                _normals.get()[offset+1] = norm[1];
                _normals.get()[offset+2] = norm[2];
            } else {
                _normals.get()[offset] = -norm[0];
                _normals.get()[offset+1] = -norm[1];
                _normals.get()[offset+2] = -norm[2];
            }
        }
    }
}

WireRectangleMesh::WireRectangleMesh(shared_ptr<float> vertices, 
    int xsize, int ysize) : RectangleMesh(vertices, xsize, ysize) {
    make_mesh_indices();
}

void
WireRectangleMesh::make_mesh_indices() {
    int east_west_count = (2 * (_xsize - 1)) * _ysize;
    int north_south_count = (2 * (_ysize - 1)) * _xsize;
    int number_of_indices = east_west_count + north_south_count;
    shared_ptr<int> indices(new int[number_of_indices]);
    IndexStrip index_strip(indices, number_of_indices);
 
    // do east west lines
    int i = 0;
    for(int row = 0; row < _ysize; row++) {
        indices.get()[i++] = row * _xsize;
        for(int col = 1; col < _xsize - 1; col++) {
            indices.get()[i++] = col + row * _xsize;
            indices.get()[i++] = col + row * _xsize;
        }
        indices.get()[i++] = _xsize - 1 + row * _xsize;
    }

    // do north south lines
    for(int col = 0; col < _xsize; col++) {
        indices.get()[i++] = col;
        for(int row = 1; row < _ysize - 1; row++) {
            indices.get()[i++] = col + row * _xsize;
            indices.get()[i++] = col + row * _xsize;
        }
        indices.get()[i++] = col + (_ysize - 1) * _xsize;
    }
    
    _index_list.push_back(index_strip);
}


FaceRectangleMesh::FaceRectangleMesh(shared_ptr<float> vertices, 
    int xsize, int ysize) : RectangleMesh(vertices, xsize, ysize) {
    make_face_indices();
}

void
FaceRectangleMesh::make_face_indices() {
    int i = 0;
    for(int yoff = 0; yoff < _ysize - 1; yoff++) {
        list<int> indices;
        for(int xoff = 0; xoff < _xsize; xoff++) {
            indices.push_back(i);
            indices.push_back(i + _xsize);
            i += 1;
        }
        
        // Made one strip, now copy it
        int num_indices = indices.size();
        shared_ptr<int> actual(new int[num_indices]);
        int y = 0;
        for(list<int>::iterator it = indices.begin();
            it != indices.end(); it++) {
            actual.get()[y++] = *it;
        }
        IndexStrip strip(actual, num_indices);
        _index_list.push_back(strip);
    }
}

list<IndexStrip> make_faces(int _xsize, int _ysize) {
    list<IndexStrip> _index_list;
    int i = 0;
    for(int yoff = 0; yoff < _ysize - 1; yoff++) {
        list<int> indices;
        for(int xoff = 0; xoff < _xsize; xoff++) {
            indices.push_back(i);
            indices.push_back(i + _xsize);
            i += 1;
        }
        int num_indices = indices.size();
        shared_ptr<int> actual(new int[num_indices]);
        int y = 0;
        for(list<int>::iterator it = indices.begin();
            it != indices.end(); it++) {
            actual.get()[y++] = *it;
        }
        IndexStrip strip(actual, num_indices);
        _index_list.push_back(strip);
    }    
    return _index_list;
}
