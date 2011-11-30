#ifndef SESSION_H
#define SESSION_H

#include "Fsm.h"
#include "State.h"

#include <event.h>
#include <stdint.h>
#include <string>

namespace Processor
{
    class BoostProcessor;
}

namespace Fsm
{
    class Session
    {
    public:
        Session(
                FiniteStateMachine* theFsm, 
                const uint64_t theSessionId);
        Session();
        void init(
                FiniteStateMachine* theFsm, 
                const uint64_t theSessionId);
        virtual ~Session();


        State& toNextState(const int theNextStateId);
		int asynHandleEvent(const int theEventId);
        void handleEvent(const int theEventId);
        void newTimer(const long long theUsec);
        void asynHandleTimeout(const unsigned char theTimerId);
        void handleTimeout(const unsigned char theTimerId);
        void cancelTimer();


        const uint64_t getSessionId()
        {
            return sessionIdM;
        }

        virtual const char* getSessionName()
        {
            return "Session";
        }

        const std::string& getEventName(const int theEventName)
        {
            return fsmM->getEventName(theEventName);
        }

        inline State& getEndState()
        {
            return fsmM->getState(endStateIdM);
        }

        inline State& getCurState()
        {
            return fsmM->getState(curStateIdM);
        }

    protected:
        bool isInitializedM;
        unsigned char curStateIdM;
        unsigned char endStateIdM;
        unsigned timerIdM;
        FiniteStateMachine* fsmM;

        struct event* fsmTimeoutEvtM;
        uint64_t sessionIdM;

#ifdef DEBUG
		int64_t tidM;
#endif

    };
}

#endif /* SESSION_H */
