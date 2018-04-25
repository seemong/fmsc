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

Mapstore::Mapstore(list<string> directories) {
    for(const string& directory : directories) {
        add_gdal_files_from_directory(directory);
    }
}

void 
Mapstore::add_gdal_files_from_directory(const string& directory) {
    cout << "Adding directory " + directory << endl;
    DIR * dir = opendir(directory.c_str());
    if (dir == 0) 
        assert(false);
        
    struct dirent * de;
    while((de = readdir(dir)) != 0) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;
        if (de->d_type == DT_DIR) {
            string dirname = directory + string("/") + string(de->d_name);
            add_subdirectories(dirname);
        } 
        cout << "Adding file " << directory << "/" << de->d_name << endl;
    }
}

void 
Mapstore::add_subdirectories(const string& directory) {
    // add subdirectories
    DIR * dir = opendir(directory.c_str());
    if (dir == 0)
        return;
          
    struct dirent * de;
    while((de = readdir(dir)) != 0) {
        if (de->d_type != DT_DIR)
            continue;
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;
        
        cout << "Going into subdir " << de->d_name << endl;
        string dirname = directory + string("/") + string(de->d_name);
        add_gdal_files_from_directory(dirname);
    }
    
    closedir(dir);
}
