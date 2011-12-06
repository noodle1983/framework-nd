#include "PcapSniffer.h"

int main()
{
    Net::Pcap::Sniffer sniffer;
    sniffer.start();

    sleep(3);

    sniffer.stop();
    return 0;
}

