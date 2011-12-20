// this file is generate automatically, please don't change it mannually
#ifndef MESSAGE_H
#define MESSAGE_H

#include "StrMsg.h"
#include "IntMsg.h"
#include "MsgErrorCode.h"
#include "MobileIdentity.h"
#include "TlvBcdString.h"
#include "Qos.h"
#include "Location.h"
#include "Timestamp.h"
#include "BcdString.h"
#include "Log.h"
#include <boost/optional.hpp>

namespace Msg
{
    enum ServiceType
    {
        SERVICE_TYPE_ACTION_ORIENTED_E = 1,
        SERVICE_TYPE_PER_SOCKET_E      = 2,
        SERVICE_TYPE_SUMMERY_E         = 3
    };

    typedef MobileIdentity<0x10> MobileIdentityIEI;
    typedef Location<0x11> LocationIEI;
    typedef Qos<0x12> QosIEI;
    typedef Timestamp<0x14> DateTimeIEI;
    typedef TlvString<0x16, Uint16> UserAgentIEI;
    typedef TlvString<0x17, Uint16> ApnIEI;
    typedef TlvString<0x18, Uint16> UrlIEI;
    typedef TlvString<0x20, Uint16> PropertiesIEI;
    typedef TlvString<0x21, Uint16> StrategyIEI;
    typedef TlvString<0x22, Uint16> HttpHeaderIEI;
    typedef TlvString<0x23, Uint16> TriggerIEI;
    typedef TlvString<0x24, Uint16> CauseIEI;

    class MsgHeader
    {
    public:
        MsgHeader(){}
        ~MsgHeader(){}

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse MsgHeader.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse MsgHeader.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse MsgHeader.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse MsgHeader.messageType");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode MsgHeader.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode MsgHeader.transactionId");
                return ret;
            }

            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode MsgHeader.messageType");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode MsgHeader.length");
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
                theOut << "\n" <<leadStr << "MsgHeader";
            }
            else
            {
                theOut << "MsgHeader";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        Uint32 messageType;
    }; /* end of class MsgHeader */


    class Heartbeat
    {
    public:
        Heartbeat(){}
        ~Heartbeat(){}

        enum{ ID = 0x00};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse Heartbeat.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse Heartbeat.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse Heartbeat.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse Heartbeat.messageType");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode Heartbeat.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode Heartbeat.transactionId");
                return ret;
            }

            messageType.valueM = Heartbeat::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode Heartbeat.messageType");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode Heartbeat.length");
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
                theOut << "\n" <<leadStr << "Heartbeat";
            }
            else
            {
                theOut << "Heartbeat";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
    }; /* end of class Heartbeat */


    class PDPActivateNotification
    {
    public:
        PDPActivateNotification(){}
        ~PDPActivateNotification(){}

        enum{ ID = 0x01};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        MobileIdentityIEI::MIN_BYTES +
                        QosIEI::MIN_BYTES +
                        LocationIEI::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            userId.init();
            qos.init();
            location.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPActivateNotification.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse PDPActivateNotification.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPActivateNotification.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPActivateNotification.messageType");
                return ret;
            }

            ret = userId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPActivateNotification.userId");
                return ret;
            }

            ret = qos.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPActivateNotification.qos");
                return ret;
            }

            ret = location.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPActivateNotification.location");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPActivateNotification.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPActivateNotification.transactionId");
                return ret;
            }

            messageType.valueM = PDPActivateNotification::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPActivateNotification.messageType");
                return ret;
            }

            ret = userId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPActivateNotification.userId");
                return ret;
            }

            ret = qos.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPActivateNotification.qos");
                return ret;
            }

            ret = location.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPActivateNotification.location");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode PDPActivateNotification.length");
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
                theOut << "\n" <<leadStr << "PDPActivateNotification";
            }
            else
            {
                theOut << "PDPActivateNotification";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "userId: ";
            userId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "qos: ";
            qos.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "location: ";
            location.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        MobileIdentityIEI userId;
        QosIEI qos;
        LocationIEI location;
    }; /* end of class PDPActivateNotification */


    class PDPDeactivateNotification
    {
    public:
        PDPDeactivateNotification(){}
        ~PDPDeactivateNotification(){}

        enum{ ID = 0x07};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        MobileIdentityIEI::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            userId.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPDeactivateNotification.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse PDPDeactivateNotification.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPDeactivateNotification.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPDeactivateNotification.messageType");
                return ret;
            }

            ret = userId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse PDPDeactivateNotification.userId");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPDeactivateNotification.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPDeactivateNotification.transactionId");
                return ret;
            }

            messageType.valueM = PDPDeactivateNotification::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPDeactivateNotification.messageType");
                return ret;
            }

            ret = userId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode PDPDeactivateNotification.userId");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode PDPDeactivateNotification.length");
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
                theOut << "\n" <<leadStr << "PDPDeactivateNotification";
            }
            else
            {
                theOut << "PDPDeactivateNotification";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "userId: ";
            userId.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        MobileIdentityIEI userId;
    }; /* end of class PDPDeactivateNotification */


    class QosModifyREQ
    {
    public:
        QosModifyREQ(){}
        ~QosModifyREQ(){}

        enum{ ID = 0x02};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        MobileIdentityIEI::MIN_BYTES +
                        QosIEI::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            userId.init();
            qos.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyREQ.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse QosModifyREQ.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyREQ.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyREQ.messageType");
                return ret;
            }

            ret = userId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyREQ.userId");
                return ret;
            }

            ret = qos.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyREQ.qos");
                return ret;
            }

            while(theIndex < endIndex)
            {
                if (theBuffer[theIndex] == TriggerIEI::TAG)
                {
                    trigger.reset(TriggerIEI());
                    ret = trigger->decode( theBuffer, endIndex, theIndex);
                    if (SUCCESS_E != ret)
                    {
                        LOG_ERROR("failed to parse QosModifyREQ.trigger");
                        return ret;
                    }
                }
                else
                {
                    LOG_ERROR("failed to parse structure at index" << theIndex);
                    return ERROR_E;
                }
           }

            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyREQ.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyREQ.transactionId");
                return ret;
            }

            messageType.valueM = QosModifyREQ::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyREQ.messageType");
                return ret;
            }

            ret = userId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyREQ.userId");
                return ret;
            }

            ret = qos.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyREQ.qos");
                return ret;
            }

            if (trigger)
            {
                ret = trigger->encode(theBuffer, theLen, theIndex);
                if (SUCCESS_E != ret)
                {
                    LOG_ERROR("failed to encode QosModifyREQ.trigger");
                    return ret;
                }
            }

            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode QosModifyREQ.length");
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
                theOut << "\n" <<leadStr << "QosModifyREQ";
            }
            else
            {
                theOut << "QosModifyREQ";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "userId: ";
            userId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "qos: ";
            qos.dump(theOut, theLayer + 1);

            if (trigger)
            {
                theOut << "\n" << leadStr << "trigger: ";
                trigger->dump(theOut, theLayer + 1);
            }
            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        MobileIdentityIEI userId;
        QosIEI qos;
        boost::optional<TriggerIEI> trigger;
    }; /* end of class QosModifyREQ */


    class QosModifyAccept
    {
    public:
        QosModifyAccept(){}
        ~QosModifyAccept(){}

        enum{ ID = 0x03};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyAccept.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse QosModifyAccept.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyAccept.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyAccept.messageType");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyAccept.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyAccept.transactionId");
                return ret;
            }

            messageType.valueM = QosModifyAccept::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyAccept.messageType");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode QosModifyAccept.length");
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
                theOut << "\n" <<leadStr << "QosModifyAccept";
            }
            else
            {
                theOut << "QosModifyAccept";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
    }; /* end of class QosModifyAccept */


    class QosModifyReject
    {
    public:
        QosModifyReject(){}
        ~QosModifyReject(){}

        enum{ ID = 0x04};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        CauseIEI::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            cause.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyReject.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse QosModifyReject.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyReject.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyReject.messageType");
                return ret;
            }

            ret = cause.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse QosModifyReject.cause");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyReject.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyReject.transactionId");
                return ret;
            }

            messageType.valueM = QosModifyReject::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyReject.messageType");
                return ret;
            }

            ret = cause.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode QosModifyReject.cause");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode QosModifyReject.length");
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
                theOut << "\n" <<leadStr << "QosModifyReject";
            }
            else
            {
                theOut << "QosModifyReject";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "cause: ";
            cause.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        CauseIEI cause;
    }; /* end of class QosModifyReject */


    class ServiceInfoIEI
    {
    public:
        ServiceInfoIEI(){}
        ~ServiceInfoIEI(){}

        enum
        {
            MIN_BYTES =
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        Uint48::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            srcIpAddr.init();
            destIpAddr.init();
            srcPort.init();
            descPort.init();
            protocol.init();
            catalogId.init();
            appId.init();
            behaviorId.init();
            upPktCnt.init();
            downPktCnt.init();
            upBytes.init();
            downBytes.init();
            rtd.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = srcIpAddr.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.srcIpAddr");
                return ret;
            }

            ret = destIpAddr.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.destIpAddr");
                return ret;
            }

            ret = srcPort.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.srcPort");
                return ret;
            }

            ret = descPort.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.descPort");
                return ret;
            }

            ret = protocol.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.protocol");
                return ret;
            }

            ret = catalogId.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.catalogId");
                return ret;
            }

            ret = appId.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.appId");
                return ret;
            }

            ret = behaviorId.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.behaviorId");
                return ret;
            }

            ret = upPktCnt.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.upPktCnt");
                return ret;
            }

            ret = downPktCnt.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.downPktCnt");
                return ret;
            }

            ret = upBytes.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.upBytes");
                return ret;
            }

            ret = downBytes.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.downBytes");
                return ret;
            }

            ret = rtd.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceInfoIEI.rtd");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = srcIpAddr.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.srcIpAddr");
                return ret;
            }

            ret = destIpAddr.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.destIpAddr");
                return ret;
            }

            ret = srcPort.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.srcPort");
                return ret;
            }

            ret = descPort.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.descPort");
                return ret;
            }

            ret = protocol.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.protocol");
                return ret;
            }

            ret = catalogId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.catalogId");
                return ret;
            }

            ret = appId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.appId");
                return ret;
            }

            ret = behaviorId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.behaviorId");
                return ret;
            }

            ret = upPktCnt.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.upPktCnt");
                return ret;
            }

            ret = downPktCnt.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.downPktCnt");
                return ret;
            }

            ret = upBytes.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.upBytes");
                return ret;
            }

            ret = downBytes.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.downBytes");
                return ret;
            }

            ret = rtd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceInfoIEI.rtd");
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
                theOut << "\n" <<leadStr << "ServiceInfoIEI";
            }
            else
            {
                theOut << "ServiceInfoIEI";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "srcIpAddr: ";
            srcIpAddr.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "destIpAddr: ";
            destIpAddr.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "srcPort: ";
            srcPort.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "descPort: ";
            descPort.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "protocol: ";
            protocol.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "catalogId: ";
            catalogId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "appId: ";
            appId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "behaviorId: ";
            behaviorId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "upPktCnt: ";
            upPktCnt.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "downPktCnt: ";
            downPktCnt.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "upBytes: ";
            upBytes.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "downBytes: ";
            downBytes.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "rtd: ";
            rtd.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Uint32 srcIpAddr;
        Uint32 destIpAddr;
        Uint16 srcPort;
        Uint16 descPort;
        Uint8 protocol;
        Uint48 catalogId;
        Uint32 appId;
        Uint32 behaviorId;
        Uint32 upPktCnt;
        Uint32 downPktCnt;
        Uint32 upBytes;
        Uint32 downBytes;
        Uint32 rtd;
    }; /* end of class ServiceInfoIEI */


    class ServiceNotification
    {
    public:
        ServiceNotification(){}
        ~ServiceNotification(){}

        enum{ ID = 0x06};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        MobileIdentityIEI::MIN_BYTES +
                        Uint8::MIN_BYTES +
                        ServiceInfoIEI::MIN_BYTES +
                        LocationIEI::MIN_BYTES +
                        DateTimeIEI::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            userId.init();
            serviceType.init();
            serviceInfo.init();
            location.init();
            timestamp.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceNotification.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse ServiceNotification.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceNotification.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceNotification.messageType");
                return ret;
            }

            ret = userId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceNotification.userId");
                return ret;
            }

            ret = serviceType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceNotification.serviceType");
                return ret;
            }

            ret = serviceInfo.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceNotification.serviceInfo");
                return ret;
            }

            ret = location.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceNotification.location");
                return ret;
            }

            ret = timestamp.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse ServiceNotification.timestamp");
                return ret;
            }

            while(theIndex < endIndex)
            {
                if (theBuffer[theIndex] == PhoneNumberIEI::TAG)
                {
                    mSISDN.reset(PhoneNumberIEI());
                    ret = mSISDN->decode( theBuffer, endIndex, theIndex);
                    if (SUCCESS_E != ret)
                    {
                        LOG_ERROR("failed to parse ServiceNotification.mSISDN");
                        return ret;
                    }
                }
                else if (theBuffer[theIndex] == MobileIdentityIEI::TAG)
                {
                    imei.reset(MobileIdentityIEI());
                    ret = imei->decode( theBuffer, endIndex, theIndex);
                    if (SUCCESS_E != ret)
                    {
                        LOG_ERROR("failed to parse ServiceNotification.imei");
                        return ret;
                    }
                }
                else if (theBuffer[theIndex] == ApnIEI::TAG)
                {
                    apn.reset(ApnIEI());
                    ret = apn->decode( theBuffer, endIndex, theIndex);
                    if (SUCCESS_E != ret)
                    {
                        LOG_ERROR("failed to parse ServiceNotification.apn");
                        return ret;
                    }
                }
                else if (theBuffer[theIndex] == HttpHeaderIEI::TAG)
                {
                    httpHeader.reset(HttpHeaderIEI());
                    ret = httpHeader->decode( theBuffer, endIndex, theIndex);
                    if (SUCCESS_E != ret)
                    {
                        LOG_ERROR("failed to parse ServiceNotification.httpHeader");
                        return ret;
                    }
                }
                else
                {
                    LOG_ERROR("failed to parse structure at index" << theIndex);
                    return ERROR_E;
                }
           }

            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceNotification.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceNotification.transactionId");
                return ret;
            }

            messageType.valueM = ServiceNotification::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceNotification.messageType");
                return ret;
            }

            ret = userId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceNotification.userId");
                return ret;
            }

            ret = serviceType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceNotification.serviceType");
                return ret;
            }

            ret = serviceInfo.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceNotification.serviceInfo");
                return ret;
            }

            ret = location.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceNotification.location");
                return ret;
            }

            ret = timestamp.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode ServiceNotification.timestamp");
                return ret;
            }

            if (mSISDN)
            {
                ret = mSISDN->encode(theBuffer, theLen, theIndex);
                if (SUCCESS_E != ret)
                {
                    LOG_ERROR("failed to encode ServiceNotification.mSISDN");
                    return ret;
                }
            }
            if (imei)
            {
                ret = imei->encode(theBuffer, theLen, theIndex);
                if (SUCCESS_E != ret)
                {
                    LOG_ERROR("failed to encode ServiceNotification.imei");
                    return ret;
                }
            }
            if (apn)
            {
                ret = apn->encode(theBuffer, theLen, theIndex);
                if (SUCCESS_E != ret)
                {
                    LOG_ERROR("failed to encode ServiceNotification.apn");
                    return ret;
                }
            }
            if (httpHeader)
            {
                ret = httpHeader->encode(theBuffer, theLen, theIndex);
                if (SUCCESS_E != ret)
                {
                    LOG_ERROR("failed to encode ServiceNotification.httpHeader");
                    return ret;
                }
            }

            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode ServiceNotification.length");
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
                theOut << "\n" <<leadStr << "ServiceNotification";
            }
            else
            {
                theOut << "ServiceNotification";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "userId: ";
            userId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "serviceType: ";
            serviceType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "serviceInfo: ";
            serviceInfo.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "location: ";
            location.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "timestamp: ";
            timestamp.dump(theOut, theLayer + 1);

            if (mSISDN)
            {
                theOut << "\n" << leadStr << "mSISDN: ";
                mSISDN->dump(theOut, theLayer + 1);
            }
            if (imei)
            {
                theOut << "\n" << leadStr << "imei: ";
                imei->dump(theOut, theLayer + 1);
            }
            if (apn)
            {
                theOut << "\n" << leadStr << "apn: ";
                apn->dump(theOut, theLayer + 1);
            }
            if (httpHeader)
            {
                theOut << "\n" << leadStr << "httpHeader: ";
                httpHeader->dump(theOut, theLayer + 1);
            }
            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        MobileIdentityIEI userId;
        Uint8 serviceType;
        ServiceInfoIEI serviceInfo;
        LocationIEI location;
        DateTimeIEI timestamp;
        boost::optional<PhoneNumberIEI> mSISDN;
        boost::optional<MobileIdentityIEI> imei;
        boost::optional<ApnIEI> apn;
        boost::optional<HttpHeaderIEI> httpHeader;
    }; /* end of class ServiceNotification */


    class AddOrModifyUser
    {
    public:
        AddOrModifyUser(){}
        ~AddOrModifyUser(){}

        enum{ ID = 0x10};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        MobileIdentityIEI::MIN_BYTES +
                        PropertiesIEI::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            userId.init();
            properties.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse AddOrModifyUser.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse AddOrModifyUser.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse AddOrModifyUser.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse AddOrModifyUser.messageType");
                return ret;
            }

            ret = userId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse AddOrModifyUser.userId");
                return ret;
            }

            ret = properties.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse AddOrModifyUser.properties");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode AddOrModifyUser.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode AddOrModifyUser.transactionId");
                return ret;
            }

            messageType.valueM = AddOrModifyUser::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode AddOrModifyUser.messageType");
                return ret;
            }

            ret = userId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode AddOrModifyUser.userId");
                return ret;
            }

            ret = properties.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode AddOrModifyUser.properties");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode AddOrModifyUser.length");
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
                theOut << "\n" <<leadStr << "AddOrModifyUser";
            }
            else
            {
                theOut << "AddOrModifyUser";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "userId: ";
            userId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "properties: ";
            properties.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        MobileIdentityIEI userId;
        PropertiesIEI properties;
    }; /* end of class AddOrModifyUser */


    class AddOrModifyUserAck
    {
    public:
        AddOrModifyUserAck(){}
        ~AddOrModifyUserAck(){}

        enum{ ID = 0x11};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse AddOrModifyUserAck.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse AddOrModifyUserAck.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse AddOrModifyUserAck.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse AddOrModifyUserAck.messageType");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode AddOrModifyUserAck.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode AddOrModifyUserAck.transactionId");
                return ret;
            }

            messageType.valueM = AddOrModifyUserAck::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode AddOrModifyUserAck.messageType");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode AddOrModifyUserAck.length");
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
                theOut << "\n" <<leadStr << "AddOrModifyUserAck";
            }
            else
            {
                theOut << "AddOrModifyUserAck";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
    }; /* end of class AddOrModifyUserAck */


    class StrategyModifyREQ
    {
    public:
        StrategyModifyREQ(){}
        ~StrategyModifyREQ(){}

        enum{ ID = 0x13};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        StrategyIEI::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            strategy.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyREQ.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse StrategyModifyREQ.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyREQ.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyREQ.messageType");
                return ret;
            }

            ret = strategy.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyREQ.strategy");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyREQ.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyREQ.transactionId");
                return ret;
            }

            messageType.valueM = StrategyModifyREQ::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyREQ.messageType");
                return ret;
            }

            ret = strategy.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyREQ.strategy");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode StrategyModifyREQ.length");
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
                theOut << "\n" <<leadStr << "StrategyModifyREQ";
            }
            else
            {
                theOut << "StrategyModifyREQ";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "strategy: ";
            strategy.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        StrategyIEI strategy;
    }; /* end of class StrategyModifyREQ */


    class StrategyModifyAccept
    {
    public:
        StrategyModifyAccept(){}
        ~StrategyModifyAccept(){}

        enum{ ID = 0x14};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyAccept.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse StrategyModifyAccept.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyAccept.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyAccept.messageType");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyAccept.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyAccept.transactionId");
                return ret;
            }

            messageType.valueM = StrategyModifyAccept::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyAccept.messageType");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode StrategyModifyAccept.length");
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
                theOut << "\n" <<leadStr << "StrategyModifyAccept";
            }
            else
            {
                theOut << "StrategyModifyAccept";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
    }; /* end of class StrategyModifyAccept */


    class StrategyModifyReject
    {
    public:
        StrategyModifyReject(){}
        ~StrategyModifyReject(){}

        enum{ ID = 0x15};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        CauseIEI::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            cause.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyReject.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse StrategyModifyReject.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyReject.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyReject.messageType");
                return ret;
            }

            ret = cause.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse StrategyModifyReject.cause");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyReject.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyReject.transactionId");
                return ret;
            }

            messageType.valueM = StrategyModifyReject::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyReject.messageType");
                return ret;
            }

            ret = cause.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode StrategyModifyReject.cause");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode StrategyModifyReject.length");
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
                theOut << "\n" <<leadStr << "StrategyModifyReject";
            }
            else
            {
                theOut << "StrategyModifyReject";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "cause: ";
            cause.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        CauseIEI cause;
    }; /* end of class StrategyModifyReject */


    class UserDataNotification
    {
    public:
        UserDataNotification(){}
        ~UserDataNotification(){}

        enum{ ID = 0x31};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        BcdUserId::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        PlainString::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            userId.init();
            ip.init();
            lac.init();
            rac.init();
            ci.init();
            status.init();
            content.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse UserDataNotification.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.messageType");
                return ret;
            }

            ret = userId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.userId");
                return ret;
            }

            ret = ip.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.ip");
                return ret;
            }

            ret = lac.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.lac");
                return ret;
            }

            ret = rac.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.rac");
                return ret;
            }

            ret = ci.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.ci");
                return ret;
            }

            ret = status.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.status");
                return ret;
            }

            ret = content.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserDataNotification.content");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.transactionId");
                return ret;
            }

            messageType.valueM = UserDataNotification::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.messageType");
                return ret;
            }

            ret = userId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.userId");
                return ret;
            }

            ret = ip.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.ip");
                return ret;
            }

            ret = lac.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.lac");
                return ret;
            }

            ret = rac.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.rac");
                return ret;
            }

            ret = ci.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.ci");
                return ret;
            }

            ret = status.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.status");
                return ret;
            }

            ret = content.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserDataNotification.content");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode UserDataNotification.length");
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
                theOut << "\n" <<leadStr << "UserDataNotification";
            }
            else
            {
                theOut << "UserDataNotification";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "userId: ";
            userId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "ip: ";
            ip.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "lac: ";
            lac.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "rac: ";
            rac.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "ci: ";
            ci.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "status: ";
            status.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "content: ";
            content.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        BcdUserId userId;
        Uint32 ip;
        Uint16 lac;
        Uint16 rac;
        Uint16 ci;
        Uint16 status;
        PlainString content;
    }; /* end of class UserDataNotification */


    class UserStatusNotification
    {
    public:
        UserStatusNotification(){}
        ~UserStatusNotification(){}

        enum{ ID = 0x32};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        BcdUserId::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        PlainString::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();
            transactionId.init();
            messageType.init();
            userId.init();
            ip.init();
            lac.init();
            rac.init();
            ci.init();
            status.init();
            content.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.length");
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse UserStatusNotification.length");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = transactionId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.transactionId");
                return ret;
            }

            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.messageType");
                return ret;
            }

            ret = userId.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.userId");
                return ret;
            }

            ret = ip.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.ip");
                return ret;
            }

            ret = lac.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.lac");
                return ret;
            }

            ret = rac.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.rac");
                return ret;
            }

            ret = ci.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.ci");
                return ret;
            }

            ret = status.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.status");
                return ret;
            }

            ret = content.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse UserStatusNotification.content");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.length");
                return ret;
            }

            ret = transactionId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.transactionId");
                return ret;
            }

            messageType.valueM = UserStatusNotification::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.messageType");
                return ret;
            }

            ret = userId.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.userId");
                return ret;
            }

            ret = ip.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.ip");
                return ret;
            }

            ret = lac.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.lac");
                return ret;
            }

            ret = rac.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.rac");
                return ret;
            }

            ret = ci.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.ci");
                return ret;
            }

            ret = status.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.status");
                return ret;
            }

            ret = content.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode UserStatusNotification.content");
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)
            {
                LOG_DEBUG("failed to encode UserStatusNotification.length");
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
                theOut << "\n" <<leadStr << "UserStatusNotification";
            }
            else
            {
                theOut << "UserStatusNotification";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "transactionId: ";
            transactionId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "userId: ";
            userId.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "ip: ";
            ip.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "lac: ";
            lac.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "rac: ";
            rac.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "ci: ";
            ci.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "status: ";
            status.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "content: ";
            content.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;
        Uint32 transactionId;
        MsgId32 messageType;
        BcdUserId userId;
        Uint32 ip;
        Uint16 lac;
        Uint16 rac;
        Uint16 ci;
        Uint16 status;
        PlainString content;
    }; /* end of class UserStatusNotification */


    class LocationSubMsg
    {
    public:
        LocationSubMsg(){}
        ~LocationSubMsg(){}

        enum
        {
            MIN_BYTES =
                        Uint8::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            flag.init();
            mNC.init();
            mCC.init();
            lAC.init();
            rAC.init();
            cI.init();
            sI.init();
        } /* end of void init(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = flag.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse LocationSubMsg.flag");
                return ret;
            }

            ret = mNC.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse LocationSubMsg.mNC");
                return ret;
            }

            ret = mCC.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse LocationSubMsg.mCC");
                return ret;
            }

            ret = lAC.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse LocationSubMsg.lAC");
                return ret;
            }

            ret = rAC.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse LocationSubMsg.rAC");
                return ret;
            }

            ret = cI.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse LocationSubMsg.cI");
                return ret;
            }

            ret = sI.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to parse LocationSubMsg.sI");
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = flag.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode LocationSubMsg.flag");
                return ret;
            }

            ret = mNC.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode LocationSubMsg.mNC");
                return ret;
            }

            ret = mCC.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode LocationSubMsg.mCC");
                return ret;
            }

            ret = lAC.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode LocationSubMsg.lAC");
                return ret;
            }

            ret = rAC.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode LocationSubMsg.rAC");
                return ret;
            }

            ret = cI.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode LocationSubMsg.cI");
                return ret;
            }

            ret = sI.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
            {
                LOG_ERROR("failed to encode LocationSubMsg.sI");
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
                theOut << "\n" <<leadStr << "LocationSubMsg";
            }
            else
            {
                theOut << "LocationSubMsg";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "flag: ";
            flag.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "mNC: ";
            mNC.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "mCC: ";
            mCC.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "lAC: ";
            lAC.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "rAC: ";
            rAC.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "cI: ";
            cI.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "sI: ";
            sI.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Uint8 flag;
        Uint16 mNC;
        Uint16 mCC;
        Uint16 lAC;
        Uint16 rAC;
        Uint16 cI;
        Uint16 sI;
    }; /* end of class LocationSubMsg */


    class QosSubMsg
    {
    public:
        QosSubMsg(){}
        ~QosSubMsg(){}

        enum
        {
            MIN_BYTES =
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
    }; /* end of class QosSubMsg */

}

#endif /* MESSAGE_H */

