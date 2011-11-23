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
        void setGroupInfo(const unsigned theTotal, const unsigned theIndex)
        {
            groupTotalM = theTotal;
            groupIndexM = theIndex;
        }

        void stop();

        int process(Job* theJob);

        void run();
    private:
        unsigned groupTotalM;
        unsigned groupIndexM;
        JobQueue jobQueueM;
        boost::mutex queueMutexM;
        boost::condition_variable queueCondM;
    };
}

#endif /* BOOSTWORKER_H */

