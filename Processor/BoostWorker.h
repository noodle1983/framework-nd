#ifndef BOOSTWORKER_H
#define BOOSTWORKER_H

#include <boost/function.hpp>
#include <boost/mutex.hpp>
#include <list>

namespace Processor
{
    typedef boost::function<void()> Job;
    typedef std:list<Job*> JobQueue;
    class BoostWorker
    {
    public:
        BoostWorker();
        ~BoostWorker();

        void stop();

        int processJob(Job* theJob);
    private:
        void run();

        JobQueue jobQueueM;
        boost::mutex queueMutexM;
        boost::condition_variable queueCondM;
    };
}

#endif /* BOOSTWORKER_H */

