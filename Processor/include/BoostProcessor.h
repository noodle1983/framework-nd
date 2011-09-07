#ifndef BOOSTPROCESSOR_H
#define BOOSTPROCESSOR_H

#include <boost/thread.hpp>

#include <BoostWorker.h>

namespace Processor
{
    class BoostWorker;
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

