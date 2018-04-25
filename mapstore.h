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

    void add_gdal_files_from_directory(const string& directory);
    void add_subdirectories(const string& directory);

public:
    /**
     * Create a mapstore based on all the given files
     */
    Mapstore(list<string> filepaths);
};

#endif
