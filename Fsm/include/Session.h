#ifndef SESSION_H
#define SESSION_H

#include "Fsm.h"
#include "State.h"

#include <event.h>

namespace Processor
{
    class BoostProcessor;
}

namespace Fsm
{
    class Session
    {
    public:
        Session(FiniteStateMachine* theFsm, const int theProcessorId);
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
        int processorIdM;
        int timerIdM;

    };
}

#endif /* SESSION_H */
