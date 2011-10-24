#include "Fsm.h"

using namespace Fsm;
//-----------------------------------------------------------------------------

FiniteStateMachine& FiniteStateMachine::operator+=(const State& theState)
{
    int stateId = theState.getId();
    if (STATE_NONE == initStateIdM)
    {
        initStateIdM = stateId;
    }
    endStateIdM = stateId;

    statesM[theState.getId()] = theState;
    return *this;
}

//-----------------------------------------------------------------------------

FiniteStateMachine& FiniteStateMachine::operator+=(const Event& theEvent)
{
    State& curState = statesM[endStateIdM];
    curState.addEvent(theEvent);
    return *this;
}

//-----------------------------------------------------------------------------




