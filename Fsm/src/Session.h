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
        Session(FiniteStateMachine * theFsm, const int theId, void* theData);
        ~Session();

        State& toNextState(const int theNextStateId);
        void handleEvent(const int theEventId, const int theEventArg);
        void newSecTimer(const int theSeconds);
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

        template<typename DataType>
        inline DataType* getData() 
        {
            return (DataType*)dataM;
        }

    private:
        int curStateIdM;
        int endStateIdM;
        FiniteStateMachine* fsmM;

        bool isInitializedM;
        void* dataM;
        struct event* fsmTimeoutEvtM;
        Processor::BoostProcessor* fsmProcessorM;
        int idM;
        int timerIdM;
                
    };
}

#endif /* SESSION_H */
