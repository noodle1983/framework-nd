#include <BoostWorker.h>

using namespace Processor;

//-----------------------------------------------------------------------------
BoostWorker::BoostWorker()
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


int BoostWorker::processJob(Job* theJob)
{
    bool jobQueueEmpty = jobQueueM.empty();
    {
        boost::lock_guard<boost::mutex> lock(queueMutexM);
        jobQueueM.push_back(theJob);
    }
    if (jobQueueEmpty)
    {
        queueCondM.notify_one();
    }

}

//-----------------------------------------------------------------------------

void BoostWorker::run()
{
    Job* job;
    while (true)
    {
        {
            boost::unique_lock<boost::mutex> lock(queueMutexM);
            if (jobQueueM.empty())
            {
                queueMutexM.wait(lock);
                if (jobQueueM.empty())
                    return;
            }

            job = jobQueueM.pop_front();
        }
        (*job)();
        delete job;
        job = NULL;
    }
}

//-----------------------------------------------------------------------------


