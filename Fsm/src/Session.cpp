#include "Session.h"

using namespace Fsm;


//-----------------------------------------------------------------------------

Session::Session(FiniteStateMachine * theFsm)
    : fsmM(theFsm)
    , isInitializedM(false)
{
    curStateIdM = fsmM->getFirstState();
    endStateIdM = fsmM->getLastState();
}

//-----------------------------------------------------------------------------

Session::~Session()
{
}

//-----------------------------------------------------------------------------

void Session::handleEvent(const int theEventId, const int theEventArg)
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

