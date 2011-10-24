#include "Session.h"
#include "Log.h"

using namespace Fsm;


//-----------------------------------------------------------------------------

Session::Session(FiniteStateMachine* theFsm, void* theData)
    : fsmM(theFsm)
    , isInitializedM(false)
    , dataM(theData)
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
        ERROR("the Event is not defined with id:" << theEventId);
        changeState(this, endStateIdM);
        return;
    }
    ActionList::iterator it = actionList.begin();
    for (; it != actionList.end(); it++)
    {
        (*it)(this);
    }

}

//-----------------------------------------------------------------------------

State& Session::toNextState(const int theNextStateId)
{
    std::string& preStateName = fsmM->getState(curStateIdM).getName();

    curStateIdM = theNextStateId;
    State& nextState = fsmM->getState(curStateIdM);
   
    DEBUG("FsmStateChanged:" << preStateName
            << " -> " << nextState.getName());

    return nextState;
}

