#ifndef EVENT_H
#define EVENT_H

#include "Action.h"

enum 
{
    ENTRY_EVT = -1,
    EXIT_EVT  = -2,
    TIMEOUT_EVT = -3
};

namespace Fsm
{
    class Event
    {
    public:

        Event(const int theId, Action theAction)
            : idM(theId)
            , actionM(theAction)
        {}
        ~Event(){};

        int getId() const
        {
            return idM;
        }

        Action getAction() const
        {
            return actionM;
        }
    private:
        int idM;
        Action actionM;

    };
}
#endif /* EVENT_H */

