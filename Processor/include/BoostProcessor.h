#ifndef BOOSTPROCESSOR_H
#define BOOSTPROCESSOR_H

#include "ProcessorJob.hpp"
#include "BoostWorker.h"

#include <string>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace Net
{
namespace Protocol
{
	class ProcessorSensor;
}
}

namespace Processor
{
    class BoostWorker;

    class BoostProcessor
    {
    public:
		friend class Net::Protocol::ProcessorSensor;

        BoostProcessor(const unsigned theThreadCount);
        BoostProcessor(const std::string& theName, const unsigned theThreadCount);
        ~BoostProcessor();

        static BoostProcessor* fsmInstance();
        static BoostProcessor* netInstance();
        static BoostProcessor* manInstance();

        void start();
        void waitStop();
        void stop();

		struct event* addLocalTimer(
                const unsigned long long theId, 
				const struct timeval& theInterval, 
				event_callback_fn theCallback,
				void* theArg)
        {
            unsigned workerId = theId % threadCountM;
            return workersM[workerId].addLocalTimer(theInterval, theCallback, theArg);
        }
		inline void cancelLocalTimer(
                const unsigned long long theId, 
                struct event*& theEvent)
        {
            unsigned workerId = theId % threadCountM;
            return workersM[workerId].cancelLocalTimer(theEvent);
        }

        int process(
                const unsigned long long theId, 
                void (*theFunc)());

        template<typename ParamType1>
        int process(
                const unsigned long long theId, 
                void (*theFunc)(ParamType1),
                ParamType1 theParam);
        
        template<typename ClassType>
        int process(
                const unsigned long long theId, 
                void (ClassType::*theFunc)(),
                ClassType*const theObj);

        template<typename ClassType>
        int process(
                const unsigned long long theId, 
                void (ClassType::*theFunc)(),
                boost::shared_ptr<ClassType> theObj);

        template<typename ClassType, typename ParamType1>
        int process(
                const unsigned long long theId, 
                void (ClassType::*theFunc)(ParamType1),
                ClassType* theObj,
                ParamType1 theParam);

        template<typename ClassType, 
                 typename ParamType1,
                 typename ParamType2>
        int process(
                const unsigned long long theId, 
                void (ClassType::*theFunc)(ParamType1, ParamType2),
                ClassType* theObj,
                ParamType1 theParam1,
                ParamType2 theParam2);
    private:
        unsigned threadCountM;
        BoostWorker* workersM;
        boost::thread_group threadsM;
        std::string nameM;

        static BoostProcessor* fsmProcessorM;
        static BoostProcessor* netProcessorM;
        static BoostProcessor* manProcessorM;
    };
	
        
//-----------------------------------------------------------------------------

	template<typename ParamType1>
	int BoostProcessor::process(
			const unsigned long long theId, 
			void (*theFunc)(ParamType1),
			ParamType1 theParam)
	{
		OneParamJob<ParamType1>* job = 
			OneParamJob<ParamType1>::AllocatorSingleton::instance()->newData(theId);
		job->init(theFunc, theParam);

		unsigned workerId = theId % threadCountM;
		return workersM[workerId].process(job);
	}
	
//-----------------------------------------------------------------------------

    template<typename ClassType>
    int BoostProcessor::process(
            const unsigned long long theId, 
            void (ClassType::*theFunc)(),
            ClassType*const theObj)
    {
		NullParamClassJob<ClassType>* job = 
			NullParamClassJob<ClassType>::AllocatorSingleton::instance()->newData(theId);
		job->init(theFunc, theObj);

		unsigned workerId = theId % threadCountM;
		return workersM[workerId].process(job);
    }
	
//-----------------------------------------------------------------------------

    template<typename ClassType>
    int BoostProcessor::process(
            const unsigned long long theId, 
            void (ClassType::*theFunc)(),
            boost::shared_ptr<ClassType> theObj)
    {
		NullParamClassEJob<ClassType>* job = 
			NullParamClassEJob<ClassType>::AllocatorSingleton::instance()->newData(theId);
		job->init(theFunc, theObj);

		unsigned workerId = theId % threadCountM;
		return workersM[workerId].process(job);
    }
	
//-----------------------------------------------------------------------------

    template<typename ClassType, typename ParamType1>
    int BoostProcessor::process(
            const unsigned long long theId, 
            void (ClassType::*theFunc)(ParamType1),
            ClassType* theObj,
            ParamType1 theParam)
    {
		OneParamClassJob<ClassType, ParamType1>* job = 
			OneParamClassJob<ClassType, ParamType1>::AllocatorSingleton::instance()->newData(theId);
		job->init(theFunc, theObj, theParam);

		unsigned workerId = theId % threadCountM;
		return workersM[workerId].process(job);
    }

	
//-----------------------------------------------------------------------------
    template<typename ClassType, 
             typename ParamType1,
             typename ParamType2>
    int BoostProcessor::process(
            const unsigned long long theId, 
            void (ClassType::*theFunc)(ParamType1, ParamType2),
            ClassType* theObj,
            ParamType1 theParam1,
            ParamType2 theParam2)
    {
		TwoParamClassJob<ClassType, ParamType1, ParamType2>* job = 
			TwoParamClassJob<ClassType, ParamType1, ParamType2>::AllocatorSingleton::instance()->newData(theId);
		job->init(theFunc, theObj, theParam1, theParam2);

		unsigned workerId = theId % threadCountM;
		return workersM[workerId].process(job);

    }

}

#endif /* BOOSTPROCESSOR_H */

