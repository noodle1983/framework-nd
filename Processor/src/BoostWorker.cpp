#include <BoostWorker.h>

using namespace Processor;

boost::thread_specific_ptr<unsigned> g_threadGroupTotal;
boost::thread_specific_ptr<unsigned> g_threadGroupIndex;
//-----------------------------------------------------------------------------
BoostWorker::BoostWorker()
    : groupTotalM(0)
    , groupIndexM(-1)
{

}

//-----------------------------------------------------------------------------

BoostWorker::~BoostWorker()
{

}

//-----------------------------------------------------------------------------

void BoostWorker::stop()
{

}

//-----------------------------------------------------------------------------


int BoostWorker::process(IJob* theJob)
{
    bool jobQueueEmpty = false;
    {
        boost::lock_guard<boost::mutex> lock(queueMutexM);
        jobQueueEmpty = jobQueueM.empty();
        jobQueueM.push_back(theJob);
    }
    if (jobQueueEmpty)
    {
        queueCondM.notify_one();
    }
    return 0;
}

//-----------------------------------------------------------------------------

void BoostWorker::run()
{
    g_threadGroupTotal.reset(new unsigned(groupTotalM));
    g_threadGroupIndex.reset(new unsigned(groupIndexM));
    IJob* job;
    while (true)
    {
        {
            boost::unique_lock<boost::mutex> lock(queueMutexM);
            while (jobQueueM.empty())
            {
                queueCondM.wait(lock);
            }

            job = jobQueueM.front();
            jobQueueM.pop_front();
        }
        (*job)();
        job->returnToPool();
        job = NULL;
    }
}

//-----------------------------------------------------------------------------


