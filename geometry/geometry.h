/**
 * geometry.h
 *
 * Basic geometry objects
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <tuple>
#include <iostream>
using namespace std;

class Coord {
protected:
    float _x;
    float _y;
    float _z;

public:
    Coord(float x, float y, float z) {
        _x = x;
        _y = y;
        _z = z;
      }

    float get_x() const {
    return _x;
    }

    void set_x(float x) {
        _x = x;
    }

    float get_y() const {
        return _y;
    }

    void set_y(float y) {
        _y = y;
    }

    float get_z() const {
        return _z;
    }

    float set_z(float z) {
        _z = z;
    }

    friend ostream& operator <<(ostream& outputStream, const Coord& c);
};

#endif // GEOMETRY_H
