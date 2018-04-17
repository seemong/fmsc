/**
 * geometry.cpp
 *
 * Basic geometry objects
 */

#include "geometry.h"

ostream& operator <<(ostream& outputStream, const Coord& c) {
    outputStream << "(" << c.get_filename() << ")";
}
