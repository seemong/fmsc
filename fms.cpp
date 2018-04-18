#include <iostream>
#include <libxml/xmlreader.h>
#include <cstring>
#include <math.h>
#include <time.h>
#include <memory>
#include "mapobject.h"
#include "display.h"
#include "geofile.h"


int streamFile(char *filename) {
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlNewTextReaderFilename(filename);
    if (reader != NULL) {
        bool save = false;
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            // process the node here
            int nodeType = xmlTextReaderNodeType(reader);
            char * name = (char *) xmlTextReaderName(reader);

            if (nodeType == XML_READER_TYPE_ELEMENT &&
                strcmp("node", name) == 0) {
                cout << "Node starts\n";
            } else if (nodeType == XML_READER_TYPE_ELEMENT &&
                strcmp("way", name) == 0) {
                cout << "Way starts\n";
            } else if (nodeType == XML_READER_TYPE_ELEMENT &&
                strcmp("nd", name) == 0) {
                cout << "nd starts\n";
            } else if (nodeType == XML_READER_TYPE_END_ELEMENT &&
                strcmp("way", name) == 0) {
                cout << "Way ends\n";
            }

            xmlFree(name);

            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            cerr << filename << " : failed to parse\n";
        }
    } else {
        cerr << "Unable to open " << filename << "\n";
    }
}

void
redraw(Display * display, void * arg) {
    static int i = 0;
    static float radius = 20;
    static float theta = 0;

    static float x = -30;
    static float y = 90;
    static float z = 50;
    display->lookAt(x, y, z, 0, 0, 0, 0, 1, 0);
    theta += 0.1;
    z -= 0.1;
    x += 0.1;
    y -= 0.2;

    display->draw_solid_sphere(3, 10, 10, -3, 0, 0, 1, 0, 0);
    display->draw_solid_cube(3, 3, 0, 0, 0, 1, 0);
    display->draw_wire_sphere(2, 10, 10, 0, -2, 0, 0, 0, 1);
}

int
main(int argc, char * argv[]) {
#if 0
    cout << "Hello world!\n";

    Map * m = Map::make_map("the-map", "rainier");
    cout << *m << "\n";
    delete m;

    cout << "Streaming  " << argv[1] << "\n";
    streamFile(argv[1]);
#endif
#if 0
    Display * display = new Display("the display", 0, 0, 800, 800);
    display->create(argc, argv);
    display->set_redraw(redraw, 0);
    display->set_perspective(90, 1, 0.001, 500);
    display->set_light_position(5, 5, 0, 0);

    clock_t t = clock();
    for(;;) {

        // d->set_light_position(eyex, eyey, 0, 0);

        display->do_event();
        display->post_redisplay();

        clock_t t_new = clock();
        cout << "tick=" << float(t_new - t)/CLOCKS_PER_SEC * 1000 << " ms\n";
        t = t_new;
    }
#endif
    shared_ptr<GeoFile> g(new GeoFile(argv[1]));
    shared_ptr<float> data = g->read_data(0, 0, 10, 10);
    for(int i = 0; i < 100; i++)
        cout << data.get()[i] << " ";
    cout << "\n";
}
