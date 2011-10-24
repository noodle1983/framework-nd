#include "Action.h"
#include "Session.h"
#include "State.h"

#include <string>

using namespace Fsm;
//-----------------------------------------------------------------------------

int Fsm::changeState(
        Fsm::Session* theSession, 
        const int theNextStateId)
{
    //exec EXIT FUNC
    {
        State& curState = theSession->getCurState();
        ActionList& actionList = curState.getActionList(EXIT_EVT);
        ActionList::iterator it = actionList.begin();
        for (; it != actionList.end(); it++)
        {
            (*it)(theSession);
        }
    }
    
    //exec ENTRY_EVT
    {
        State& nextState = theSession->toNextState(theNextStateId);
        ActionList& actionList = nextState.getActionList(ENTRY_EVT);
        ActionList::iterator it = actionList.begin();
        for (; it != actionList.end(); it++)
        {
            (*it)(theSession);
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------

int Fsm::generateEvent(
        Fsm::Session* theSession, 
        const int theEventId,
        const int theEventArg)
{
    theSession->handleEvent(theEventId, theEventArg);
    return 0;
}

//-----------------------------------------------------------------------------
