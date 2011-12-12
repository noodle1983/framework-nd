#ifndef EVENTPOOL_HPP
#define EVENTPOOL_HPP

#include <event.h>
#include <vector>
#include <assert.h>

namespace Data 
{

    class EventPool
    {
    public:        
        typedef struct event EventType;
        typedef std::vector<EventType*> EventCacheVector;
        
        EventPool(unsigned theBatchCount)
            : batchCountM(theBatchCount)
            , freeHeaderM(&eventM)
            , freeTailerM(&eventM)
			, sizeM(0)
			, newCounterM(0)
			, freeCounterM(0)
        {
            if (batchCountM < 2)
            {
                batchCountM = 2;
            }
        }
        ~EventPool()
        {
			sizeM = 0;
            int vectorSize = eventCachesM.size();
            for (int i = 0; i < vectorSize; i++)
            {
                delete[] (eventCachesM[i]);
            }
        }

        EventType* newEvent(
                struct event_base *theBase, 
                evutil_socket_t theFd, 
                short theFlag, 
                event_callback_fn theCallback, 
                void *theArg)
        {
            newCounterM++;
            if (freeHeaderM != freeTailerM)
            {
                EventType* res = freeHeaderM;
                freeHeaderM = (EventType*)res->ev_arg;
                event_assign(res, theBase, theFd, theFlag, theCallback, theArg);
                return res;
            }
            else
            {
                EventType* newEventCache = new EventType[batchCountM]; 
				sizeM += batchCountM;
                for (unsigned long long i = 0; i < batchCountM - 1; i++)
                {
                    newEventCache[i].ev_arg = (void*)&newEventCache[i+1];
                }
                newEventCache[batchCountM-1].ev_arg = (void*)freeHeaderM;
                eventCachesM.push_back(newEventCache);

                freeHeaderM = &newEventCache[1];
                event_assign(newEventCache, theBase, theFd, theFlag, theCallback, theArg);
                return newEventCache;
            }
        }

        void freeEvent(EventType*& theEvent)
        {
            freeCounterM++;
            freeTailerM->ev_arg = (void*)theEvent;
            freeTailerM = theEvent;
            freeTailerM->ev_arg = NULL;
            theEvent = NULL;
        }

		unsigned long long getSize()
		{
			return sizeM;
		}

		unsigned long long getUsed()
		{
			return newCounterM - freeCounterM;
		}
    private:
        EventCacheVector eventCachesM;
        unsigned batchCountM;
        EventType eventM;
        EventType* freeHeaderM;
        EventType* freeTailerM;

		unsigned long long sizeM;
		unsigned long long newCounterM;
		unsigned long long freeCounterM;
    };
}
#endif /* EVENTPOOL_HPP */

