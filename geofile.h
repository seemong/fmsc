/**
 * geofile.h
 *
 * Presents abstract interface to TIF or HGT files
 */

#ifndef GEOFILE_H
#define GEOFILE_H

#include <string>
#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>
#include <memory>
#include "geotile.h"
using namespace std;


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
    int _bands;
    float _left;
    float _right;
    float _top;
    float _bottom;
    float _xincrement;
    float _yincrement;
    GDALDataset * _dataset;

    /**
     * Single initialization of GeoFile class
     */
    static void init();

public:
    GeoFile(string filename);
    virtual ~GeoFile();
    
    /**
     *  Return true if the file opened successfully
     */
    bool open();

    inline string get_filename() { return _filename; }
    inline int get_rows() { return _rows; }
    inline int get_cols() { return _cols; }
    inline float get_left() { return _left; }
    inline float get_bottom() { return _bottom; }
    inline float get_right() { return _right; }
    inline float get_top() { return _top; }
    inline float get_xincrement() {return _xincrement; }
    inline float get_yincrement() {return _yincrement; }

    /**
     * Read a slice of data at xoff, yoff of the given number of 
     * cols and rows. Returns a ref counted shared pointer to
     * a float array that is constructed to hold the data
     */
    shared_ptr<float> read_data(int xoff, int yoff,
        int xsize, int ysize);

    /**
     * Read the data and return as vertices
     */
    shared_ptr<GeoTile> read_data_as_tile(int xoff, int yoff,
        int xsize, int ysize);

    /**
     *  Read the data in coodinates and return as a GeoTile
     */
    shared_ptr<GeoTile> get_tile(float left, float bottom, float right,
        float top);
        
    /**
     * True if this GeoFile contains this point
     */
    bool contains(float x, float y);
};

/**
 * Converts meters to units of arc
 */
inline float 
meters_to_arc(float meters) {
    return meters / 1852.0 / 60.0;
}


#endif // GEOFILE_H
