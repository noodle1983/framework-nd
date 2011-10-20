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
    QosModifyREQ                0x02
    QosModifyAccept             0x03
    QosModifyReject             0x04
#    ServiceQuery                0x05
    ServiceNotification         0x06
    AddOrModifyUser             0x10
    AddOrModifyUserAck          0x11
    StrategyModifyREQ           0x13
    StrategyModifyAccept        0x14
    StrategyModifyReject        0x15
#    SubscribeRawData            0x20
#    SubscribeRawDataAck         0x21
#    CancelSubscribe             0x23
#    CancelSubscribeAck          0x24
    UserDataNotification        0x31
    UserStatusNotification      0x32
#    InjectSignalingREQ          0x41
#    InjectSignalingAck          0x42

##############################################
#MessageList
#    MessageName                TypeValue   
##############################################
SubMessageList
    MobileIdentityIEI    Uint8       0x10
    LocationIEI          Uint8       0x11
    QosIEI               Uint8       0x12
#    ServiceInfoIEI              0x13
#    DateTimeIEI                 0x14
    PhoneNumberIEI       Uint8       0x15
    UserAgentIEI         Uint16      0x16
    ApnIEI               Uint16      0x17
    UrlIEI               Uint16      0x18
    PropertiesIEI        Uint16      0x20
    StrategyIEI          Uint16      0x21
    RtdIEI               Uint16      0x22
    TriggerIEI           Uint16      0x23
    CauseIEI             Uint16      0x23

##############################################
#SubMessage Name
#    FieldName    FieldType    Optional
##############################################
SubMessage Header
    Length           Length32  M
    TransactionId    Uint32    M
    MessageType      MsgId32   M

SubMessage MsgAHeader
    include Header
    Imsi             Uint64    M
    Nsapi            Uint32    M

SubMessage MsgBHeader
    include Header

SubMessage MsgCHeader
    include Header

SubMessage MsgDHeader
    include Header

SubMessage MsgE1Header
    include Header
    UserId           Uint64    M         
    Ip               Uint32    M         
    Lac              Uint16    M         
    Rac              Uint16    M         
    Ci               Uint16    M         
    Status           Uint16    M

SubMessage MsgE2Header
    include Header

##############################################
#Message Name
#    FieldName    FieldType    InitValue
##############################################
Message MsgHeader
    Length           Length32  M
    TransactionId    Uint32    M
    MessageType      Uint32    M

Message  Heartbeat
    include Header

Message PDPActivateNotification
    include MsgBHeader
    UserId         MobileIdentityIEI       M  
    Qos            QosIEI                  M
    Location       LocationIEI             M

Message QosModifyREQ 
    include MsgBHeader
    UserId         MobileIdentityIEI       M
    Qos            QosIEI                  M
    Trigger        TriggerIEI              O

Message QosModifyAccept
    include MsgBHeader

Message QosModifyReject
    include MsgBHeader
    Cause          CauseIEI                M

Message ServiceInfoIEI
    SrcIpAddr      Uint32                  M
    DestIpAddr     Uint32                  M
    SrcPort        Uint16                  M
    DescPort       Uint16                  M
    Protocol       Uint8                   M
    CatalogId      Uint32                  M
    AppId          Uint32                  M
    BehaviorId     Uint32                  M
    UpPktCnt       Uint32                  M
    DownPktCnt     Uint32                  M
    UpBytes        Uint32                  M
    DownBytes      Uint32                  M

Message ServiceNotification
    include MsgBHeader
    UserId         MobileIdentityIEI       M  
    ServiceType    Uint8                   M
    ServiceInfo    ServiceInfoIEI          M
    Location       LocationIEI             M
    Timestamp      Uint64                  M
    MSISDN         PhoneNumberIEI          O
    Imei           MobileIdentityIEI       O
    UserAgent      UserAgentIEI            O
    Apn            ApnIEI                  O
    Url            UrlIEI                  O
    Rtd            RtdIEI                  O

Message AddOrModifyUser
    include MsgCHeader
    UserId         MobileIdentityIEI       M  
    Properties     PropertiesIEI           M 

Message AddOrModifyUserAck
    include MsgCHeader

Message StrategyModifyREQ
    include MsgCHeader
    Strategy       StrategyIEI             M

Message StrategyModifyAccept
    include MsgCHeader

Message StrategyModifyReject
    include MsgCHeader
    Cause          CauseIEI                M

Message UserDataNotification
    include MsgE1Header
    Content        PlainString             M

Message UserStatusNotification
    include MsgE1Header
    Content        PlainString             M
