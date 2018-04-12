OBJECTS = \
	geometry/geometry.o \
	geometry/mapobject.o \
	fms/fms.o

.cpp.o:
	g++ -c -I . $< -o $@

runfms: $(OBJECTS)
	g++ $(OBJECTS) -o runfms

fms/fms.o: fms/fms.cpp geometry/geometry.h geometry/mapobject.h

geometry/geometry.o: geometry/geometry.h geometry/geometry.cpp

geometry/mapobject.o: geometry/geometry.h geometry/mapobject.cpp

clean: 
	rm $(objects) runfms
