#include <stdio.h>
#include <iostream>
#include "mapstore.h"
using namespace std;

int
main(int argc, char ** argv) {
        cout << "Hello World" << endl;
        list<string> l;
        l.push_back("/usr/local/google/home/seemongt/fms/data");
        Mapstore m(l);
        
        list<GeoTile> tiles = m.get_geotiles(-122.2, 46.3, -121.1, 48.2, 3);
        int i = 0;
        for(GeoTile& tile : tiles) {
            printf("(%d): %f, %f, %f, %f\n", i++, 
                tile.get_left(), tile.get_bottom(), tile.get_right(), tile.get_top());
        }
        
        cout << "Goodbye world" << endl;
}
