/**
 * geofile.h
 *
 * Presents abstract interface to TIF or HGT files
 */

#ifndef GEOFILE_H
#define GEOFILE_H

#include <string>
#include <gdal.h>

/**
 * GeoFile is a class that defines a TIF or HGT files with helpfu;
 * methods to generate mesh vertices from the data, and to get slices
 * of the data based on elat/lon bounding boxes
 */
class GeoFile {
protected:
    string _filename;
    int _rows;
    int _cols;
    float _left;
    float _right;
    float _top;
    float _bottom;
    float _xincrement;
    float _yincrement;

public:
    GeoFile(string filename);
    ~GeoFile();

    inline string get_filename() {
        return _filename;
    }

    inline int get_rows() {
        return _rows;
    }

    inline int get_cols() {
        return _cols;
    }

    inline float get_left() {
        return _left;
    }

    inline float get_bottom() {
        return _bottom;
    }

    inline float get_right() {
        return _right;
    }

    inline float get_top() {
        return _top;
    }

    inline float get_xincrement() {
        return _xincrement;
    }

    inline float get_yincrement() {
        return _yincrement;
    }
};

#endif
