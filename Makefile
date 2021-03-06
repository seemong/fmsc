OBJECTS = \
	display.o \
	geofile.o \
	geotile.o \
	geometry.o \
	mapobject.o \
	mesh.o \
	fms.o \
	mapstore.o \
	mapcache.o \
	sthread.o

TARGET = fms

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

CFLAGS = -g -I/usr/include/libxml2 -DGL_GLEXT_PROTOTYPES

LIBS = -lxml2 -lGL -lglut -lGLU -lgdal

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) $(CFLAGS) -o $(TARGET)

fms.o: fms.cpp geometry.h mapobject.h display.h geofile.h mesh.h \
	mapstore.h geotile.h sthread.h

geometry.o: geometry.h geometry.cpp

mapobject.o: geometry.h mapobject.cpp

display.o: display.h display.cpp

geofile.o: geofile.h geofile.cpp mesh.h geotile.h

geotile.o: geotile.h geotile.cpp mesh.h

mesh.o: mesh.h mesh.cpp

mapstore.o: mapstore.h mapstore.cpp

mapcache.o: mapcache.h mapcache.cpp

sthread.o: sthread.h

testfms.o: testfms.cpp

clean:
	rm $(OBJECTS) $(TARGET)

testfms: display.o testfms.o mapstore.o geofile.o mapcache.o geotile.o
	g++ display.o testfms.o mapstore.o geofile.o $(LIBS) $(CFLAGS) -o testfms
	
