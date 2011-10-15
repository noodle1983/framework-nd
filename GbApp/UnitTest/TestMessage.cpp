#include "AutoMsg/Message.h"
#include <iostream>
#include <glib.h>
#include <assert.h>
using namespace std;
using namespace GbApp::Msg;

int main()
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

    return 0;
}

