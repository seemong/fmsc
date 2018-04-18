/**
 * Implement GeoFile, an interface to HGT and TIF digital elevation data
 */

#include <fstream>
#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>
#include <string>
#include <cassert>
#include <iostream>
#include "geofile.h"

void
GeoFile::init() {
    static bool initialized = false;
    if (! initialized) {
        GDALAllRegister();
        initialized = true;
    }
}

GeoFile::GeoFile(string filename):
    _filename(filename), _rows(0), _cols(0), _bands(0), _left(0), _bottom(0),
    _top(0), _right(0), _xincrement(0), _yincrement(0) {
    GeoFile::init();
    _dataset = (GDALDataset *) GDALOpen(_filename.c_str(), GA_ReadOnly);
    _cols = _dataset->GetRasterXSize();
    _rows = _dataset->GetRasterYSize();
    _bands = _dataset->GetRasterCount();

    double gt[6];
    _dataset->GetGeoTransform(gt);
    _left = gt[0];
    _xincrement = gt[1];
    _top = gt[3];
    _yincrement = -gt[5];
    _right = _left +  _xincrement * (_cols - 1);
    _bottom = _top - _yincrement * (_rows - 1);
}

GeoFile::~GeoFile() {
    GDALClose(_dataset);
}


shared_ptr<float>
GeoFile::read_data(int xoff, int yoff, int xsize, int ysize) {
    shared_ptr<float> data(new float[xsize * ysize]);
    GDALRasterBand * band = _dataset->GetRasterBand(1);
    for(int row = 0; row < ysize; row++) {
        CPLErr err = band->RasterIO(GF_Read, xoff, yoff, xsize, ysize,
            data.get(), xsize, ysize, GDT_Float32, 0, 0);
        assert(err == CE_None);
    }
    return data;
}

GeoTile
GeoFile::read_data_as_tile(int xoff, int yoff,
    int xsize, int ysize) {
    shared_ptr<float> band_data = read_data(xoff, yoff, xsize, ysize);
    shared_ptr<float> vertex_data(new float[xsize * ysize * 3]);
    float boxleft = _left + xoff * _xincrement;
    float boxtop = _top - yoff * _yincrement;
    float boxright = boxleft + (xsize - 1) * _xincrement;
    float boxbottom = boxtop - (ysize - 1) * _yincrement;
    for(int row = 0; row < ysize; row++) {
        for(int col = 0; col < xsize; col++) {
            float x = boxleft + col * _xincrement;
            float y = boxtop - row * _yincrement;
            float z = band_data.get()[col + row * xsize];
            int vertex_index = (col + row * xsize) * 3;
            vertex_data.get()[vertex_index] = x;
            vertex_data.get()[vertex_index + 1] = y;
            vertex_data.get()[vertex_index + 2] = z;
        }
    }
    return GeoTile(xsize, ysize, boxleft, boxbottom, boxright, boxtop, 
        vertex_data);
}

GeoTile 
GeoFile::get_tile(float left, float bottom, float right, float top) {
    int xoff = (left - _left)/_xincrement;
    int yoff = (_top - top)/_yincrement;
    int xsize = (right - left)/_xincrement;
    int ysize = (top - bottom)/_yincrement;
    return read_data_as_tile(xoff, yoff, xsize, ysize);
}

shared_ptr<int>
make_mesh_indices(int xsize, int ysize) {
    xcount = 2 + (xsize - 2) * 2;
    ycount = 2 + (ysize - 2) * 2;
    shared_ptr<int> indices(new int[xcount * ycount * 2]);
    
    // do east west lines
    int i = 0

    // do north south lines
    
    return indices;
}
