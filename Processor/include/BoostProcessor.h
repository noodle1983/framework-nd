#ifndef BOOSTPROCESSOR_H
#define BOOSTPROCESSOR_H

#include "ProcessorJob.hpp"
#include "BoostWorker.h"

#include <boost/thread.hpp>

namespace Processor
{
    class BoostWorker;

    class BoostProcessor
    {
    public:
        BoostProcessor(const unsigned theThreadCount);
        ~BoostProcessor();

        static BoostProcessor* fsmInstance();
        static BoostProcessor* netInstance();
        static BoostProcessor* manInstance();

        void start();
        void stop();

        int process(
                const unsigned theId, 
                void (*theFunc)());

        template<typename ParamType1>
        int process(
                const unsigned theId, 
                void (*theFunc)(ParamType1),
                ParamType1 theParam);
        
        template<typename ClassType>
        int process(
                const unsigned theId, 
                void (ClassType::*theFunc)(),
                ClassType* theObj);

        template<typename ClassType, typename ParamType1>
        int process(
                const unsigned theId, 
                void (ClassType::*theFunc)(ParamType1),
                ClassType* theObj,
                ParamType1 theParam);
    private:
        unsigned threadCountM;
        BoostWorker* workersM;
        boost::thread_group threadsM;

        static BoostProcessor* fsmProcessorM;
        static BoostProcessor* netProcessorM;
        static BoostProcessor* manProcessorM;
    };
	
//-----------------------------------------------------------------------------

	template<typename ParamType1>
	int BoostProcessor::process(
			const unsigned theId, 
			void (*theFunc)(ParamType1),
			ParamType1 theParam)
	{
		OneParamJob<ParamType1>* job = 
			OneParamJob<ParamType1>::AllocatorSingleton::instance()->newData();
		job->init(theFunc, theParam);

		unsigned workerId = theId % threadCountM;
		return workersM[workerId].process(job);
	}


}

#endif /* BOOSTPROCESSOR_H */

