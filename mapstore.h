/**
 * mapstore.h --
 * 
 * Stores a bunch of geofile maps and gives methods to retrieve meshes
 * from them
 */

#ifndef MAPSTORE_H
#define MAPSTORE_H

#include <string>
#include <list>
#include <memory>
#include "geofile.h"
using namespace std;

class Mapstore {
protected:
    list<shared_ptr<GeoFile>> _geo_files;

    void add_geofiles_from_directory(const string& directory);
    void add_geofile(const string& filename);

public:
    /**
     * Create a mapstore based on all the given files
     */
    Mapstore(list<string> filepaths);
};

#endif
