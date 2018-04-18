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
    _top(0), _right(0), _xincrement(0), _yincrement(0), _raster(0) {
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
    if (_raster)
        delete _raster;
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
