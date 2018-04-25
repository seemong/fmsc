#include <iostream>
#include "mapstore.h"
using namespace std;

int
main(int argc, char ** argv) {
        cout << "Hello World" << endl;
        list<string> l;
        l.push_back("/usr/local/google/home/seemongt/fms/data");
        Mapstore m(l);
        cout << "Goodbye world" << endl;
}
