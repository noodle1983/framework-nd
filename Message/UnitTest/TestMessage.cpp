#include "AutoMsg/Message.h"
#include <iostream>
#include <glib.h>
#include <assert.h>
using namespace std;
using namespace GbApp::Msg;

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
        UserDataNotification userDataNotification1;
        userDataNotification1.init();
        userDataNotification1.transactionId.valueM = 0;
        userDataNotification1.userId.valueM = 1;
        userDataNotification1.ip.valueM = 2;
        userDataNotification1.lac.valueM = 3;
        userDataNotification1.rac.valueM = 4;
        userDataNotification1.ci.valueM = 5;
        userDataNotification1.status.valueM = 6;
        userDataNotification1.content.valueM = string("UserDataNotification");

        char buffer[1024];
        unsigned wIndex = 0;
        userDataNotification1.encode(buffer, sizeof(buffer), wIndex);
        assert(wIndex == UserDataNotification::MIN_BYTES + strlen("UserDataNotification"));
        UserDataNotification userDataNotification2;
        unsigned rIndex = 0;
        userDataNotification2.decode(buffer, wIndex, rIndex);
        assert(rIndex == wIndex);
        userDataNotification2.dump(std::cout);
    }

    return 0;
}

