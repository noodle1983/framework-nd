#!/usr/bin/perl 

my %msgName2Type;
my %msgType2Name;
my %submsgName2Type;
my %submsgType2Name;

#hash table for all the message defination
#structure
#msgName=>[(FieldName, FieldType, FieldCodec, M/O)]
my %msgsDef;
my %submsgsDef;

#load defination
parseMsgDef();

dumpMsgDef();

#print message
open CMSG_HANDLE, " > Message.h" or die "failed to open Message.h:$!\n";

    genHeaders();

    while((my $msgName, my $msgDef) = each %submsgsDef)
    {
        genMsg($msgName, $msgDef);
    }

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
#include <stdint.h>
#include "IntCodec.h"

END_OF_HEADER
}

################################################################################
sub genMsg
{
    my $msgName = shift;
    my $msgDef = shift;
    
    genInitFunction($msgName, $msgDef);
    genDecodeFunction($msgName, $msgDef);
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
        ($fieldName, $fieldType, $fieldCodec, $fieldOption) = @$_;
        if ($fieldOption eq "M")
        {
print  CMSG_HANDLE<<END_OF_INIT_BODY;
            ${fieldCodec}::init(${fieldName});            
END_OF_INIT_BODY
        }
    }

print CMSG_HANDLE <<END_OF_INIT_END;
        }

END_OF_INIT_END

}

################################################################################
sub genDecodeFunction
{
    my $msgName = shift;
    my $msgDef = shift;
    my $optianlExisted = 0;

print CMSG_HANDLE<<END_OF_DECODE_BEG;
        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
END_OF_DECODE_BEG
    
    foreach(@$msgDef)
    {
        ($fieldName, $fieldType, $fieldCodec, $fieldOption) = @$_;
        if ($fieldOption eq "M")
        {
print  CMSG_HANDLE<<END_OF_DECODE_BODY;
            if (0 != ${fieldCodec}::decode(
                    theBuffer, theLen, theIndex, ${fieldName}))            
            {
                DEBUG("failed to parse ${msgName}.${fieldName}");
                return -1;
            }

END_OF_DECODE_BODY
        }
        elsif ($fieldOption eq "O" && $optianlExisted == 0)
        {
            $optianlExisted = 1;
            my $msgType = $submsgName2Type{ucfirst($fieldName)};
print  CMSG_HANDLE<<END_OF_DECODE_WOBODY;
            while(theIndex < theLen)
            {
                if (theBuffer[theIndex] == $msgType) 
                {
                    if (0 != ${fieldCodec}::decode(
                            theBuffer, theLen, theIndex, ${fieldName}))            
                    {
                        DEBUG("failed to parse ${msgName}.${fieldName}");
                        return -1;
                    }
                }
END_OF_DECODE_WOBODY
        }
        elsif ($fieldOption eq "O")
        {
            my $msgType = $submsgName2Type{ucfirst($fieldName)};
print  CMSG_HANDLE<<END_OF_DECODE_OBODY;
                else if (theBuffer[theIndex] == $msgType) 
                {
                    if (0 != ${fieldCodec}::decode(
                            theBuffer, theLen, theIndex, ${fieldName}))            
                    {
                        DEBUG("failed to parse ${msgName}.${fieldName}");
                        return -1;
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
                    DEBUG("failed to parse structure at index" << theIndex);
                    return -1;
                }
           } 
END_OF_DECODE_OBODY_E

    }


print CMSG_HANDLE <<END_OF_DECODE_END;
            return 0;
        }

END_OF_DECODE_END

}


################################################################################
#######################       parse message       ##############################
################################################################################
sub parseMsgDef
{
    my $state = "NONE";
    my $curMsg;
    my $lineNo = 0;
    open MSG_DEF_HANDLER, "< Message.d" or die "failed to open Message.d:$!\n";
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
            if ($line =~ /^\s+(\w+)\s+(\w+)\s*$/)
            {
                $submsgName2Type{$1} = $2;
            }
            else
            {
                $state = "NONE";
            }
        }

        if ($state eq "Message")
        {
            if ($line =~ /^\s+(\w+)\s+(\w+)\s+(\w+)\s+(\w+)\s*$/)
            {
                push @{$msgsDef{$curMsg}}, [lcfirst($1), $2, $3, $4];
            }
            else
            {
                $state = "NONE";
            }
        }

        if ($state eq "SubMessage")
        {
            if ($line =~ /^\s+(\w+)\s+(\w+)\s+(\w+)\s+(\w+)\s*$/)
            {
                push @{$submsgsDef{$curMsg}}, [lcfirst($1), $2, $3, $4];
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
    while(($key, $value) = each %submsgName2Type)
    {
        print "$key\t$value\n";
    }

    print "-" x 35 . "MessageDef" . "-" x 35 . "\n";
    while(($key, $value) = each %msgsDef)
    {
        print "$key\n";
        foreach(@$value)
        {
            ($a, $b, $c, $d) = @$_;
            print "\t$a\t$b\t$c\t$d\n";
        }
        
    }
    
    print "-" x 35 . "SubMessageDef" . "-" x 35 . "\n";
    while(($key, $value) = each %submsgsDef)
    {
        print "$key\n";
        foreach(@$value)
        {
            ($a, $b, $c, $d) = @$_;
            print "\t$a\t$b\t$c\t$d\n";
        }
        
    }
}
