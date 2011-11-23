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
                const uint64_t theProcessorId,
                const std::string& theLogicName = "");
        virtual ~Session();

        State& toNextState(const int theNextStateId);
		int asynHandleEvent(const int theEventId);
        int handleEvent(const int theEventId);
        void newTimer(const long long theUsec);
        void asynHandleTimeout(const int theTimerId);
        void handleTimeout(const int theTimerId);
        void cancelTimer();

        const uint64_t getSessionId()
        {
            return sessionIdM;
        }

        const std::string& getSessionName()
        {
            return sessionNameM;
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
        int curStateIdM;
        int endStateIdM;
        FiniteStateMachine* fsmM;

        bool isInitializedM;
        struct event* fsmTimeoutEvtM;
        Processor::BoostProcessor* fsmProcessorM;
        uint64_t sessionIdM;
        int timerIdM;

        std::string sessionNameM;

#ifdef DEBUG
		int64_t tidM;
#endif

    };
}

#endif /* SESSION_H */
