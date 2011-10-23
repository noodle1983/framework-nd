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
        State(const int theId)
            :idM(theId)
        {}
        ~State();

        inline int getId(){return idM;}
        void addEvent(Event& theEvent)
        {
            int eventId = theEvent.getId();
            ActionList& actionList = rulesM[eventId];
            actionList.push_back(theEvent.getAction());
        }

    private:
        int idM;
        std::map<int, ActionList> rulesM; 
    };
}
#endif /* STATE_H */

