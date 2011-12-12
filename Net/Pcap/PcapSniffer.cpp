#include "PcapSniffer.h"
#include "Log.h"
#include "ConfigCenter.h"

#include <boost/bind.hpp>

using namespace Net::Pcap;
using namespace Config;

//-----------------------------------------------------------------------------

Sniffer::Sniffer()
	: pcapHandlerM(NULL)
    , isInPromiscuousModeM(0)
    , isToStopM(false)
{
}

//-----------------------------------------------------------------------------

Sniffer::~Sniffer()
{
}

//-----------------------------------------------------------------------------

void Sniffer::start()
{
    char errbuf[PCAP_ERRBUF_SIZE]; /* Error string */
    struct bpf_program fp;     /* The compiled filter expression */
    bpf_u_int32 mask;      /* The netmask of our sniffing device */
    bpf_u_int32 net;       /* The IP of our sniffing device */
    
    //std::string devNameM;               /* Device to sniff on */
    //std::string filterExpM; /* The filter expression */
    devNameM = ConfigCenter::instance()->get("pcap.dev", "eth0");
    filterExpM = ConfigCenter::instance()->get("pcap.filter", "tcp port 22");
    isInPromiscuousModeM = ConfigCenter::instance()->get("pcap.promiscuous", 0);

    if (pcap_lookupnet(devNameM.c_str(), &net, &mask, errbuf) == -1) 
    {
        LOG_ERROR("Can't get netmask for device " << devNameM << ":" << errbuf);
        net = 0;
        mask = 0;
    }

    pcapHandlerM = pcap_open_live(devNameM.c_str(), BUFSIZ, isInPromiscuousModeM, 1000, errbuf);
    if (pcapHandlerM == NULL) 
    {
        LOG_FATAL("Couldn't open device " << devNameM << ":" << errbuf);
        exit(-1);
    }

    if (pcap_compile(pcapHandlerM, &fp, filterExpM.c_str(), 0, net) == -1) 
    {
        LOG_FATAL("Couldn't parse filter '" << filterExpM << "' :" << pcap_geterr(pcapHandlerM));
        exit(-1);
    }
    if (pcap_setfilter(pcapHandlerM, &fp) == -1) 
    {
        LOG_FATAL("Couldn't install filter '" << filterExpM << "':" << pcap_geterr(pcapHandlerM));
        exit(-1);
    }

    isToStopM = false;
	fileDumperM.init(pcapHandlerM);
    threadsM.create_thread(boost::bind(&Sniffer::sniffLoop, this));
}

//-----------------------------------------------------------------------------

void Sniffer::stop()
{
    if (pcapHandlerM)
    {
        isToStopM = true;
        pcap_breakloop(pcapHandlerM);
        threadsM.interrupt_all();
        threadsM.join_all();
        pcapHandlerM = NULL;
    }
}

//-----------------------------------------------------------------------------

void Sniffer::sniffLoop()
{
    while(!isToStopM)
    {
        pcap_loop(pcapHandlerM, 1024, handlePackage, (u_char*)this);
    }

}

//-----------------------------------------------------------------------------

void Sniffer::handlePackage(u_char *args, const struct pcap_pkthdr *theHeader, const u_char *thePackage)
{
    Sniffer* sniffer = (Sniffer*)args;   

	struct pcap_pkthdr* headerCopy = new pcap_pkthdr;
	*headerCopy = *theHeader;
	boost::shared_ptr<struct pcap_pkthdr> newHeader(headerCopy);

	u_char *pkgCopy = new u_char[theHeader->caplen];
	memcpy(pkgCopy, thePackage, theHeader->caplen);
	boost::shared_array<u_char> newPackage(pkgCopy);
	
	sniffer->fileDumperM.asynHandlePackage(newHeader, newPackage);
}
