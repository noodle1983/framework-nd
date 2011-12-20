#ifndef QOS_H
#define QOS_H

#include "IntMsg.h"
#include "MsgErrorCode.h"
#include "Log.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

namespace Msg
{
    template<char theTag>
    class Qos
    {
    public:
        Qos()
        {}
        ~Qos(){}

        enum {TAG = theTag};
        enum
        {
            MIN_BYTES = 2 +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            arp.init();
            delay.init();
            reliability.init();
            peak.init();
            precedence.init();
            mean.init();
            traffic.init();
            deliveryorder.init();
            deliveryOfErroneousSdu.init();
            maxSduSize.init();
            maxBitRateUp.init();
            maxBitRateDown.init();
            residualBer.init();
            sduErrRatio.init();
            transferDelay.init();
            trafficHandlingPriority.init();
            guaranteedBitRateUp.init();
            guaranteedBitRateDown.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            Uint8 tag;
            ret = tag.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;
            if (tag.valueM != TAG)
                return ERROR_E;

            Uint8 length;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;
            if (theIndex + length.valueM > theLen)
                return NOT_ENOUGH_BUFFER_E;

            ret = arp.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.arp");
                return ret;
            }

            ret = delay.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.delay");
                return ret;
            }

            ret = reliability.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.reliability");
                return ret;
            }

            ret = peak.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.peak");
                return ret;
            }

            ret = precedence.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.precedence");
                return ret;
            }

            ret = mean.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.mean");
                return ret;
            }

            ret = traffic.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.traffic");
                return ret;
            }

            ret = deliveryorder.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.deliveryorder");
                return ret;
            }

            ret = deliveryOfErroneousSdu.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.deliveryOfErroneousSdu");
                return ret;
            }

            ret = maxSduSize.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.maxSduSize");
                return ret;
            }

            ret = maxBitRateUp.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.maxBitRateUp");
                return ret;
            }

            ret = maxBitRateDown.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.maxBitRateDown");
                return ret;
            }

            ret = residualBer.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.residualBer");
                return ret;
            }

            ret = sduErrRatio.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.sduErrRatio");
                return ret;
            }

            ret = transferDelay.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.transferDelay");
                return ret;
            }

            ret = trafficHandlingPriority.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.trafficHandlingPriority");
                return ret;
            }

            ret = guaranteedBitRateUp.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.guaranteedBitRateUp");
                return ret;
            }

            ret = guaranteedBitRateDown.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosSubMsg.guaranteedBitRateDown");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            Uint8 length;
            length.valueM = MIN_BYTES - 2;
            if (theIndex + MIN_BYTES > theLen)
                return NOT_ENOUGH_BUFFER_E;

            theBuffer[theIndex++] = TAG;
            length.encode(theBuffer, theLen, theIndex);

            ret = arp.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.arp");
                return ret;
            }

            ret = delay.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.delay");
                return ret;
            }

            ret = reliability.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.reliability");
                return ret;
            }

            ret = peak.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.peak");
                return ret;
            }

            ret = precedence.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.precedence");
                return ret;
            }

            ret = mean.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.mean");
                return ret;
            }

            ret = traffic.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.traffic");
                return ret;
            }

            ret = deliveryorder.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.deliveryorder");
                return ret;
            }

            ret = deliveryOfErroneousSdu.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.deliveryOfErroneousSdu");
                return ret;
            }

            ret = maxSduSize.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.maxSduSize");
                return ret;
            }

            ret = maxBitRateUp.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.maxBitRateUp");
                return ret;
            }

            ret = maxBitRateDown.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.maxBitRateDown");
                return ret;
            }

            ret = residualBer.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.residualBer");
                return ret;
            }

            ret = sduErrRatio.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.sduErrRatio");
                return ret;
            }

            ret = transferDelay.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.transferDelay");
                return ret;
            }

            ret = trafficHandlingPriority.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.trafficHandlingPriority");
                return ret;
            }

            ret = guaranteedBitRateUp.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.guaranteedBitRateUp");
                return ret;
            }

            ret = guaranteedBitRateDown.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosSubMsg.guaranteedBitRateDown");
                return ret;
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "QosSubMsg";
            }
            else
            {
                theOut << "QosSubMsg";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "arp: ";
            arp.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "delay: ";
            delay.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "reliability: ";
            reliability.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "peak: ";
            peak.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "precedence: ";
            precedence.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "mean: ";
            mean.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "traffic: ";
            traffic.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "deliveryorder: ";
            deliveryorder.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "deliveryOfErroneousSdu: ";
            deliveryOfErroneousSdu.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "maxSduSize: ";
            maxSduSize.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "maxBitRateUp: ";
            maxBitRateUp.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "maxBitRateDown: ";
            maxBitRateDown.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "residualBer: ";
            residualBer.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "sduErrRatio: ";
            sduErrRatio.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transferDelay: ";
            transferDelay.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "trafficHandlingPriority: ";
            trafficHandlingPriority.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "guaranteedBitRateUp: ";
            guaranteedBitRateUp.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "guaranteedBitRateDown: ";
            guaranteedBitRateDown.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Uint8 arp;
        Uint8 delay;
        Uint8 reliability;
        Uint8 peak;
        Uint8 precedence;
        Uint8 mean;
        Uint8 traffic;
        Uint8 deliveryorder;
        Uint8 deliveryOfErroneousSdu;
        Uint8 maxSduSize;
        Uint8 maxBitRateUp;
        Uint8 maxBitRateDown;
        Uint8 residualBer;
        Uint8 sduErrRatio;
        Uint8 transferDelay;
        Uint8 trafficHandlingPriority;
        Uint8 guaranteedBitRateUp;
        Uint8 guaranteedBitRateDown;
    };

}
#endif  /*QOS_H*/


