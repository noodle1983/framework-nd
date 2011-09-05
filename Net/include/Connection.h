#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/shared_ptr.hpp>

namespace Net
{
    class Connection
    {
    public:
        virtual ~Connection() = 0;
        virtual void onRead(int theFd, short theEvt, void *theArg) = 0;
        virtual void onWrite(int theFd, short theEvt, void *theArg) = 0;

    };

    typedef boost::shared_ptr<Connection> ConnectionPtr;
    typedef std::map<int, ConnectionPtr> ConnectionMap;
}

#endif /*CONNECTION_H*/

