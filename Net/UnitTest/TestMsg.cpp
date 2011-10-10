
#include <assert.h>

#include <MsgCodec/IntCodec.h>
#include <iostream>


using namespace std;
using namespace Net::MsgCodec;
struct TestMsg
{
    uint32_t ui0;
    uint32_t ui1;
    uint32_t ui2;
    uint32_t ui3;
};

int main()
{
    char buffer[16];
    memset(buffer, 1, 16);
    CodecInterface* msgCodec = new Uint32Codec<TestMsg>(&TestMsg::ui0, 
                               new Uint32Codec<TestMsg>(&TestMsg::ui1, 
                               new Uint32Codec<TestMsg>(&TestMsg::ui2, 
                               new Uint32Codec<TestMsg>(&TestMsg::ui3,
                                  NULL))));
    unsigned index = 0;
    TestMsg msg; 
    msgCodec->decode((const char*)buffer, (unsigned)16, index, (void*)&msg);
    cout << "TestMsg:" << msg.ui0 << ","
        << msg.ui1 << ","
        << msg.ui2 << ","
        << msg.ui3 << endl;

    TestMsg msg2;
    msg2.ui0 = 16843009;
    msg2.ui1 = 16843009;
    msg2.ui2 = 16843009;
    msg2.ui3 = 16843009;
    char buffer2[16];
    unsigned index2 = 0;
    msgCodec->encode((char*)buffer2, (unsigned)16, index2, (void*)&msg2);
    assert(!memcmp(buffer, buffer2, 16));
    return 0;
}

