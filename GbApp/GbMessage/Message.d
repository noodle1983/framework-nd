#key:
#   SubMessage 
#   Message

##############################################
#MessageList
#    MessageName                TypeValue
##############################################
MessageList
    Heartbeat                   0x00
    PDPActivateNotification     0x01
    QoSModifyREQ                0x02
    QosModifyAccept             0x03
    QoSModifyReject             0x04
    ServiceQuery                0x05
    ServiceNotification         0x06
    AddOrModifyUser             0x10
    AddOrModifyUserAck          0x11
    StrategyModifyREQ           0x13
    StrategyModifyAccept        0x14
    StrategyModifyReject        0x15
    SubscribeRawData            0x20
    SubscribeRawDataAck         0x21
    CancelSubscribe             0x23
    CancelSubscribeAck          0x24
    UserDataNotification        0x31
    UserStatusNotification      0x32
    InjectSignalingREQ          0x41
    InjectSignalingAck          0x42

##############################################
#SubMessage Name
#    FieldName    FieldType    InitValue
##############################################
SubMessage Header
    Length           uint32_t    Uint32Codec    M
    TransactionId    uint32_t    Uint32Codec    M
    MessageType      uint32_t    Uint32Codec    M

##############################################
#Message Name
#    FieldName    FieldType    InitValue
##############################################
Message   QoSModifyREQ

