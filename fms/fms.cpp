#include <iostream>
#include <libxml/xmlreader.h>
#include <cstring>
#include "geometry/mapobject.h"
#include "display/display.h"


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

int main(int argc, char * argv[]) {
#if 0
    cout << "Hello world!\n";
    
    Map * m = Map::make_map("the-map", "rainier");
    cout << *m << "\n";
    delete m;
    
    cout << "Streaming  " << argv[1] << "\n";
    streamFile(argv[1]);
#endif
    Display * d = new Display("the display", 0, 0, 800, 800);
    d->create(argc, argv);
    for(;;);
}
