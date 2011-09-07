#ifndef BOOSTWORKER_H
#define BOOSTWORKER_H

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <list>

namespace Processor
{
    typedef boost::function<void ()> Job;
    typedef std::list<Job*> JobQueue;
    class BoostWorker
    {
    public:
        BoostWorker();
        ~BoostWorker();

        void stop();

        int process(Job* theJob);

        void run();
    private:

        JobQueue jobQueueM;
        boost::mutex queueMutexM;
        boost::condition_variable queueCondM;
    };
}

#endif /* BOOSTWORKER_H */

