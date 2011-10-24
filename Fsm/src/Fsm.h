#ifndef FSM_H
#define FSM_H

#include <map>
#include <State.h>

namespace Fsm
{
    class FiniteStateMachine
    {
    public:
        enum {STATE_NONE = -1};
        FiniteStateMachine()
            : initStateIdM(STATE_NONE)
            , endStateIdM(STATE_NONE)
        {
        }

        ~FiniteStateMachine(){}

        FiniteStateMachine& operator+=(const State& theState);
        FiniteStateMachine& operator+=(const Event& theEvent);

        inline State& getState(const int theStateId)
        {
            return statesM[theStateId];
        }
        
        inline int getFirstState()
        {
            return initStateIdM;
        }
    
        inline int getLastState()
        {
            return endStateIdM;
        }

    private:
        std::map<int, State> statesM;
        int initStateIdM;
        int endStateIdM;
    };

}
#endif /* FSM_H */

