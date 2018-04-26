#include <stdio.h>
#include <iostream>
#include <memory>
#include "mapstore.h"
using namespace std;

class Foo {
    int x;
    
public:
    Foo(int a) : x(a) {
    }
    ~Foo() {
        cout << "Deleting Foo(" << x << ")" << endl;
    }
};

int
main(int argc, char ** argv) {
    cout << "Hello World" << endl;
    list<shared_ptr<Foo>> l;
    {
        shared_ptr<Foo> a(new Foo(1));
        shared_ptr<Foo> b(new Foo(2));
        l.push_back(a);
        l.push_back(b);
    }
    
    cout << "Goodbye world" << endl;
}
