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
    PDPDeactivateNotification   0x07
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
    CdrMsg                       0xF0
    SignalEventMsg               0xF1

##############################################
#MessageList
#    MessageName                TypeValue   
##############################################
SubMessageList
    MobileIdentityIEI    MobileIdentity       0x10
    LocationIEI          Location    0x11
    QosIEI               Qos         0x12
#    ServiceInfoIEI              0x13
    DateTimeIEI          Timestamp   0x14
#    PhoneNumberIEI       Uint8       0x15
    UserAgentIEI         Uint16      0x16
    ApnIEI               Uint16      0x17
    UrlIEI               Uint16      0x18
    PropertiesIEI        Uint16      0x20
    StrategyIEI          Uint16      0x21
    HttpHeaderIEI        Uint16      0x22

    TriggerIEI           Uint16      0x23
    CauseIEI             Uint16      0x24

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
    UserId           BcdUserId M         
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

Message PDPDeactivateNotification
    include MsgBHeader
    UserId         MobileIdentityIEI       M  

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
    CatalogId      Uint48                  M
    AppId          Uint32                  M
    BehaviorId     Uint32                  M
    UpPktCnt       Uint32                  M
    DownPktCnt     Uint32                  M
    UpBytes        Uint32                  M
    DownBytes      Uint32                  M
    Rtd            Uint32                  M

Message ServiceNotification
    include MsgBHeader
    UserId         MobileIdentityIEI       M  
    ServiceType    Uint8                   M
    ServiceInfo    ServiceInfoIEI          M
    Location       LocationIEI             M
    Timestamp      DateTimeIEI             M
    MSISDN         PhoneNumberIEI          O
    Imei           MobileIdentityIEI       O
    Apn            ApnIEI                  O
    HttpHeader     HttpHeaderIEI           O


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

Message LocationSubMsg
    Flag           Uint8                   M
    MNC            Uint16                  M
    MCC            Uint16                  M
    LAC            Uint16                  M
    RAC            Uint16                  M
    CI             Uint16                  M
    SI             Uint16                  M

Message QosSubMsg
    Arp                       Uint8        M
    Delay                     Uint8        M
    Reliability               Uint8        M
    Peak                      Uint8        M
    Precedence                Uint8        M
    Mean                      Uint8        M
    Traffic                   Uint8        M
    Deliveryorder             Uint8        M
    DeliveryOfErroneousSdu    Uint8        M
    MaxSduSize                Uint8        M
    MaxBitRateUp              Uint8        M
    MaxBitRateDown            Uint8        M
    ResidualBer               Uint8        M
    SduErrRatio               Uint8        M
    TransferDelay             Uint8        M
    TrafficHandlingPriority   Uint8        M
    GuaranteedBitRateUp       Uint8        M
    GuaranteedBitRateDown     Uint8        M

Message CdrMsg
    Type                                    Uint8         M
    Subtype                                 Uint8         M
    Protocol                                Uint8         M
    StatusControl                           Uint8         M
    ReportTime                              DateTimeIEI   M
    Device                                  Uint16        M
    Channel                                 Uint16        M
    ID                                      Uint32        M
    StartTime                               DateTimeIEI   M
    TMSI                                    Uint64        M
    IMSI                                    Uint64        M
    IMEI                                    Uint64        M
    MSISDN                                  Uint64        M
    CDRType                                 Uint8         M
    CDRResult                               Uint8         M
    CDRIndicator                            Uint32        M
    RABAssignmentRequestTime                Uint32        M
    RABAssignmentResponseTime               Uint32        M
    RABReleaseTime                          Uint32        M
    IdentityRequestTime                     Uint32        M
    IdentityResponseTime                    Uint32        M
    AuthenticationRequestTime               Uint32        M
    AuthenticationResponseTime              Uint32        M
    PDPActive                               Uint32        M
    PDPDeactive                             Uint32        M
    IMSIAttachTime                          Uint32        M
    IMSIDetachTime                          Uint32        M
    MCC                                     Uint16        M
    MNC                                     Uint8         M
    LAC                                     Uint16        M
    CI                                      Uint16        M
    LastMCC                                 Uint16        M
    LastMNC                                 Uint8         M
    LastLAC                                 Uint16        M
    LastCI                                  Uint16        M
    PCMTS                                   Uint16        M
    AttachRejectCause                       Uint16        M
    RoutingAreaUpdateRejectCause            Uint16        M
    ServiceRejectCause                      Uint16        M
    AuthenticationAndCipheringRejectCause   Uint16        M
    AuthenticationFlag                      Uint8         M

Message SignalEventMsg
    Type               Uint8         M
    Subtype            Uint8         M
    Protocol           Uint8         M
    StatusControl      Uint8         M
    ReportTime         DateTimeIEI   M
    StartTime          DateTimeIEI   M
    Device             Uint16        M
    Channel            Uint16        M
    TMSI               Uint32        M
    IMSI               Uint64        M
    IMEI               Uint64        M
    MSISDN             Uint64        M
    SequenceNumber     Uint32        M
    CDRType            Uint8         M
    MCC                Uint16        M
    MNC                Uint8         M
    LAC                Uint16        M
    CI                 Uint16        M
    RAC                Uint8         M
    RequestTime        Uint32        M
    RequestType        Uint8         M
    AcceptTime         Uint32        M
    CompleteTime       Uint32        M
    RejectedTime       Uint32        M
    RejectedCause      Uint16        M
    BaseCdrId          Uint32        M

