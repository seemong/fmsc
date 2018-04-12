#include <iostream>
#include <libxml/xmlreader.h>
#include <cstring>
#include "geometry/mapobject.h"


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
            } 
            
            
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
    cout << "Hello world!\n";
    
    Map * m = Map::make_map("the-map", "rainier");
    cout << *m << "\n";
    delete m;
    
    cout << "Streaming  " << argv[1] << "\n";
    streamFile(argv[1]);
}
