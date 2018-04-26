/**
 * mapcache.h --
 * 
 * Implements a cache of tiles -- given a position, it calculates
 * the tiles to use. It returns a new set of tiles if the 
 * position is within reload_interval
 */
#ifndef MAPCACHE_H
#define MAPCACHE_H

#include <memory>
#include "mapstore.h"
using namespace std;

class Mapcache {
protected:
    shared_ptr<Mapstore> _mapstore;
    float _tile_range;
    float _reload_distance;
    
    // Current set of coordinate extents
    float _left;
    float _bottom;
    float _right;
    float _top;
    list<shared_ptr<GeoTile>> _tiles;

public:
    Mapcache(shared_ptr<Mapstore> mapstore, float tile_range, 
        float reload_distance);
    
    list<shared_ptr<GeoTile>> get_tiles(float x, float y, float z);
};

#endif // MAPCACHE_H
