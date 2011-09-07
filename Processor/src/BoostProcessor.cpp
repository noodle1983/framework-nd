#include <BoostProcessor.h>
#include <BoostWorker.h>

#include <boost/bind.hpp>

using namespace Processor;
//-----------------------------------------------------------------------------
BoostProcessor::BoostProcessor(const unsigned theThreadCount)
    :threadCountM(theThreadCount), 
     workersM(NULL)
{
}

//-----------------------------------------------------------------------------

BoostProcessor::~BoostProcessor()
{
    delete []workersM;
}

//-----------------------------------------------------------------------------

void BoostProcessor::start()
{
    if (0 == threadCountM)
        return;

    workersM = new BoostWorker[threadCountM];
    for (unsigned i = 0; i < threadCountM; i++)
    {
        threadsM.create_thread(boost::bind(&BoostWorker::run, &workersM[i]));
    }
}

//-----------------------------------------------------------------------------

void BoostProcessor::stop()
{
    threadsM.interrupt_all();
    threadsM.join_all();
}

//-----------------------------------------------------------------------------

int BoostProcessor::process(const unsigned theId, Job* job)
{
    unsigned workerId = theId % threadCountM;
    return workersM[workerId].process(job);
}

//-----------------------------------------------------------------------------

