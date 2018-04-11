/**
 * geometry.cpp
 *
 * Implements map geometry objects
 */

#include <iostream>
#include "mapobject.h"

void Map::add_node(string id, Coord coord) {
    Node n = Node(id, coord);
    pair<int, Node> t = make_pair(_last_node_index++, n);
    _nodes_dict[id] = t;
}

int main(int argc, char * argv[]) {
    Map * m = new Map("foo");
    cout << m->get_id() << "\n";
    delete m;
/*
    map<string, tuple<int, Node>> me;
    Coord coord(1, 2, 3);
    Node n = Node("bar", coord);
    me["foo"] = make_tuple(3, n);
    */
}
