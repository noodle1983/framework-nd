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
    return 0;
}

//-----------------------------------------------------------------------------

int Session::handleEvent(const int theEventId)
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
        return -1;
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
    return 0;
}

//-----------------------------------------------------------------------------

State& Session::toNextState(const int theNextStateId)
{
    const std::string& preStateName = fsmM->getState(curStateIdM).getName();

    curStateIdM = theNextStateId;
    State& nextState = fsmM->getState(curStateIdM);

    LOG_DEBUG( sessionNameM << "[" << sessionIdM << "] " 
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
	// the timerPair will be deleted in the fsm's thread
    //delete timerPair;
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
		delete (TimerPair*)fsmTimeoutEvtM->ev_arg;
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
        handleEvent(TIMEOUT_EVT);
    }
}

//-----------------------------------------------------------------------------

void Session::newTimer(const long long theUsec)
{
    if (fsmTimeoutEvtM)
    {
		delete (TimerPair*)fsmTimeoutEvtM->ev_arg;
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
		delete (TimerPair*)fsmTimeoutEvtM->ev_arg;
        Net::Reactor::Reactor::instance()->delEvent(fsmTimeoutEvtM);
    }
}

//-----------------------------------------------------------------------------

