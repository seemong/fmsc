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
using namespace std;

/**
 *  GeoTile represents a slice of vertex data read from a GeoFile
 */
class GeoTile {
protected:
    int _xsize;
    int _ysize;
    float _left;
    float _bottom;
    float _right;
    float _top;
    shared_ptr<float> _data;

public:
    GeoTile(int xsize, int ysize, float left, float bottom,
        float right, float top, shared_ptr<float> data) :
        _xsize(xsize), _ysize(ysize), _left(left), _bottom(bottom),
        _right(right), _top(top), _data(data){
    }

    inline int get_xsize() { return _xsize; }
    inline int get_ysize() { return _ysize; }
    inline float get_left() { return _left; }
    inline float get_bottom() { return _bottom; }
    inline float get_right() { return _right; }
    inline float get_top() { return _top; }
    inline shared_ptr<float> get_data() { return _data; }

    inline float * get_vertex(int xoff, int yoff) {
        return _data.get() + (xoff + yoff * _xsize) * 3;
    }
};

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
    GeoTile read_data_as_tile(int xoff, int yoff,
        int xsize, int ysize);

    /**
     *  Read the data in coodinates and return as a GeoTile
     */
    GeoTile get_tile(float left, float bottom, float right,
        float top);
};

/**
 * Make a mesh of indices of the number of cols and rows
 * with east west lines and north south lines
 */
shared_ptr<int> make_mesh_indices(int xsize, int ysize);

/**
 * Make an array of normal pointers of size (xsize * ysize * 3) floats
 * for the given vertices with size xsize and ysize*/
shared_ptr<float> make_normals(shared_ptr<float> vertices, int xsize, int ysize);

#endif // GEOFILE_H
