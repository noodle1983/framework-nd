#ifndef SESSION_H
#define SESSION_H

#include "Fsm.h"
#include "State.h"

namespace Fsm
{
    class Session
    {
    public:
        Session(FiniteStateMachine * theFsm);
        ~Session();

        void handleEvent(const int theEventId, const int theEventArg);

        inline State& getEndState()
        {
            return fsmM->getState(endStateIdM);
        }

        inline State& getCurState()
        {
            return fsmM->getState(curStateIdM);
        }

        inline State& getNextState(const int theNextStateId)
        {
            curStateIdM = theNextStateId;
            return fsmM->getState(curStateIdM);
        }

    private:
        int curStateIdM;
        int endStateIdM;
        FiniteStateMachine* fsmM;
        bool isInitializedM;
    };
}

#endif /* SESSION_H */
