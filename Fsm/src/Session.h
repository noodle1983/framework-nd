#ifndef SESSION_H
#define SESSION_H

#include "Fsm.h"
#include "State.h"

namespace Fsm
{
    class Session
    {
    public:
        Session(FiniteStateMachine * theFsm)
            : fsmM(theFsm)
            , isInitializedM(false)
        {
            curStateIdM = fsmM->getFirstState();
            endStateIdM = fsmM->getLastState();
        }
        ~Session(){};

        void fsmForword(const int theEventId, const int theEventArg)
        {
            if (!isInitializedM)
            {
                //the first state's entry function
                State& curState = getCurState();
                ActionList& actionList = curState.getActionList(ENTRY_EVT);
                ActionList::iterator it = actionList.begin();
                for (; it != actionList.end(); it++)
                {
                    (*it)(this);
                }
                isInitializedM = true;
            }
            State& curState = getCurState();
            ActionList& actionList = curState.getActionList(theEventId, theEventArg);
            if (actionList.empty())
            {
                //todo ERROR message
                changeState(this, endStateIdM);
                return;
            }
            ActionList::iterator it = actionList.begin();
            for (; it != actionList.end(); it++)
            {
                (*it)(this);
            }

        }

        State& getEndState()
        {
            return fsmM->getState(endStateIdM);
        }

        State& getCurState()
        {
            return fsmM->getState(curStateIdM);
        }

        State& getNextState(const int theNextStateId)
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
