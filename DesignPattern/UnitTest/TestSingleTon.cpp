#include <Singleton.hpp>
#include <assert.h>

class A{};

typedef DesignPattern::Singleton<A, 0> TheA;
typedef DesignPattern::Singleton<A, 1> AnotherA;

int main()
{
    assert(TheA::instance() != AnotherA::instance());
    return 0;
}


