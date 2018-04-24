#include <iostream>
#include <libxml/xmlreader.h>
#include <cstring>
#include <math.h>
#include <time.h>
#include <memory>
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

float eyex, eyey, eyez;
float centerx, centery, centerz;

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
    Meshes * meshes = (Meshes *) arg;
    
    FaceRectangleMesh * m = meshes->face;
    shared_ptr<float> vertices = m->get_vertices();
    int num_vertices = m->get_number_of_vertices();
    shared_ptr<float> normals = m->get_normals();
    list<IndexStrip> index_list = m->get_index_list();
    
    static float earth_color[] = {135/256.0, 67/256.0, 23/256.0};
    
    //printf("Draw mesh with %d vertices\n", num_vertices);
    
    static float theta = 0;
    eyey = eyey + meters_to_arc(5);
    if (eyey > 48)
        eyey = 47;
        
    theta += 0.1;
    
    display->lookAt(eyex, eyey, eyez, centerx, 90, 0, 0, 0, 1);
    shared_ptr<VBO> vertex_vbo = m->get_vertex_vbo();
    shared_ptr<VBO> normals_vbo = m->get_normals_vbo();
    for(list<IndexStrip>::iterator it = index_list.begin();
        it != index_list.end(); it++) {
            
        shared_ptr<int> indices = it->get_indices();
        int num_indices = it->get_number_of_indices();
            /*
        display->draw_triangle_strip(vertices, num_vertices, indices, num_indices, 
            normals, earth_color[0], earth_color[1], earth_color[2]);  
            */
        display->draw_triangle_strip_vbo(vertex_vbo, indices, num_indices, 
            normals_vbo, earth_color[0], earth_color[1], earth_color[2]);
    }
    /*
    WireRectangleMesh * wire = meshes->wire;
    index_list = wire->get_index_list();
    for(list<IndexStrip>::iterator it = index_list.begin();
        it != index_list.end(); it++) {
        shared_ptr<int> indices = it->get_indices();
        int num_indices = it->get_number_of_indices();

        display->draw_lines(vertices, num_vertices, indices, num_indices, 
            normals, 0.1, 0, 0, 2);  
    }
    */
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
    GeoTile tile = g->read_data_as_tile(0, 0, 1200, 1200);
    shared_ptr<float> v = tile.get_vertices();
    for(int i = 0; i < tile.get_xsize() * tile.get_ysize(); i++) {
        // printf("(%f, %f, %f) ", v.get()[3*i], v.get()[3*i+1], v.get()[3*i+2]);
    }
    
    float min_x = tile.get_left();
    float min_y = tile.get_bottom();
    float max_x = tile.get_right();
    float max_y = tile.get_top();
    
    centerx = (min_x + max_x) / 2;
    centery = (min_y + max_y) / 2;
    centerz = 0.0;
    
    eyex = (min_x + max_x)/2;
    eyey = min_y;
    eyez = meters_to_arc(2000);
    
    printf("eyex=%f, eyey=%f, eyez=%f\n", eyex, eyey, eyez);
    printf("cenx=%f, ceny=%f, cenz=%f\n", centerx, centery, 0.0);
    
    list<IndexStrip> is = make_faces(5, 3);
    for(list<IndexStrip>::iterator it = is.begin(); it != is.end(); it++) {
        shared_ptr<int> indices = it->get_indices();
        int num_indices = it->get_number_of_indices();
        for(int i = 0; i < num_indices; i++) {
            // printf("%d ", indices.get()[i]);
        }
        // printf("\n");
    }
    
    FaceRectangleMesh face(v, tile.get_xsize(), tile.get_ysize());
    printf("Normals:\n");
    shared_ptr<float> n = face.get_normals();
    for(int i = 0; i < face.get_number_of_normals(); i++) {
        // printf("(%f, %f, %f) ", n.get()[3*i], n.get()[3*i+1], n.get()[3*i+2]);
    }
    // printf("\n");
    
    WireRectangleMesh wire(v, tile.get_xsize(), tile.get_ysize());
    
    Meshes meshes;
    meshes.wire = &wire;
    meshes.face = &face;
    
    Display * display = new Display("the display", 0, 0, 800, 800);
    display->create(argc, argv);
    display->set_redraw(redraw, &meshes);
    display->set_perspective(90, 1, meters_to_arc(10), 500);
    // display->lookAt(15, 15, 15, 0, 0, 0, 0, 0, 1);
    /*
    display->set_ortho(tile.get_left(), tile.get_right(), 
        tile.get_bottom(), tile.get_top(), -5000, 5000);
        */
    // display->set_light_position(5, 5, 5, 1);

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
