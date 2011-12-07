#ifndef PCAPFILEDUMPER_H
#define PCAPFILEDUMPER_H

#include "BoostProcessor.h"
#include <pcap.h>

namespace Net
{
namespace Pcap
{
    class FileDumper
    {
    public:
        FileDumper();
        ~FileDumper();
		
		void init(pcap_t* thePcapHandler);
		void fini();
		void reset(pcap_t* thePcapHandler);

        void asynHandlePackage(const struct pcap_pkthdr *theHeader, const u_char *thePacket);
        void handlePackage(const struct pcap_pkthdr *theHeader, const u_char *thePacket);

	private:
		std::string getFileName();

    private:    
        Processor::BoostProcessor processorM;
        pcap_t* pcapHandlerM;
        pcap_dumper_t* pcapDumperM;
    };
}
}

#endif /* PCAPFILEDUMPER_H */

