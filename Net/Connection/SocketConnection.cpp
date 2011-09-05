#include "SocketConnection.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <err.h>

using namespace Net::Connection;

//-----------------------------------------------------------------------------
SocketConnection::SocketConnection(const int theFd)
	:fdM(theFd)
{
}

//-----------------------------------------------------------------------------

SocketConnection::~SocketConnection()
{
	std::list<Buffer*>::iterator it = outputQueueM.begin();
	for (; it != outputQueueM.end(); it++)
	{
		delete *it;
	}
}

//-----------------------------------------------------------------------------

void SocketConnection::onRead(int theFd, short theEvt)
{
	Buffer* buffer = new Buffer;

    buffer->lenM = read(theFd, buffer->rawM, sizeof(buffer->rawM));
    if (buffer->lenM == 0) 
	{
        printf("Client disconnected.\n");
        close(theFd);
        event_del(&readEvtM);
        return;
    }
    else if (buffer->lenM < 0) 
	{
        printf("Socket failure, disconnecting client: %s",
            strerror(errno));
        close(theFd);
        event_del(&readEvtM);
        return;
    }
	
	outputQueueM.push_back(buffer);
    event_add(&writeEvtM, NULL);
}

//-----------------------------------------------------------------------------

void SocketConnection::onWrite(int theFd, short theEvt)
{

    if (outputQueueM.begin() == outputQueueM.end())
    	return;

	Buffer* buffer = outputQueueM.front();

    int len = buffer->lenM - buffer->offsetM;
    len = write(theFd, buffer->rawM + buffer->offsetM, len);
    if (len == -1) {
        if (errno == EINTR || errno == EAGAIN) 
		{
            event_add(&writeEvtM, NULL);
            return;
        }
        else 
		{
            err(1, "write");
        }
    }
    else if ((buffer->offsetM + len) < buffer->lenM) 
	{
        buffer->offsetM += len;
        event_add(&writeEvtM, NULL);
        return;
    }
	else if ((buffer->offsetM + len) > buffer->lenM)
	{
		err(1, "write");
	}
	outputQueueM.pop_front();
	delete buffer;
	
}

//-----------------------------------------------------------------------------


