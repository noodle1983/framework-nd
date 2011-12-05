#include "Session.h"
#include "Log.h"
#include "BoostProcessor.h"
#include "Reactor.h"

#include <utility>

using namespace Fsm;

#ifdef DEBUG
#include <assert.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)
#endif

typedef std::pair<Session*, unsigned char> TimerPair;
//-----------------------------------------------------------------------------

Session::Session(
        FiniteStateMachine* theFsm, 
        const uint64_t theSessionId)
    : isInitializedM(false)
    , curStateIdM(theFsm->getFirstStateId())
    , endStateIdM(theFsm->getLastStateId())
    , timerIdM(0)
    , fsmM(theFsm)
    , fsmTimeoutEvtM(NULL)
    , sessionIdM(theSessionId)
{
#ifdef DEBUG 
    tidM = -1;
#endif

}

//-----------------------------------------------------------------------------

Session::Session()
    : isInitializedM(false)
    , curStateIdM(0)
    , endStateIdM(0)
    , timerIdM(0)
    , fsmM(NULL)
    , fsmTimeoutEvtM(NULL)
    , sessionIdM(0)
{
#ifdef DEBUG 
    tidM = -1;
#endif

}

//-----------------------------------------------------------------------------

void Session::init(
        FiniteStateMachine* theFsm, 
        const uint64_t theSessionId)
{
    fsmM = theFsm;
    sessionIdM = theSessionId;
    isInitializedM = false;
    curStateIdM = fsmM->getFirstStateId();
    endStateIdM = fsmM->getLastStateId();
    timerIdM = 0;
    if (fsmTimeoutEvtM)
    {
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
    }
    fsmTimeoutEvtM = NULL;
#ifdef DEBUG 
    tidM = -1;
#endif
}

//-----------------------------------------------------------------------------

Session::~Session()
{
}

//-----------------------------------------------------------------------------

int Session::asynHandleEvent(const int theEventId)
{
    Processor::BoostProcessor::fsmInstance()->process(sessionIdM,
        &Session::handleEvent, this, theEventId);
    return 0;
}

//-----------------------------------------------------------------------------

void Session::handleEvent(const int theEventId)
{
#ifdef DEBUG
    extern boost::thread_specific_ptr<unsigned> g_threadGroupTotal;
    extern boost::thread_specific_ptr<unsigned> g_threadGroupIndex;
    unsigned threadCount = *g_threadGroupTotal.get();
    unsigned threadIndex = *g_threadGroupIndex.get();
    if ((sessionIdM%threadCount) != threadIndex)
    {
        LOG_FATAL("job is handled in wrong thread:" << threadIndex 
                << ", count:" << threadCount
                << ", id:" << sessionIdM);
        assert(false);
    }
    

    if (-1 == tidM)
    {
        tidM = gettid();
    }
    else if (tidM != gettid())
    {
        LOG_FATAL("tid not match pre:" << tidM << "-now:" << gettid());
        assert(false);
    }
#endif
    if (!isInitializedM)
    {
        //the first state's entry function
        const int curStateId = curStateIdM;
        State& curState = getCurState();
        ActionList& actionList = curState.getActionList(ENTRY_EVT);
        ActionList::iterator it = actionList.begin();
        if (it != actionList.end())
        {
            LOG_DEBUG(getSessionName() 
                    << "[" << getSessionId() << "] handleEvent("
                    << getEventName(ENTRY_EVT) << ")");
            for (; it != actionList.end(); it++)
            {
                if (curStateId != curStateIdM)
                {
                    LOG_DEBUG("state changed, ignore rest action for event:" << theEventId);
                    break;
                }
                (*it)(this);
            }
        }
        isInitializedM = true;
    }

    LOG_DEBUG(getSessionName() 
            << "[" << getSessionId() << "] handleEvent("
            << getEventName(theEventId) << ")");
    State& curState = getCurState();
    ActionList& actionList = curState.getActionList(theEventId);
    if (actionList.empty())
    {
        LOG_ERROR(getSessionName()
                << "[" << getSessionId() << "]"
                << " the Event " << theEventId << " is not defined"
                << " under state:" << curState.getName());
        changeState(this, endStateIdM);
        return ;
    }

    const int curStateId = curStateIdM;
    ActionList::iterator it = actionList.begin();
    for (; it != actionList.end(); it++)
    {
        if (curStateId != curStateIdM)
        {
            LOG_DEBUG(getSessionName()
                    << " state changed, ignore rest action for event:" << theEventId);
            break;
        }
        (*it)(this);
    }
    return ;
}

//-----------------------------------------------------------------------------

State& Session::toNextState(const int theNextStateId)
{
    const std::string& preStateName = fsmM->getState(curStateIdM).getName();

    curStateIdM = theNextStateId;
    State& nextState = fsmM->getState(curStateIdM);

    LOG_DEBUG( getSessionName() << "[" << sessionIdM << "] " 
            << preStateName << " -> " << nextState.getName());

    return nextState;
}

//-----------------------------------------------------------------------------

void onFsmTimeOut(int theFd, short theEvt, void *theArg)
{
    TimerPair* timerPair = (TimerPair*) theArg;
    Session* session = timerPair->first;
    int timerId = timerPair->second;
    session->asynHandleTimeout(timerId);
    // the timerPair will be deleted in the fsm's thread
    //delete timerPair;
}
//-----------------------------------------------------------------------------

void Session::asynHandleTimeout(const unsigned char theTimerId)
{
    Processor::BoostProcessor::fsmInstance()->process(sessionIdM,
        &Session::handleTimeout, this, theTimerId);
}

//-----------------------------------------------------------------------------

void Session::handleTimeout(const unsigned char theTimerId)
{
#ifdef DEBUG
    extern boost::thread_specific_ptr<unsigned> g_threadGroupTotal;
    extern boost::thread_specific_ptr<unsigned> g_threadGroupIndex;
    unsigned threadCount = *g_threadGroupTotal.get();
    unsigned threadIndex = *g_threadGroupIndex.get();
    if ((sessionIdM%threadCount) != threadIndex)
    {
        LOG_FATAL("job is handled in wrong thread:" << threadIndex 
                << ", count:" << threadCount
                << ", id:" << sessionIdM);
        assert(false);
    }
    

    if (-1 == tidM)
    {
        tidM = gettid();
    }
    else if (tidM != gettid())
    {
        LOG_FATAL("tid not match pre:" << tidM << "-now:" << gettid());
        assert(false);
    }
#endif
    if (fsmTimeoutEvtM && theTimerId == timerIdM)
    {
        //otherwise it is another timer and the previous one is freed already
        TimerPair* timePair = (TimerPair*)event_get_callback_arg(fsmTimeoutEvtM);
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
        delete timePair;
        handleEvent(TIMEOUT_EVT);
    }
}

//-----------------------------------------------------------------------------

void Session::newTimer(const long long theUsec)
{
    if (fsmTimeoutEvtM)
    {
        TimerPair* timePair = (TimerPair*)event_get_callback_arg(fsmTimeoutEvtM);
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
        delete timePair;
    }
    timerIdM++;
    TimerPair* timerPair = new TimerPair(this, timerIdM);
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
        TimerPair* timePair = (TimerPair*)event_get_callback_arg(fsmTimeoutEvtM);
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
        delete timePair;
    }
}

//-----------------------------------------------------------------------------

