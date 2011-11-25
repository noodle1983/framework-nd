#include "DataAllocator.hpp"
#include <iostream>
using namespace std;


struct Test
{
    Test* nextFreeM;
    char reserve[1024];
};

int main()
{
    Data::Allocator<Test, 1000 * 1000> allocator;
    Test *t = allocator.newData();
    allocator.freeData(t);
    cout << "type any key to exit: " << endl;
    int i;
    cin >> i;
    cout << endl;
    return 0;
}


