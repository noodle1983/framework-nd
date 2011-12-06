#ifndef PCAPFILEDUMPER_H
#define PCAPFILEDUMPER_H

#include "BoostProcessor.h"

namespace Net
{
namespace Pcap
{
    class FileDumper
    {
    public:
        FileDumper();
        ~FileDumper();

        asynHandlePackage(const struct pcap_pkthdr *theHeader, const u_char *thePacket);

    private:    
        Processor::BoostProcessor processorM;
        pcap_t* pcapHandlerM;
        pcap_dumper_t* pcapDumperM;
    };
}
}

#endif /* PCAPFILEDUMPER_H */

