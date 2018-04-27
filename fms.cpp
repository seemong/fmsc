#include <iostream>
#include <libxml/xmlreader.h>
#include <cstring>
#include <math.h>
#include <time.h>
#include <memory>
#include <cassert>
#include "mapobject.h"
#include "display.h"
#include "geofile.h"
#include "mapstore.h"
#include "mapcache.h"
#include "sthread.h"
using namespace std;


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

class Meshes {
public:
    FaceRectangleMesh * face;
    WireRectangleMesh * wire;
};

/**
 * The lookAt function must be called inside redraw
 * because predraw resets the identity matrix each time
 */
void
redraw(Display * display, void * arg) {
    Mapcache * mapcache = (Mapcache *) arg;
    
    static float position[] = {
        -122.5, 44.01, meters_to_arc(2000.0)
    };
    static float eyex = position[0], eyey = position[1], eyez = position[2];
    static float centerx, centery, centerz;
    centerx = eyex;
    centery = 90;
    centerz = eyez;
    static float upx = 0, upy = 0, upz = 1;
    eyey += meters_to_arc(50);

    list<shared_ptr<GeoTile>> geotiles = mapcache->get_tiles(eyex, eyey, eyez);
    if (geotiles.size() == 0) {
        eyey = position[1];
        return;
    }      
    
    for(shared_ptr<GeoTile>& tile : geotiles) {
        shared_ptr<FaceRectangleMesh> mesh = tile->get_face_mesh();

        shared_ptr<float> vertices = mesh->get_vertices();
        int num_vertices = mesh->get_number_of_vertices();
        shared_ptr<float> normals = mesh->get_normals();
        list<IndexStrip> index_list = mesh->get_index_list();
        
        static float earth_color[] = {135/256.0, 67/256.0, 23/256.0};
    
        display->lookAt(eyex, eyey, eyez, centerx, 90, 0, upx, upy, upz);
        display->set_light_position(0, 0, 100, 1);
 
        for(list<IndexStrip>::iterator it = index_list.begin();
            it != index_list.end(); it++) {
            
            shared_ptr<int> indices = it->get_indices();
            int num_indices = it->get_number_of_indices();
         
            display->draw_triangle_strip(vertices, num_vertices, indices, num_indices, 
                normals, earth_color[0], earth_color[1], earth_color[2]);  
        }
    }
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
    // Call display init early before using any open GL function
    Display::Init(argc, argv);
    Display * display = new Display("the display", 0, 0, 800, 800);
    display->create();
       
    shared_ptr<Mapstore> mapstore = 
        shared_ptr<Mapstore>(new Mapstore(argv[1]));
    Mapcache mapcache(mapstore, meters_to_arc(8000), meters_to_arc(4000));
    
    display->set_redraw(redraw, &mapcache);
    display->set_perspective(90, 1, meters_to_arc(10), 500);
    clock_t t = clock();
    for(;;) {
        display->do_event();
        display->post_redisplay();

        clock_t t_new = clock();
	int sleep_time = int(float(t_new - t)/CLOCKS_PER_SEC * 1000);
	if (sleep_time > 0)
	    Sthread::sleep_millis(sleep_time);
        cout << "tick=" << float(t_new - t)/CLOCKS_PER_SEC * 1000 << " ms\n";
        t = t_new;
    }
}
