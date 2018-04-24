OBJECTS = \
	display.o \
	geofile.o \
	geometry.o \
	mapobject.o \
	mesh.o \
	fms.o

TARGET = fms

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

CFLAGS = -g -I/usr/include/libxml2 -DGL_GLEXT_PROTOTYPES

LIBS = -lxml2 -lGL -lglut -lGLU -lgdal

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) $(CFLAGS) -o fms

fms.o: fms.cpp geometry.h mapobject.h display.h geofile.h mesh.h

geometry.o: geometry.h geometry.cpp

mapobject.o: geometry.h mapobject.cpp

display.o: display.h display.cpp

geofile.o: geofile.h geofile.cpp mesh.h

mesh.o: mesh.h mesh.cpp

clean:
	rm $(OBJECTS) $(TARGET)

test: display.o test.o
	g++ display.o test.o $(LIBS) $(CFLAGS) -o test
	
