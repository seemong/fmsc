/**
 * mapstore.cpp
 * 
 * Implement map store of multiple gdal files
 */

#include <iostream>
#include <dirent.h>
#include <assert.h>
#include "geofile.h"
#include "mapstore.h"
using namespace std;

Mapstore::Mapstore(const string& directory) {
    add_geofiles_from_directory(directory);
}

void 
Mapstore::add_geofiles_from_directory(const string& directory) {
    DIR * dir = opendir(directory.c_str());
    if (dir == 0) 
        assert(false);
        
    struct dirent * de;
    while((de = readdir(dir)) != 0) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;
        else if (de->d_type == DT_DIR) {
            string dirname = directory + string("/") + string(de->d_name);
            add_geofiles_from_directory(dirname);
        } else {
            string filename = directory + string("/") + string(de->d_name);
            add_geofile(filename);
        }
    }
}

void
Mapstore::add_geofile(const string& filename) {
    cout << "Adding geo file " + filename << endl;
    shared_ptr<GeoFile> geofile = shared_ptr<GeoFile>(new GeoFile(filename));
    bool good = geofile->open();
    if (good) {
        _geo_files.push_back(geofile);
    }
}

list<shared_ptr<GeoTile>>
Mapstore::get_geotiles(float left, float bottom, float right, float top, 
        int resolution) {
    float increment = resolution/3600.0;
    list<shared_ptr<GeoTile>> tiles;
    for(shared_ptr<GeoFile>& geofile : _geo_files) {
        if (round(increment * 100000) == 
            round(geofile->get_xincrement() * 100000) && (
                geofile->contains(left, bottom) ||
                geofile->contains(right, top) ||
                geofile->contains(left, top) ||
                geofile->contains(right, bottom))) {
            shared_ptr<GeoTile> tile = geofile->get_tile(left, bottom, 
                right, top);
            tiles.push_back(tile);
        }
    }
    
    return tiles;
}
