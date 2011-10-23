#ifndef SESSION_H
#define SESSION_H

namespace Fsm
{
    class Session
    {
    public:
        Session(const int theInitState);
        virtual ~Session(){};

    private:
        int curState;
    };
}

#endif /* SESSION_H */
