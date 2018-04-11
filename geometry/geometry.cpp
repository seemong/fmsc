/**
 * geometry.cpp
 *
 * Basic geometry objects
 */

#include "geometry.h"

ostream& operator <<(ostream& outputStream, const Coord& c) {
    outputStream << "(" << c.get_x() << ", " << c.get_y() << ", "
        << c.get_z() << ")";
}


int main() {
    Coord c(1, 2, 3);
    cout << c << "\n";
}
