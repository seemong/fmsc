OBJECTS = \
	display.o \
	geometry.o \
	mapobject.o \
	fms.o

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

CFLAGS = -g -I/usr/include/libxml2

LIBS = -lxml2 -lGL -lglut

fms: $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) $(CFLAGS) -o fms

fms.o: fms.cpp geometry.h mapobject.h display.h

geometry.o: geometry.h geometry.cpp

mapobject.o: geometry.h mapobject.cpp

display.o: display.h display.cpp

clean:
	rm $(objects) runfms
