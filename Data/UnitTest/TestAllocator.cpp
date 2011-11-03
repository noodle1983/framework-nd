#include "DataAllocator.h"
#include <iostream>
using namespace std;


struct Test
{
    Test()
    {
        cout << "Test Construct" << endl;
    }

    ~Test()
    {
        cout << "Test Destruct" << endl;
    }
};

int main()
{
    Data::Allocator<Test> allocator;
    Test *t = allocator.newData();
    allocator.freeData(t);
    return 0;
}


