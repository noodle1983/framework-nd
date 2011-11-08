#include "AutoMsg/Message.h"
#include <iostream>
#include <glib.h>
#include <assert.h>
using namespace std;
using namespace Msg;

int main()
{
    {
        char buffer[12];
        int length = GINT32_TO_BE(12);
        memcpy(buffer, &length, 4);
        memset(buffer+4, 1, 12);
        MsgHeader header;
        unsigned index = 0;
        header.decode(buffer, 12, index);
        header.dump(std::cout);

        char buffer2[12];
        index = 0;
        header.encode(buffer2, 12, index);
        assert(0 == memcmp(buffer, buffer2, 12));
        assert(12 == index);
    }

    {
        ServiceNotification serviceNotification1;
        serviceNotification1.init();
        serviceNotification1.transactionId.valueM = 0;
        serviceNotification1.userId.valueM = 1;
        serviceNotification1.ip.valueM = 2;
        serviceNotification1.lac.valueM = 3;
        serviceNotification1.rac.valueM = 4;
        serviceNotification1.ci.valueM = 5;
        serviceNotification1.status.valueM = 6;
        serviceNotification1.content.valueM = string("ServiceNotification");

        char buffer[1024];
        unsigned wIndex = 0;
        serviceNotification1.encode(buffer, sizeof(buffer), wIndex);
        assert(wIndex == ServiceNotification::MIN_BYTES + strlen("ServiceNotification"));
        ServiceNotification serviceNotification2;
        unsigned rIndex = 0;
        serviceNotification2.decode(buffer, wIndex, rIndex);
        assert(rIndex == wIndex);
        serviceNotification2.dump(std::cout);
    }

    return 0;
}

