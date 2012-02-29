#include "BoostWorker.h"
#include "Log.h"

#include <assert.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace Processor;

boost::thread_specific_ptr<unsigned> g_threadGroupTotal;
boost::thread_specific_ptr<unsigned> g_threadGroupIndex;

#ifdef DEBUG
#include <assert.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)
#endif
//-----------------------------------------------------------------------------
BoostWorker::BoostWorker()
    : groupTotalM(0)
    , groupIndexM(-1)
    , bufferJobQueueM(25) //(32M/8) jobs Max
	, eventPoolM(1000)
    , isToStopM(false)
{
    min_heap_ctor(&timerHeapM);	
    timeNowM.tv_sec = 0;
    timeNowM.tv_usec = 0;

#ifdef DEBUG 
    tidM = -1;
#endif
}

//-----------------------------------------------------------------------------

BoostWorker::~BoostWorker()
{
    min_heap_dtor(&timerHeapM);	
}

//-----------------------------------------------------------------------------

void BoostWorker::stop()
{
    isToStopM = true;
    queueCondM.notify_one();
}

//-----------------------------------------------------------------------------


int BoostWorker::process(IJob* theJob)
{
    bool jobQueueEmpty = false;
    {
        boost::lock_guard<boost::mutex> lock(queueMutexM);
        //jobQueueEmpty = jobQueueM.empty();
        //jobQueueM.push_back(theJob);
        jobQueueEmpty = bufferJobQueueM.empty();
        unsigned len = bufferJobQueueM.putn((char*)&theJob, sizeof(IJob*));
        assert(len > 0);
    }
    if (jobQueueEmpty)
    {
        queueCondM.notify_one();
    }
    return 0;
}

//-----------------------------------------------------------------------------

struct event* BoostWorker::addLocalTimer(
        const struct timeval& theInterval, 
		event_callback_fn theCallback,
		void* theArg)
{
#ifdef DEBUG
    extern boost::thread_specific_ptr<unsigned> g_threadGroupTotal;
    extern boost::thread_specific_ptr<unsigned> g_threadGroupIndex;
    unsigned threadCount = *g_threadGroupTotal.get();
    unsigned threadIndex = *g_threadGroupIndex.get();
    if (threadIndex != groupIndexM || threadCount != threadCount)
    {
        LOG_FATAL("job is handled in wrong thread:" << threadIndex 
                << ", count:" << threadCount
                << ", worker's index:" << groupIndexM
                << ", worker's groupCount:" << groupTotalM);
        assert(false);
    }
    

    if (-1 == tidM)
    {
        tidM = gettid();
    }
    else if (tidM != gettid())
    {
        LOG_FATAL("tid not match pre:" << tidM << "-now:" << gettid());
        assert(false);
    }
#endif

	bool timerHeapEmpty = false;
    //if it is the first one, get the time of now
    //else reuse the one in run()
    if ((timerHeapEmpty = min_heap_empty(&timerHeapM)))
    {
        evutil_gettimeofday(&timeNowM, NULL);
    }
    if (1000000 > min_heap_size(&timerHeapM))
    {
        min_heap_reserve(&timerHeapM, 1000000);
    }

	struct event* timeoutEvt = eventPoolM.newEvent(NULL, -1, 0, theCallback, theArg);
    timeoutEvt->ev_timeout.tv_usec = timeNowM.tv_usec + theInterval.tv_usec;
    timeoutEvt->ev_timeout.tv_sec = timeNowM.tv_sec 
        + theInterval.tv_sec 
        + timeoutEvt->ev_timeout.tv_usec/1000000;
    timeoutEvt->ev_timeout.tv_usec %= 1000000;

    if (-1 == min_heap_push(&timerHeapM, timeoutEvt))
    {
        LOG_FATAL("not enough memory!");
        exit(-1);
    }
	if (timerHeapEmpty)
	{
        queueCondM.notify_one();
	}
	return timeoutEvt;
}

//-----------------------------------------------------------------------------

void BoostWorker::cancelLocalTimer(struct event*& theEvent)
{
    min_heap_erase(&timerHeapM, theEvent);
	eventPoolM.freeEvent(theEvent);
	theEvent = NULL;
}


//-----------------------------------------------------------------------------

void BoostWorker::run()
{
    g_threadGroupTotal.reset(new unsigned(groupTotalM));
    g_threadGroupIndex.reset(new unsigned(groupIndexM));
    IJob* job;
    while (!isToStopM)
    {
        /*
        {
            boost::unique_lock<boost::mutex> lock(queueMutexM);
            while (jobQueueM.empty())
            {
                queueCondM.wait(lock);
            }

            job = jobQueueM.front();
            jobQueueM.pop_front();
        }
        */
        //handle Job
        if (0 < bufferJobQueueM.getn((char*)&job, sizeof(IJob*)))
        {
            (*job)();
            job->returnToPool();
            job = NULL;
        }

        //handle timer
        if (!min_heap_empty(&timerHeapM))
        {
            evutil_gettimeofday(&timeNowM, NULL);
            while(!min_heap_empty(&timerHeapM)) 
            {
                struct event* topEvent = min_heap_top(&timerHeapM);
                if (evutil_timercmp(&topEvent->ev_timeout, &timeNowM, <=))
                {
                    min_heap_pop(&timerHeapM);
                    (topEvent->ev_callback)(-1, 0, topEvent->ev_arg);
					eventPoolM.freeEvent(topEvent);
                }
                else
                {
                    break;
                }
            } 
        }

		if (!bufferJobQueueM.empty())
		{
			continue;
		}
        else if (!isToStopM && bufferJobQueueM.empty() && !min_heap_empty(&timerHeapM))
        {
            boost::unique_lock<boost::mutex> queueLock(queueMutexM);
            queueCondM.timed_wait(queueLock, 
                    boost::posix_time::from_time_t(timeNowM.tv_sec) 
                        + boost::posix_time::microseconds(timeNowM.tv_usec + 500));
        }
        else
        {
            boost::unique_lock<boost::mutex> queueLock(queueMutexM);
            while (!isToStopM && bufferJobQueueM.empty() && min_heap_empty(&timerHeapM))
            {
                queueCondM.wait(queueLock); 
            }
        }
    }
}

