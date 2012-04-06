#!/usr/bin/perl

my $supervisor_dir = "/etc/ProcessorSupervisor/";

while (1)
{
    while (<glob "$supervisor_dir/*.command">)
    {
	my $file = $_;
        next if ($file eq ".");
        next if ($file eq "..");
        next unless ($file =~ /.*\.command/);

        checkProcessor($file);
    }
    sleep(1);
}

sub checkProcessor
{
    my $file = shift;
    return 0 unless $file;

    my $proc_name = $file;
    $proc_name =~ s/\.command//;

    (my $checkCmd, my $startCmd) = parseCommand($file);

    `$checkCmd`;
    my $result = $?;
    if ($result != 0)
    {
        `$startCmd `;
        print "$proc_name is down, restart it with cmd: $startCmd\n";
    }
}

sub parseCommand
{
    my $file = shift;
    return unless $file;

    my $checkCmd;
    my $startCmd;

    open FILE_HANDLE, " < $file" or return;
    while(<FILE_HANDLE>)
    {
        my $line = $_;
        chomp $line;

        if ($line =~ /^check:(.*)$/)
        {
            $checkCmd = $1;
            next;
        }
        if ($line =~ /^start:(.*)$/)
        {
            $startCmd = $1;
            next;
        }
    }
    close FILE_HANDLE;

    #print "[file:$file][check:$checkCmd][startCmd:$startCmd]\n";
    return ($checkCmd, $startCmd);
}
