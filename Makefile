OBJECTS = \
	display/display.o \
	geometry/geometry.o \
	geometry/mapobject.o \
	fms/fms.o

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

CFLAGS = -g

LIBS = -lxml2 -lGL -lglut

fms: $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) $(CFLAGS) -o fms

fms.o: fms.cpp geometry.h mapobject.h display.h

geometry.o: geometry.h geometry.cpp

mapobject.o: geometry.h mapobject.cpp

display.o: display.h display.cpp

clean:
	rm $(objects) runfms
