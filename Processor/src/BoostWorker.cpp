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


int BoostWorker::process(Job* theJob)
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
    Job* job;
    while (true)
    {
        {
            boost::unique_lock<boost::mutex> lock(queueMutexM);
            if (jobQueueM.empty())
            {
                queueCondM.wait(lock);
                if (jobQueueM.empty())
                    return;
            }

            job = jobQueueM.front();
            jobQueueM.pop_front();
        }
        (*job)();
        delete job;
        job = NULL;
    }
}

//-----------------------------------------------------------------------------


