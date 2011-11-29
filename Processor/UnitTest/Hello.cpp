#include <iostream>
#include <boost/bind.hpp>

#include <BoostProcessor.h>


void say(const char* theWords)
{
    std::cout << theWords << std::endl;
}

int main()
{
    Processor::BoostProcessor processor(1);
    processor.start();
    processor.process(1, say, "Hello");
	sleep(1);
    processor.stop();
    return 0;
}

