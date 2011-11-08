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
		void asynHandleEvent(const int theEventId);
        void handleEvent(const int theEventId);
        void newTimer(const long long theUsec);
        void asynHandleTimeout(const int theTimerId);
        void handleTimeout(const int theTimerId);
        void cancelTimer();


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
        uint64_t processorIdM;
        int timerIdM;

        std::string logicNameM;

    };
}

#endif /* SESSION_H */
