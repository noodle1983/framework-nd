#include "Message.h"
#include <iostream>
#include <assert.h>
using namespace std;
using namespace GbApp::Msg;

int main()
{
    char buffer[16];
    int length = htonl(16);
    memcpy(buffer, &length, 4);
    memset(buffer+4, 1, 12);
    buffer[12] = 0;
    buffer[13] = 2;
    buffer[14] = 'H';
    buffer[15] = 'i';
    Header header;
    unsigned index = 0;
    header.decode(buffer, 16, index);
    cout << header.length.valueM << endl;
    cout << header.transactionId.valueM << endl;
    cout << header.messageType.valueM << endl;
    cout << header.option.valueM << endl;

    char buffer2[16];
    index = 0;
    header.encode(buffer2, 16, index);
    assert(0 == memcmp(buffer, buffer2, 16));
    assert(16 == index);

    return 0;
}

