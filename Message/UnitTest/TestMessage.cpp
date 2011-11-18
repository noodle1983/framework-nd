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
        serviceNotification1.userId.typeM = MobileIdentityIEI::IMSI_E;
        serviceNotification1.userId.valueM = 13800138000;//it is not imsi actually, test only
        serviceNotification1.serviceType.valueM = SERVICE_TYPE_SUMMERY_E;           
        serviceNotification1.serviceInfo.srcIpAddr.valueM = 1;            
        serviceNotification1.serviceInfo.destIpAddr.valueM = 2;           
        serviceNotification1.serviceInfo.srcPort.valueM = 3;           
        serviceNotification1.serviceInfo.descPort.valueM = 4;           
        serviceNotification1.serviceInfo.protocol.valueM = 5;           
        serviceNotification1.serviceInfo.catalogId.valueM = 6;           
        serviceNotification1.serviceInfo.appId.valueM = 7;           
        serviceNotification1.serviceInfo.behaviorId.valueM = 8;           
        serviceNotification1.serviceInfo.upPktCnt.valueM = 9;           
        serviceNotification1.serviceInfo.downPktCnt.valueM = 10;           
        serviceNotification1.serviceInfo.upBytes.valueM = 11;           
        serviceNotification1.serviceInfo.downBytes.valueM = 12;           

        //serviceNotification1.location.valueM = "location";           
        serviceNotification1.timestamp.seconds.valueM = 13;           
        serviceNotification1.timestamp.fraction.valueM = 14;           
        serviceNotification1.mSISDN.reset(PhoneNumberIEI("13800138000"));           
        serviceNotification1.imei.reset(serviceNotification1.userId);           
        serviceNotification1.userAgent.reset(UserAgentIEI("Firefox"));           
        serviceNotification1.apn.reset(ApnIEI("CMNET"));           
        serviceNotification1.url.reset(UrlIEI("http://www.google.se"));           
        serviceNotification1.rtd.reset(RtdIEI("rtd"));           


        char buffer[1024];
        unsigned wIndex = 0;
        serviceNotification1.encode(buffer, sizeof(buffer), wIndex);
        ServiceNotification serviceNotification2;
        unsigned rIndex = 0;
        serviceNotification2.decode(buffer, wIndex, rIndex);
        //assert(rIndex == wIndex);
        serviceNotification2.dump(std::cout);
    }
    {
        UserDataNotification msg;
        msg.init();
        msg.userId.valueM = 123456789012345;           
        msg.ip.valueM = 1;           
        msg.lac.valueM = 2;           
        msg.rac.valueM = 3;           
        msg.ci.valueM = 4;           
        msg.status.valueM = 5;           
        msg.content.valueM = "abc";           

        char buffer[1024];
        unsigned wIndex = 0;
        msg.encode(buffer, sizeof(buffer), wIndex);
        UserDataNotification msg2;
        unsigned rIndex = 0;
        msg2.decode(buffer, wIndex, rIndex);
        //assert(rIndex == wIndex);
        msg2.dump(std::cout);

    }

    return 0;
}

