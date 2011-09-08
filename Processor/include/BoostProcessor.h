#ifndef BOOSTPROCESSOR_H
#define BOOSTPROCESSOR_H

#include <boost/thread.hpp>

namespace Processor
{
    class BoostWorker;
    typedef boost::function<void ()> Job;
    class BoostProcessor
    {
    public:
        BoostProcessor(const unsigned theThreadCount);
        ~BoostProcessor();

        void start();
        void stop();

        int process(const unsigned theId, Job* job);

    private:
        unsigned threadCountM;
        BoostWorker* workersM;
        boost::thread_group threadsM;
    };

}

#endif /* BOOSTPROCESSOR_H */

