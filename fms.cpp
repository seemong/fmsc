#include <iostream>
#include <libxml/xmlreader.h>
#include <cstring>
#include <math.h>
#include <time.h>
#include <memory>
#include <stdio.h>
#include "mapobject.h"
#include "display.h"
#include "geofile.h"
#include "mesh.h"


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
    /*
    static int i = 0;
    static float radius = 20;
    static float theta = 0;

    static float x = -50;
    static float y = 100;
    static float z = 50;
    display->lookAt(x, y, z, 0, 0, 0, 0, 1, 0);
    theta += 0.1;
    z -= 0.1;
    x += 0.1;
    y -= 0.2;

    display->draw_solid_sphere(3, 10, 10, -3, 0, 0, 1, 0, 0);
    display->draw_solid_cube(3, 3, 0, 0, 0, 1, 0);
    display->draw_wire_sphere(2, 10, 10, 0, -2, 0, 0, 0, 1);
    */
    
    Mesh * m = (Mesh *) arg;
    shared_ptr<float> vertices = m->get_vertices();
    int num_vertices = m->get_number_of_vertices();
    shared_ptr<float> normals = m->get_normals();
    shared_ptr<int> indices = m->get_indices();
    int num_indices = m->get_number_of_indices();
    
    display->draw_lines(vertices, num_vertices, indices, num_indices,
        normals, 1, 0, 0, 2);
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

    shared_ptr<GeoFile> g(new GeoFile(argv[1]));
    GeoTile tile = g->read_data_as_tile(0, 0, 5, 3);
    shared_ptr<float> v = tile.get_vertices();
    for(int i = 0; i < tile.get_xsize() * tile.get_ysize(); i++) {
        printf("(%f, %f, %f) ", v.get()[3*i], v.get()[3*i+1], v.get()[3*i+2]);
    }
    cout << "\n";
    
    WireRectangleMesh mesh(v, tile.get_xsize(),
        tile.get_ysize());
    shared_ptr<int> in = mesh.get_indices();
    printf("Indices:\n");
    for(int i = 0; i < mesh.get_number_of_indices(); i++) {
        printf("(%d) ", in.get()[i]);
    }
    printf("\n");
    
    printf("Normals:\n");
    shared_ptr<float> n = mesh.get_normals();
    for(int i = 0; i < mesh.get_number_of_normals(); i++) {
        printf("(%f, %f, %f) ", n.get()[3*i], n.get()[3*i+1], n.get()[3*i+2]);
    }
    printf("\n");

    Display * display = new Display("the display", 0, 0, 800, 800);
    display->create(argc, argv);
    display->set_redraw(redraw, &mesh);
    // display->set_perspective(90, 1, 0.001, 500);
    display->set_ortho(tile.get_left(), tile.get_right(), 
        tile.get_bottom(), tile.get_top(), -5000, 5000);
    display->set_light_position(5, 5, 5, 0);

    clock_t t = clock();
    for(;;) {

        // d->set_light_position(eyex, eyey, 0, 0);

        display->do_event();
        display->post_redisplay();

        clock_t t_new = clock();
        cout << "tick=" << float(t_new - t)/CLOCKS_PER_SEC * 1000 << " ms\n";
        t = t_new;
    }
}
