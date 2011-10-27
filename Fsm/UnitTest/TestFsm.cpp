#include "Fsm.h"
#include "Action.h"
#include "State.h"
#include "Session.h"
#include "Reactor.h"
#include <iostream>
#include <boost/bind.hpp>
#include <unistd.h>
using namespace std;

int testEnter(Fsm::Session* theSession)
{
    cout << "enter" << endl;
    return 0;
}
int testExit(Fsm::Session* theSession)
{
    cout << "exit" << endl;
    return 0;
}

enum MyState
{
    INIT_STATE = 1,
    TIME_STATE = 2,
    END_STATE  = 3
};

enum MyEvent
{
    START_EVT = 0
};

int main()
{
    Fsm::FiniteStateMachine fsm;
    fsm += NEW_FSM_STATE(INIT_STATE);
    fsm +=      Fsm::Event(Fsm::ENTRY_EVT,  &testEnter);
    fsm +=      Fsm::Event(START_EVT,  CHANGE_STATE(TIME_STATE));
    fsm +=      Fsm::Event(Fsm::EXIT_EVT,   &testExit);

    fsm += NEW_FSM_STATE(TIME_STATE);
    fsm +=      Fsm::Event(Fsm::ENTRY_EVT,   NEW_TIMER(1 * 1000000));
    fsm +=      Fsm::Event(Fsm::TIMEOUT_EVT, CHANGE_STATE(END_STATE));
    fsm +=      Fsm::Event(Fsm::EXIT_EVT,    CANCEL_TIMER());

    fsm += NEW_FSM_STATE(END_STATE);
    fsm +=      Fsm::Event(Fsm::ENTRY_EVT,  &testEnter);
    fsm +=      Fsm::Event(Fsm::EXIT_EVT,   &testExit);
    cout << "fsm,initstate:" << fsm.getFirstStateId() << endl;
    cout << "fsm,endstate:" << fsm.getLastStateId() << endl;

    Fsm::Session session(&fsm, 0);
    session.handleEvent(START_EVT);
    sleep(3);
    return 0;
}

