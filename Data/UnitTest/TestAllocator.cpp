#include "DataAllocator.hpp"
#include <iostream>
using namespace std;


struct Test
{
	typedef Data::Allocator<Test, 1000 * 1000> Allocator;
	union
	{
		Allocator* allocatorM;
		Test* nextFreeM;
	};
    char reserve[1024];
};

int main()
{
	Test::Allocator allocator;
    Test *t = allocator.newData();
    allocator.freeData(t);
    cout << "type any key to exit: " << endl;
    int i;
    cin >> i;
    cout << endl;
    return 0;
}


