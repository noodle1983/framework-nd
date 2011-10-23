#ifndef FSM_H
#define FSM_H

#include <map>
#include <State.h>

namespace Fsm
{
    class Fsm
    {
    public:
        enum {STATE_NONE = -1;};
        Fsm()
            : initStateIdM(STATE_NONE)
            , endStateIdM(STATE_NONE)
        {
        }
        ~Fsm();

        Fsm operator+(State& theState)
        {
            int stateId = theState.getId();
            if (STATE_NONE == initStateIdM)
            {
                initStateIdM = stateId;
            }
            endStateIdM = stateId;

            statesM[theState.getId()] = theState;
        }

        Fsm operator+(Event& theEvent)
        {
            State& curState = statesM[endStateIdM];
            curState.addEvent(theEvent);
        }
    
    private:
        std::map<int, State> statesM;
        int initStateIdM;
        int endStateIdM;
    };

}
#endif /* FSM_H */

