#include "BoostProcessor.h"
#include "BoostWorker.h"
#include "ConfigCenter.h"

#include <boost/bind.hpp>

using namespace Processor;
using namespace Config;

//-----------------------------------------------------------------------------

static boost::mutex fsmProcessorInstanceMutex;
static boost::mutex netProcessorInstanceMutex;
static boost::mutex manProcessorInstanceMutex;
static boost::shared_ptr<BoostProcessor> fsmProcessorInstanceReleaser;
static boost::shared_ptr<BoostProcessor> netProcessorInstanceReleaser;
static boost::shared_ptr<BoostProcessor> manProcessorInstanceReleaser;
BoostProcessor* BoostProcessor::fsmProcessorM = NULL;
BoostProcessor* BoostProcessor::netProcessorM = NULL;
BoostProcessor* BoostProcessor::manProcessorM = NULL;
//-----------------------------------------------------------------------------

BoostProcessor* BoostProcessor::fsmInstance()
{
    if (NULL == fsmProcessorM)
    {
        boost::lock_guard<boost::mutex> lock(fsmProcessorInstanceMutex);
        if (NULL == fsmProcessorM)
        {
            int threadCount = ConfigCenter::instance()->get("prc.fsmTno", 3);
            BoostProcessor* fsmProcessor = new BoostProcessor(threadCount);
            fsmProcessorInstanceReleaser.reset(fsmProcessor);
            fsmProcessor->start();
            fsmProcessorM = fsmProcessor;
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
            int threadCount = ConfigCenter::instance()->get("prc.netTno", 3);
            BoostProcessor* netProcessor = new BoostProcessor(threadCount);
            netProcessorInstanceReleaser.reset(netProcessor);
            netProcessor->start();
            netProcessorM = netProcessor;
        }
    }
    return netProcessorM;
}

//-----------------------------------------------------------------------------

BoostProcessor* BoostProcessor::manInstance()
{
    if (NULL == manProcessorM)
    {
        boost::lock_guard<boost::mutex> lock(manProcessorInstanceMutex);
        if (NULL == manProcessorM)
        {
            int threadCount = ConfigCenter::instance()->get("prc.manTno", 1);
            BoostProcessor* manProcessor = new BoostProcessor(threadCount);
            manProcessorInstanceReleaser.reset(manProcessor);
            manProcessor->start();
            manProcessorM = manProcessor;
        }
    }
    return manProcessorM;
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
    //threadsM.interrupt_all();
    stop();
    delete []workersM;
}

//-----------------------------------------------------------------------------

void BoostProcessor::start()
{
    if (0 == threadCountM)
        return;

    if (NULL != workersM)
        return;

    workersM = new BoostWorker[threadCountM];
    for (unsigned i = 0; i < threadCountM; i++)
    {
        workersM[i].setGroupInfo(threadCountM, i);
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

