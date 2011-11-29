#include <iostream>
#include <boost/bind.hpp>

#include <BoostProcessor.h>


void say(const char* theWords)
{
    std::cout << theWords << std::endl;
}

int main()
{
    Processor::BoostProcessor::netInstance()->start();
    Processor::BoostProcessor::netInstance()->process(1, say, "Hello");
    sleep(1);
    return 0;
}

