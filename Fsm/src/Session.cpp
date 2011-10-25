#include "Session.h"
#include "Log.h"
#include "BoostProcessor.h"
#include "Reactor.h"

#include <utility>

using namespace Fsm;


//-----------------------------------------------------------------------------

Session::Session(FiniteStateMachine* theFsm, const int theId, void* theData)
    : fsmM(theFsm)
    , isInitializedM(false)
    , dataM(theData)
    , fsmTimeoutEvtM(NULL)
    , fsmProcessorM(NULL)
    , idM(theId)
    , timerIdM(0)
{
    curStateIdM = fsmM->getFirstStateId();
    endStateIdM = fsmM->getLastStateId();
}

//-----------------------------------------------------------------------------

Session::~Session()
{
}

//-----------------------------------------------------------------------------

void Session::handleEvent(const int theEventId)
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
    ActionList& actionList = curState.getActionList(theEventId);
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
    const std::string& preStateName = fsmM->getState(curStateIdM).getName();

    curStateIdM = theNextStateId;
    State& nextState = fsmM->getState(curStateIdM);

    DEBUG("FsmStateChanged:" << preStateName
            << " -> " << nextState.getName());

    return nextState;
}

//-----------------------------------------------------------------------------
typedef std::pair<Session*, int> TimerPair;

void onFsmTimeOut(int theFd, short theEvt, void *theArg)
{
    TimerPair* timerPair = (TimerPair*) theArg;
    Session* session = timerPair->first;
    int timerId = timerPair->second;
    session->asynHandleTimeout(timerId);
    delete timerPair;
}
//-----------------------------------------------------------------------------

void Session::asynHandleTimeout(const int theTimerId)
{
    if (fsmProcessorM)
    {
        fsmProcessorM->process(idM,
            new Processor::Job(boost::bind(&Session::handleTimeout, this, theTimerId)));
    }
    else
    {
        Processor::BoostProcessor::fsmInstance()->process(idM,
            new Processor::Job(boost::bind(&Session::handleTimeout, this, theTimerId)));
    }

}

//-----------------------------------------------------------------------------

void Session::handleTimeout(const int theTimerId)
{
    if (fsmTimeoutEvtM && theTimerId == timerIdM)
    {
        //otherwise it is another timer and the previous one is freed already
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
        handleEvent(TIMEOUT_EVT);
    }
}

//-----------------------------------------------------------------------------

void Session::newSecTimer(const int theSeconds)
{
    if (fsmTimeoutEvtM)
    {
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
    }
    timerIdM++;
    std::pair<Session*,int>* timerPair = new std::pair<Session*, int>(this, timerIdM);
    fsmTimeoutEvtM = Net::Reactor::Reactor::instance()->newTimer(onFsmTimeOut, timerPair);
    struct timeval tv;
    tv.tv_sec = theSeconds;
    tv.tv_usec = 0;
    event_add(fsmTimeoutEvtM, &tv);
}

//-----------------------------------------------------------------------------

void Session::cancelTimer()
{
    if (fsmTimeoutEvtM)
    {
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
    }
}

//-----------------------------------------------------------------------------

