#!/usr/bin/perl 

my $msgDefFile = shift;
my $msgOutFile = $msgDefFile;
$msgOutFile =~ s/msg$/h/;

my %msgName2Type;
my @submsgName2Type;

#hash table for all the message defination
#structure
#msgName=>[(FieldName, FieldType, M/O)]
my %msgsDef;
my @msgsSeq;
my %submsgsDef;

#load defination
parseMsgDef();

#dumpMsgDef();

#print message
open CMSG_HANDLE, " > $msgOutFile" or die "failed to open $msgOutFile:$!\n";

    genHeaders();

    foreach my $TypeLenTypeValue (@submsgName2Type)
    {
        (my $subType, my $lengthType, my $subValue) = @$TypeLenTypeValue;
        genSubMsg($subType, $lengthType, $subValue);
    }
    foreach my $msgName  (@msgsSeq)
    {
        genMsg($msgName, $msgsDef{$msgName});
    }

    genEnd();
    #genRootMsg();
close CMSG_HANDLE;

exit 0;

################################################################################
#######################       generate C++        ##############################
################################################################################
sub genHeaders
{
print CMSG_HANDLE<<END_OF_HEADER;
// this file is generate automatically, please don't change it mannually
#ifndef MESSAGE_H
#define MESSAGE_H

#include "StrMsg.h"
#include "IntMsg.h"
#include "MsgErrorCode.h"
#include "MobileIdentity.h"
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

END_OF_HEADER
}

sub genEnd
{
print CMSG_HANDLE<<END_OF_MESSAGE_E;
}

#endif /* MESSAGE_H */

END_OF_MESSAGE_E
}
################################################################################
#######################       generate genSubMsg       #######################
################################################################################
sub genSubMsg
{
    my $submsgType = shift;
    my $submsgLenType = shift;
    my $submsgValue = shift;

    if ($submsgLenType =~ /Uint/)
    {
print CMSG_HANDLE<<END_OF_SUBMSG;
    typedef TlvString<${submsgValue}, ${submsgLenType}> ${submsgType};
END_OF_SUBMSG
    }
    else
    {
print CMSG_HANDLE<<END_OF_SUBMSG;
    typedef ${submsgLenType}<${submsgValue}> ${submsgType};
END_OF_SUBMSG
    }

}

################################################################################
#######################       generate genMsg       #######################
################################################################################
sub genMsg
{
    my $msgName = shift;
    my $msgDef = shift;
    
print CMSG_HANDLE<<END_OF_MSGDEF_CLASS_B;

    class ${msgName}
    {
    public:
        ${msgName}(){}
        ~${msgName}(){}

END_OF_MSGDEF_CLASS_B
    my $msgType = $msgName2Type{$msgName};
    if ($msgType)
    {
print CMSG_HANDLE<<END_OF_MSGDEF_ENUM;
        enum{ ID = ${msgType}};

END_OF_MSGDEF_ENUM

    }
    genMinSize($msgName, $msgDef);

    genInitFunction($msgName, $msgDef);
    genDecodeFunction($msgName, $msgDef);
    genEncodeFunction($msgName, $msgDef);
    genDumpFunction($msgName, $msgDef);
    genFieldDef($msgName, $msgDef);

print CMSG_HANDLE<<END_OF_MSGDEF_CLASS_E;
    }; /* end of class ${msgName} */

END_OF_MSGDEF_CLASS_E
}
################################################################################
sub genMinSize
{
    my $msgName = shift;
    my $msgDef = shift;

print CMSG_HANDLE<<END_OF_MINSIZE_BEG;
        enum
        {
            MIN_BYTES =
END_OF_MINSIZE_BEG
    
    foreach(@$msgDef)
    {
        ($fieldName, $fieldType, $fieldOption) = @$_;
        if ($fieldOption eq "M")
        {
print  CMSG_HANDLE<<END_OF_MINSIZE_BODY;
                        ${fieldType}::MIN_BYTES +
END_OF_MINSIZE_BODY
        }

    }

print CMSG_HANDLE <<END_OF_MINSIZE_END;
                        0
        }; /* end of enum MIN_BYTES */

END_OF_MINSIZE_END


}
################################################################################
sub genDumpFunction
{
    my $msgName = shift;
    my $msgDef = shift;

print CMSG_HANDLE<<END_OF_DUMP_BEG;
        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\\n" <<leadStr << "${msgName}";
            }
            else
            {
                theOut << "${msgName}";
            }
            leadStr.append("    ");
END_OF_DUMP_BEG
    
    foreach(@$msgDef)
    {
        ($fieldName, $fieldType, $fieldOption) = @$_;
        if ($fieldOption eq "M")
        {
print  CMSG_HANDLE<<END_OF_DUMP_BODY;

            theOut << "\\n" << leadStr << "${fieldName}: ";
            $fieldName.dump(theOut, theLayer + 1);

END_OF_DUMP_BODY
        }
        elsif ($fieldOption eq "O")
        {
print  CMSG_HANDLE<<END_OF_DUMP_OBODY;
            if (${fieldName})
            {
                theOut << "\\n" << leadStr << "${fieldName}: ";
                $fieldName->dump(theOut, theLayer + 1);           
            }
END_OF_DUMP_OBODY

        }

    }

print CMSG_HANDLE <<END_OF_DUMP_END;
            if (0 == theLayer)
            {
                theOut << "\\n";
            }
            return theOut;
        } /* end of dump(...) */

END_OF_DUMP_END


}
################################################################################
sub genFieldDef
{
    my $msgName = shift;
    my $msgDef = shift;

print CMSG_HANDLE<<END_OF_MSGFIELD_B;

    public:
END_OF_MSGFIELD_B

    foreach(@$msgDef)
    {
        ($fieldName, $fieldType, $fieldOption) = @$_;
        if ($fieldOption eq "M")
        {
print  CMSG_HANDLE<<END_OF_FIELDDEF_BODY;
        ${fieldType} ${fieldName};           
END_OF_FIELDDEF_BODY
        }
        elsif ($fieldOption eq "O")
        {
print  CMSG_HANDLE<<END_OF_FIELDDEF_OBODY;
        boost::optional<${fieldType}> ${fieldName};           
END_OF_FIELDDEF_OBODY
        }
        else
        {
            die "invalid Optional Definination for $msgName.$fieldName";
        }
    }

}
################################################################################
sub genInitFunction
{
    my $msgName = shift;
    my $msgDef = shift;

print CMSG_HANDLE<<END_OF_INIT_BEG;
        void init()
        {
END_OF_INIT_BEG
    
    foreach(@$msgDef)
    {
        ($fieldName, $fieldType, $fieldOption) = @$_;
        if ($fieldOption eq "M")
        {
print  CMSG_HANDLE<<END_OF_INIT_BODY;
            ${fieldName}.init();            
END_OF_INIT_BODY
        }
    }

print CMSG_HANDLE <<END_OF_INIT_END;
        } /* end of void init(...) */

END_OF_INIT_END

}

################################################################################
sub genDecodeFunction
{
    my $msgName = shift;
    my $msgDef = shift;
    my $theMsg = "the$msgName";
    my $optianlExisted = 0;
    my $msgLengthVar = "theLen";

print CMSG_HANDLE<<END_OF_DECODE_BEG;
        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
END_OF_DECODE_BEG
    
    foreach(@$msgDef)
    {
        ($fieldName, $fieldType, $fieldOption) = @$_;
        if ($fieldOption eq "M")
        {
print  CMSG_HANDLE<<END_OF_DECODE_BODY;
            ret = ${fieldName}.decode(theBuffer, ${msgLengthVar}, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ${msgName}.${fieldName}");
                return ret;
            }

END_OF_DECODE_BODY
            if ($fieldType =~ /^Length/)
            {
print  CMSG_HANDLE<<END_OF_DECODE_LENGTH;
            unsigned endIndex = theIndex - ${fieldType}::MIN_BYTES + ${fieldName}.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("failed to parse ${msgName}.${fieldName}");
                return NOT_ENOUGH_BUFFER_E;
            }
END_OF_DECODE_LENGTH
                $msgLengthVar = "endIndex";
            }
           
        }
        elsif ($fieldOption eq "O" && $optianlExisted == 0)
        {
            $optianlExisted = 1;
print  CMSG_HANDLE<<END_OF_DECODE_WOBODY;
            while(theIndex < ${msgLengthVar})
            {
                if (theBuffer[theIndex] == ${fieldType}::TAG) 
                {
                    ${fieldName}.reset(${fieldType}());
                    ret = $fieldName->decode( theBuffer, ${msgLengthVar}, theIndex);
                    if (SUCCESS_E != ret)            
                    {
                        LOG_ERROR("failed to parse ${msgName}.${fieldName}");
                        return ret;
                    }
                }
END_OF_DECODE_WOBODY
        }
        elsif ($fieldOption eq "O")
        {
print  CMSG_HANDLE<<END_OF_DECODE_OBODY;
                else if (theBuffer[theIndex] == ${fieldType}::TAG) 
                {
                    ${fieldName}.reset(${fieldType}());
                    ret = $fieldName->decode( theBuffer, ${msgLengthVar}, theIndex);
                    if (SUCCESS_E != ret)            
                    {
                        LOG_ERROR("failed to parse ${msgName}.${fieldName}");
                        return ret; 
                    }
                }
END_OF_DECODE_OBODY

        }

    }
    if ($optianlExisted == 1)
    {
print  CMSG_HANDLE<<END_OF_DECODE_OBODY_E;
                else
                {
                    LOG_ERROR("failed to parse structure at index" << theIndex);
                    return ERROR_E;
                }
           } 
END_OF_DECODE_OBODY_E
    }
    if ($msgLengthVar ne "theLen") 
    {
print CMSG_HANDLE <<CHECK_END_OF_DECODE_CHECK_END;
CHECK_END_OF_DECODE_CHECK_END
    }
print CMSG_HANDLE <<END_OF_DECODE_END;

            return ret;
        } /* end of int decode(...) */

END_OF_DECODE_END

}
################################################################################
sub genEncodeFunction
{
    my $msgName = shift;
    my $msgDef = shift;
    my $lenFieldName;
    my $lenFieldType;

print CMSG_HANDLE<<END_OF_ENCODE_BEG;
        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
END_OF_ENCODE_BEG
    
    foreach(@$msgDef)
    {
        ($fieldName, $fieldType, $fieldOption) = @$_;
        if ($fieldType =~ /^Length/)
        {
            $lenFieldName = $fieldName;
            $lenFieldType = $fieldType;
print  CMSG_HANDLE<<END_OF_ENCODE_LEN_B;
            unsigned startIndex = theIndex;
            ret = ${fieldName}.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ${msgName}.${fieldName}");
                return ret;
            }

END_OF_ENCODE_LEN_B
        }
        elsif ($fieldOption eq "M")
        {
            if ($fieldType =~ /^MsgId/)
            {
print  CMSG_HANDLE<<END_OF_ENCODE_SET_ID;
            ${fieldName}.valueM = ${msgName}::ID;
END_OF_ENCODE_SET_ID

            }
print  CMSG_HANDLE<<END_OF_ENCODE_BODY;
            ret = ${fieldName}.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ${msgName}.${fieldName}");
                return ret;
            }

END_OF_ENCODE_BODY
        }
        elsif ($fieldOption eq "O")
        {
print  CMSG_HANDLE<<END_OF_ENCODE_OBODY;
            if ($fieldName) 
            {
                ret = ${fieldName}->encode(theBuffer, theLen, theIndex);
                if (SUCCESS_E != ret)            
                {
                    LOG_ERROR("failed to encode ${msgName}.${fieldName}");
                    return ret;
                }
            }
END_OF_ENCODE_OBODY

        }
        else
        {
            die "Please set the Optional Flag for ${msgName}.${fieldName}\n";
        }

    }
    if ($lenFieldName)
    {
print  CMSG_HANDLE<<END_OF_ENCODE_LEN_E;

            //re-encode the length
            ${lenFieldName}.valueM = theIndex - startIndex;
            ret = ${lenFieldName}.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode ${msgName}.${lenFieldName}");
                return ret; 
            }

END_OF_ENCODE_LEN_E

    }

print CMSG_HANDLE <<END_OF_ENCODE_END;
            return ret;
        } /* end of int encode(...) */

END_OF_ENCODE_END

}


################################################################################
#######################       parse message       ##############################
################################################################################
sub parseMsgDef
{
    my $state = "NONE";
    my $curMsg;
    my $lineNo = 0;
    open MSG_DEF_HANDLER, "< $msgDefFile" or die "failed to open $msgDefFile:$!\n";
    while (<MSG_DEF_HANDLER>)
    {
        $lineNo++;
        chomp;
        my $line = $_;
        $line =~ s/\#.*$//g;
        next if $line =~ /^\s*$/;
        
        if ($state eq "MessageList")
        {
            if ($line =~ /^\s+(\w+)\s+(\w+)\s*$/)
            {
                $msgName2Type{$1} = $2;
            }
            else
            {
                $state = "NONE";
            }
        }
        
        if ($state eq "SubMessageList")
        {
            if ($line =~ /^\s+(\w+)\s+(\w+)\s+(\w+)\s*$/)
            {
                push @submsgName2Type, [$1, $2, $3];
            }
            else
            {
                $state = "NONE";
            }
        }

        if ($state eq "Message")
        {
            if ($line =~ /^\s+include\s+(\w+)\s*$/)
            {
                @{$msgsDef{$curMsg}} = (@{$msgsDef{$curMsg}}, @{$submsgsDef{$1}} );
            }
            elsif ($line =~ /^\s+(\w+)\s+(\w+)\s+(\w+)\s*$/)
            {
                push @{$msgsDef{$curMsg}}, [lcfirst($1), $2, $3];
            }
            else
            {
                $state = "NONE";
            }
        }

        if ($state eq "SubMessage")
        {
            if ($line =~ /^\s+include\s+(\w+)\s*$/)
            {
                @{$submsgsDef{$curMsg}} = (@{$submsgsDef{$curMsg}}, @{$submsgsDef{$1}} );
            }
            elsif ($line =~ /^\s+(\w+)\s+(\w+)\s+(\w+)\s*$/)
            {
                push @{$submsgsDef{$curMsg}}, [lcfirst($1), $2, $3];
            }
            else
            {
                $state = "NONE";
            }
        }
        
        if ($state eq "NONE")
        {
            if ($line =~ /^MessageList/)
            {
                $state = "MessageList";
            }
            elsif ($line =~ /^SubMessageList/)
            {
                $state = "SubMessageList";
            }
            elsif ($line =~ /^SubMessage\s+(\w+)\s*$/)
            {
                $curMsg = $1;
                $submsgsDef{$curMsg} = [];
                $state = "SubMessage";
            }
            elsif ($line =~ /^Message\s+(\w+)\s*$/)
            {
                $curMsg = $1;
                $msgsDef{$curMsg} = [];
                $state = "Message";
                push @msgsSeq, $curMsg;
            }
            else
            {
                die "parse error at line: $lineNo:$line\n";
            }
        }
    }

    close MSG_DEF_HANDLER;
}

################################################################################

sub dumpMsgDef
{
    print "-" x 35 . "Message2Type" . "-" x 35 . "\n";
    while(($key, $value) = each %msgName2Type)
    {
        print "$key\t$value\n";
    }

    print "-" x 35 . "SubMessage2Type" . "-" x 35 . "\n";
    foreach my $TypeLenTypeValue (@submsgName2Type)
    {
        (my $subType, my $lengthType, my $subValue) = @$TypeLenTypeValue;
        print "TlvString<$subValue, $lengthType> $subType\n";
    }
    while(($key, $value) = each %submsgName2Type)
    {
        print "$key\t$value\n";
    }

    print "-" x 35 . "SubMessageDef" . "-" x 35 . "\n";
    while(($key, $value) = each %submsgsDef)
    {
        print "$key\n";
        foreach(@$value)
        {
            ($a, $b, $c) = @$_;
            print "\t$a\t$b\t$c\n";
        }
        
    }

    print "-" x 35 . "MessageDef" . "-" x 35 . "\n";
    while(($key, $value) = each %msgsDef)
    {
        print "$key\n";
        foreach(@$value)
        {
            ($a, $b, $c) = @$_;
            print "\t$a\t$b\t$c\n";
        }
        
    }
    
}
