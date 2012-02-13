#include "BoostProcessor.h"
#include "BoostWorker.h"
#include "ConfigCenter.h"
#include "ProcessorJob.hpp"
#include "ProcessorSensor.h"

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
			Net::Protocol::ProcessorSensorSingleton::instance()->registProcessor(
					"FsmProcessor", fsmProcessorM);
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
			Net::Protocol::ProcessorSensorSingleton::instance()->registProcessor(
					"NetProcessor", netProcessorM);
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
			Net::Protocol::ProcessorSensorSingleton::instance()->registProcessor(
					"ManProcessor", manProcessorM);
        }
    }
    return manProcessorM;
}

//-----------------------------------------------------------------------------

BoostProcessor::BoostProcessor(const unsigned theThreadCount)
    : threadCountM(theThreadCount)
    , workersM(NULL)
{
}

//-----------------------------------------------------------------------------

BoostProcessor::BoostProcessor(const std::string& theName, const unsigned theThreadCount)
    : threadCountM(theThreadCount)
    , workersM(NULL)
    , nameM(theName)
{
    if (!nameM.empty())
    {
        Net::Protocol::ProcessorSensorSingleton::instance()->registProcessor(
            nameM, this);
    }
}

//-----------------------------------------------------------------------------

BoostProcessor::~BoostProcessor()
{
    //threadsM.interrupt_all();
    if (!nameM.empty())
    {
        Net::Protocol::ProcessorSensorSingleton::instance()->unregistProcessor(nameM);
    }
    if (workersM)
    {
        stop();
        delete []workersM;
    }
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

void BoostProcessor::waitStop()
{
    if (NULL == workersM)
        return;

    unsigned int i = 0;
    while(true)
    {
        /* check the worker once only */
        for (; i < threadCountM && workersM[i].isJobQueueEmpty(); i++)
        {
        }
        if (i == threadCountM)
        {
            break;
        }
        else
        {
            usleep(1);
        }
    }
    threadsM.interrupt_all();
}

//-----------------------------------------------------------------------------

void BoostProcessor::stop()
{
    if (NULL == workersM)
        return;

    for (unsigned i = 0; i < threadCountM; i++)
    {
        workersM[i].stop();
    }
    threadsM.interrupt_all();
    threadsM.join_all();
}

//-----------------------------------------------------------------------------

/*
int BoostProcessor::process(const unsigned long long theId, Job* job)
{
    unsigned workerId = theId % threadCountM;
    return workersM[workerId].process(job);
}
*/

//-----------------------------------------------------------------------------

int BoostProcessor::process(
        const unsigned long long theId, 
        void (*theFunc)())
{
    NullParamJob* job = 
        NullParamJob::AllocatorSingleton::instance()->newData(theId);
    job->init(theFunc);

    unsigned workerId = theId % threadCountM;
    return workersM[workerId].process(job);
}

//-----------------------------------------------------------------------------
