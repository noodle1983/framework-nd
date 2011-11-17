#include "Session.h"
#include "Log.h"
#include "BoostProcessor.h"
#include "Reactor.h"

#include <utility>

using namespace Fsm;


//-----------------------------------------------------------------------------

Session::Session(
        FiniteStateMachine* theFsm, 
        const uint64_t theProcessorId,
        const std::string& theLogicName)
    : fsmM(theFsm)
    , isInitializedM(false)
    , fsmTimeoutEvtM(NULL)
    , fsmProcessorM(NULL)
    , sessionIdM(theProcessorId)
    , timerIdM(0)
    , sessionNameM(theLogicName)
{
    curStateIdM = fsmM->getFirstStateId();
    endStateIdM = fsmM->getLastStateId();
}

//-----------------------------------------------------------------------------

Session::~Session()
{
}

//-----------------------------------------------------------------------------

void Session::asynHandleEvent(const int theEventId)
{
    if (fsmProcessorM)
    {
        fsmProcessorM->process(sessionIdM,
            new Processor::Job(boost::bind(&Session::handleEvent, this, theEventId)));
    }
    else
    {
        Processor::BoostProcessor::fsmInstance()->process(sessionIdM,
            new Processor::Job(boost::bind(&Session::handleEvent, this, theEventId)));
    }

}

//-----------------------------------------------------------------------------

void Session::handleEvent(const int theEventId)
{
    if (!isInitializedM)
    {
        //the first state's entry function
        const int curStateId = curStateIdM;
        State& curState = getCurState();
        ActionList& actionList = curState.getActionList(ENTRY_EVT);
        ActionList::iterator it = actionList.begin();
        if (it != actionList.end())
        {
            DEBUG(getSessionName() 
                    << "[" << getSessionId() << "] handleEvent("
                    << getEventName(ENTRY_EVT) << ")");
            for (; it != actionList.end(); it++)
            {
                if (curStateId != curStateIdM)
                {
                    DEBUG("state changed, ignore rest action for event:" << theEventId);
                    break;
                }
                (*it)(this);
            }
        }
        isInitializedM = true;
    }

    DEBUG(getSessionName() 
            << "[" << getSessionId() << "] handleEvent("
            << getEventName(theEventId) << ")");
    State& curState = getCurState();
    ActionList& actionList = curState.getActionList(theEventId);
    if (actionList.empty())
    {
        ERROR(getSessionName()
                << "the Event is not defined with id:" << theEventId);
        changeState(this, endStateIdM);
        return;
    }

    const int curStateId = curStateIdM;
    ActionList::iterator it = actionList.begin();
    for (; it != actionList.end(); it++)
    {
        if (curStateId != curStateIdM)
        {
            DEBUG(getSessionName()
                    << " state changed, ignore rest action for event:" << theEventId);
            break;
        }
        (*it)(this);
    }

}

//-----------------------------------------------------------------------------

State& Session::toNextState(const int theNextStateId)
{
    const std::string& preStateName = fsmM->getState(curStateIdM).getName();

    curStateIdM = theNextStateId;
    State& nextState = fsmM->getState(curStateIdM);

    DEBUG( sessionNameM << "[" << sessionIdM << "] " 
            << preStateName << " -> " << nextState.getName());

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
        fsmProcessorM->process(sessionIdM,
            new Processor::Job(boost::bind(&Session::handleTimeout, this, theTimerId)));
    }
    else
    {
        Processor::BoostProcessor::fsmInstance()->process(sessionIdM,
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

void Session::newTimer(const long long theUsec)
{
    if (fsmTimeoutEvtM)
    {
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
    }
    timerIdM++;
    std::pair<Session*,int>* timerPair = new std::pair<Session*, int>(this, timerIdM);
    fsmTimeoutEvtM = Net::Reactor::Reactor::instance()->newTimer(onFsmTimeOut, timerPair);
    struct timeval tv;
    tv.tv_sec = theUsec/1000000;
    tv.tv_usec = theUsec%1000000;
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

