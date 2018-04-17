/**
 * mapboject.h
 *
 * Define objects for maps
 */

#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <list>
#include <map>
#include <string>
#include <tuple>
#include <iostream>
using namespace std;

#include "geometry.h"

class Mapobject {
protected:
    string _id;
    map<string, string> _attribs;

public:
    Mapobject(string id="") {
        _id = id;
    }

    string get_id() const {
        return _id;
    }

    void add_attrib(string k, string v) {
        _attribs[k] = v;
    }

    string get_attrib(string k) {
        return _attribs[k];
    }

    map<string, string> get_attribs_iterator() const {
        return _attribs;
    }
};

class Node : public Mapobject {
protected:
    Coord _coord;

public:
    Node() : Mapobject(""), _coord(Coord(0, 0, 0)) {
    }
    
    Node(string id, Coord coord) : Mapobject(id), _coord(coord) {
    }

    float get_latitude() const {
        return _coord.get_y();
    }

    void set_latitude(float lat) {
        _coord.set_y(lat);
    }

    float get_longitude() const {
        return _coord.get_x();
    }

    void set_longitude(float lon) {
        _coord.set_x(lon);
    }

    float get_altitude() const {
        return _coord.get_z();
    }

    void set_altitude(float altitude) {
        _coord.set_z(altitude);
    }
};

class Way : public Mapobject {
protected:
    list<string> _node_ids;

public:
    Way(string id="") : Mapobject(id) {
    }

    list<string> get_node_ids() const {
        return _node_ids;
    }

    void add_node_id(string id) {
        _node_ids.push_back(id);
    }
};

class _NodeDictElt {
public:
    int _index;
    Node _node;
};

class _WayDictElt {
public:
    int _index;
    Way _way;
};

class Map : public Mapobject {
protected:
    map<string, _NodeDictElt> _nodes_dict;
    map<string, _WayDictElt> _ways_dict;

    int _last_node_index = 0;
    int _last_way_index = 0;

public:
    Map(string id) : Mapobject(id) {
    }

    void add_node(Node n);
    
    void add_way(Way w);
    
    friend ostream& operator<< (ostream& out, const Map& m); 
    
    static Map * make_map(string id, string name);
};

#endif // MAPOBJECT_H
