#ifndef EVENT_H
#define EVENT_H

#include "Action.h"


namespace Fsm
{
	enum PreDefineEvent
	{
		ENTRY_EVT     = -1,
		EXIT_EVT      = -2,
		TIMEOUT_EVT   = -3,
		NEXT_EVT      = -4,
		OK_EVT        = -5, 
		FAILED_EVT    = -6

	};
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

