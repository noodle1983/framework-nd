#ifndef ACTION_H
#define ACTION_H

#include <boost/function.hpp>

namespace Fsm
{
    class Session;
    typedef boost::function<void (Fsm::Session *)> Action;

    int changeState(
            Fsm::Session* theSession, 
            const int theNextStateId);

    int generateEvent(
            Fsm::Session* theSession, 
            const int theEventId);

    int newSecTimer(
            Fsm::Session* theSession, 
            const int theSeconds);

    int cancelTimer(
            Fsm::Session* theSession);
}

#endif /* ACTION_H */
