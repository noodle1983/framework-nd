#ifndef EVENT_H
#define EVENT_H
namespace Fsm
{
    class Event
    {
    public:
        Event(const int theId, const int theEventArg, Action& theAction)
            : idM(theId)
            , eventArgM(theEventArg)
            , actionM(theAction)
        {}
        ~Event();

        int getId()
        {
            return idM;
        }

        int getEventArg()
        {
            return eventArgM;
        }

        Action getAction()
        {
            return actionM;
        }
    private:
        int idM;
        int eventArgM;
        Action actionM;

    };
}
#endif /* EVENT_H */

