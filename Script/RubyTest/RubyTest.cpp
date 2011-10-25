#include "RubyInterpreter.h"
#include "TestStruct.h"

using namespace Script::Ruby;

int main()
{
    RubyInterpreter in;
    in.init();
    in.execute("print \"This is C++ call Ruby print funtion!\n\"");
    in.executeFile("test.rb");
    Init_TestStruct();
    in.fini();
    return 0;
}


