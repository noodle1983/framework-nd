#ifndef SESSION_H
#define SESSION_H

#include "Fsm.h"
#include "State.h"

namespace Fsm
{
    class Session
    {
    public:
        Session(FiniteStateMachine * theFsm, void* theData);
        ~Session();

        State& toNextState(const int theNextStateId);
        void handleEvent(const int theEventId, const int theEventArg);

        inline State& getEndState()
        {
            return fsmM->getState(endStateIdM);
        }

        inline State& getCurState()
        {
            return fsmM->getState(curStateIdM);
        }

        template<typename DataType>
        inline DataType* getData() 
        {
            return (DataType*)dataM;
        }

    private:
        int curStateIdM;
        int endStateIdM;
        FiniteStateMachine* fsmM;
        bool isInitializedM;
        void* dataM;
        
    };
}

#endif /* SESSION_H */
