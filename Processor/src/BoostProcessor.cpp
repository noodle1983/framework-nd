#include <BoostProcessor.h>
#include <BoostWorker.h>

#include <boost/bind.hpp>

using namespace Processor;

//-----------------------------------------------------------------------------

static boost::mutex fsmProcessorInstanceMutex;
static boost::mutex netProcessorInstanceMutex;
static boost::shared_ptr<BoostProcessor> fsmProcessorInstanceReleaser;
static boost::shared_ptr<BoostProcessor> netProcessorInstanceReleaser;
BoostProcessor* BoostProcessor::fsmProcessorM = NULL;
BoostProcessor* BoostProcessor::netProcessorM = NULL;
//-----------------------------------------------------------------------------

BoostProcessor* BoostProcessor::fsmInstance()
{
    if (NULL == fsmProcessorM)
    {
        boost::lock_guard<boost::mutex> lock(fsmProcessorInstanceMutex);
        if (NULL == fsmProcessorM)
        {
            fsmProcessorM = new BoostProcessor(3);
            fsmProcessorInstanceReleaser.reset(fsmProcessorM);
            fsmProcessorM->start();
        }
    }
    return fsmProcessorM;
}


//-----------------------------------------------------------------------------

BoostProcessor* BoostProcessor::netInstance()
{
    if (NULL == netProcessorM)
    {
        boost::lock_guard<boost::mutex> lock(netProcessorInstanceMutex);
        if (NULL == netProcessorM)
        {
            netProcessorM = new BoostProcessor(3);
            netProcessorInstanceReleaser.reset(netProcessorM);
            netProcessorM->start();
        }
    }
    return netProcessorM;
}

//-----------------------------------------------------------------------------
BoostProcessor::BoostProcessor(const unsigned theThreadCount)
    :threadCountM(theThreadCount), 
     workersM(NULL)
{
}

//-----------------------------------------------------------------------------

BoostProcessor::~BoostProcessor()
{
    threadsM.interrupt_all();
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

