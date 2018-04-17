/**
 * geometry.cpp
 *
 * Implements map geometry objects
 */

#include <iostream>
#include "mapobject.h"

ostream& operator<< (ostream& out, const Map& m) {
    out << m.get_id();
    return out;
}

void Map::add_node(Node node) {
    _NodeDictElt nde;
    nde._index = _last_node_index++;
    nde._node = node;
    _nodes_dict[node.get_id()] = nde;
}

void Map::add_way(Way way) {
    _WayDictElt wde;
    wde._index = _last_way_index++;
    wde._way = way;
    _ways_dict[way.get_id()] = wde;
}

Map *
Map::make_map(string id, string name) {
    Map * m = new Map(id);
    return m;
}

