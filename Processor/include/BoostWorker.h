#ifndef BOOSTWORKER_H
#define BOOSTWORKER_H

#include "ProcessorJob.hpp"
#include "KfifoBuffer.h"

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <list>

#include <minheap-internal.h>

namespace Processor
{
    //typedef std::list<IJob*> JobQueue;
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

        inline bool isJobQueueEmpty()
        {return bufferJobQueueM.empty();}
        void stop();

        int process(IJob* theJob);
		void addLocalTimer(const struct timeval& theInterval, struct event* theEvent);
		void cancelLocalTimer(struct event*);

        void run();
    private:
        unsigned groupTotalM;
        unsigned groupIndexM;
		
        //JobQueue jobQueueM;
		Utility::KfifoBuffer bufferJobQueueM;
        boost::mutex queueMutexM;
        boost::mutex nullMutexM;
        boost::condition_variable queueCondM;

		//integrate timer handling
		min_heap_t timerHeapM;
		struct timeval timeNowM;	

#ifdef DEBUG
		int64_t tidM;
#endif
    };
}

#endif /* BOOSTWORKER_H */

