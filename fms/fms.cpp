#include <iostream>
#include "geometry/mapobject.h"

int main() {
    cout << "Hello world!\n";
    
    Map * m = Map::make_map("the-map", "rainier");
    cout << *m << "\n";
    delete m;
}
