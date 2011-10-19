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

        static BoostProcessor* getNetInstance();
    private:
        unsigned threadCountM;
        BoostWorker* workersM;
        boost::thread_group threadsM;

        static BoostProcessor* netProcessorM;
    };

}

#endif /* BOOSTPROCESSOR_H */

