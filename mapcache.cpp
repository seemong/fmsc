/**
 * mapcache.cpp
 * 
 * Implements mapcache to give tile updates
 */

#include <iostream>
#include <cassert>
#include "mapcache.h"
using namespace std;

Mapcache::Mapcache(shared_ptr<Mapstore> mapstore, float tile_range, 
    float reload_distance) :
    _mapstore(mapstore), _tile_range(tile_range), 
    _reload_distance(reload_distance) {
    _left = 180;
    _bottom = 90;
    _right = -180;
    _top = -90;
}

list<shared_ptr<GeoTile>>
Mapcache::get_tiles(float x, float y, float z) {
    if (x - _reload_distance > _left &&
        x + _reload_distance < _right &&
        y - _reload_distance > _bottom &&
        y + _reload_distance < _top)
        return _tiles;
        
    // Make a new list of tiles
    // TODO: stitch scene when going between geotiles in the map store
    cout << "NEW TILE at " << x << ", " << y << ", " << z << endl;
    _left = x - _tile_range;
    _bottom = y - _tile_range;
    _right = x + _tile_range;
    _top = y + _tile_range;
    _tiles = _mapstore->get_geotiles(_left, _bottom, _right, _top, 3);
    return _tiles;
}
