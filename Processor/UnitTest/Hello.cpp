#include <iostream>
#include <boost/bind.hpp>

#include "BoostProcessor.h"
#include "Log.h"

static int closed = false;
static boost::mutex closedMutexM;
static boost::condition_variable closedCondM;
void sig_stop(int sig)
{
    LOG_DEBUG("receive signal " << sig << ". stopping...");
    boost::lock_guard<boost::mutex> lock(closedMutexM);
    closed = true;
    closedCondM.notify_one();
}


void on_timeout(int theFd, short theEvt, void *theArg)
{
    LOG_WARN("time end");
    delete (struct event*) theArg;
    sig_stop(2);
}

void say(Processor::BoostProcessor* theProcessor)
{
    std::cout << "Hello, I will exit after 1 second" << std::endl;
    struct event* timeoutEvt = event_new(NULL, -1, EV_PERSIST, on_timeout, NULL);
    timeoutEvt->ev_arg = timeoutEvt;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    theProcessor->addLocalTimer(1, tv, timeoutEvt);
    LOG_WARN("time begin");

}

int main()
{
    Processor::BoostProcessor processor(1);
    processor.start();
    processor.process(1, say, &processor);

    boost::unique_lock<boost::mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }
    processor.waitStop();
    return 0;
}

