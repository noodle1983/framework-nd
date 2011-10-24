#ifndef STATE_H
#define STATE_H

#include <map>
#include <list>

#include <Event.h>
#include <Action.h>

namespace Fsm
{
    typedef std::list<Action> ActionList;
    class State
    {
    public:   
        State(){}
        State(const int theId)
            :idM(theId)
        {}
        ~State(){};

        inline int getId()const {return idM;}
        void addEvent(const Event& theEvent)
        {
            int eventId = theEvent.getId();
            ActionList& actionList = rulesM[eventId];
            actionList.push_back(theEvent.getAction());
        }
        ActionList& getActionList(const int theEventId, const int theEventArg)
        {
            return rulesM[theEventId];
        }

        ActionList& getActionList(const int theEventId)
        {
            return rulesM[theEventId];
        }

    private:
        int idM;
        std::map<int, ActionList> rulesM; 
    };
}
#endif /* STATE_H */

