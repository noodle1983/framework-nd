#!/usr/bin/perl 

my %msgName2Type;
my %msgType2Name;
my %submsgName2Type;
my %submsgType2Name;

#hash table for all the message defination
#structure
#msgName=>[(FieldName, FieldType, FieldCodec, M/O)]
my %msgsDef;

#load defination
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
    
    if ($state eq "SubMessage"
        || $state eq "Message")
    {
        if ($line =~ /^\s+(\w+)\s+(\w+)\s+(\w+)\s+(\w+)\s*$/)
        {
            push @{$msgsDef{$curMsg}}, [$1, $2, $3, $4];
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
        elsif ($line =~ /^SubMessage\s+(\w+)\s*$/)
        {
            $curMsg = $1;
            $msgsDef{$curMsg} = [];
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
            die "parse error at line: $lineNo\n";
        }
    }
}


while(($key, $value) = each %msgName2Type)
{
    print "$key=>$value\n";
}
while(($key, $value) = each %msgsDef)
{
    print "$key\n";
    foreach(@$value)
    {
        ($a, $b, $c, $d) = @$_;
        print "\t$a\t$b\t$c\t$d\n";
    }
    
}
