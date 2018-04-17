OBJECTS = \
	display.o \
	geometry.o \
	mapobject.o \
	fms.o

TARGET = fms

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

CFLAGS = -g -I/usr/include/libxml2

LIBS = -lxml2 -lGL -lglut -lGLU

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) $(CFLAGS) -o fms

fms.o: fms.cpp geometry.h mapobject.h display.h

geometry.o: geometry.h geometry.cpp

mapobject.o: geometry.h mapobject.cpp

display.o: display.h display.cpp

clean:
	rm $(OBJECTS) $(TARGET)
