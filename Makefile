OBJECTS = \
	display/display.o \
	geometry/geometry.o \
	geometry/mapobject.o \
	fms/fms.o

.cpp.o:
	g++ -c -I . -I /usr/include/libxml2  $< -o $@
	
LIBS = -lxml2 -lGL -lglut

runfms: $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) -o runfms 

fms/fms.o: fms/fms.cpp geometry/geometry.h geometry/mapobject.h

geometry/geometry.o: geometry/geometry.h geometry/geometry.cpp

geometry/mapobject.o: geometry/geometry.h geometry/mapobject.cpp

display/display.o: display/display.h display/display.cpp

clean: 
	rm $(objects) runfms
