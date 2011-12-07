#include "PcapFileDumper.h"
#include "ConfigCenter.h"

using namespace Net::Pcap;
using namespace Config;

//-----------------------------------------------------------------------------

FileDumper::FileDumper()
	: processorM(1)
	, pcapHandlerM(NULL)
	, pcapDumperM(NULL)
{
	processorM.start();
}

//-----------------------------------------------------------------------------

FileDumper::~FileDumper()
{
	fini();
	processorM.waitStop();
}

//-----------------------------------------------------------------------------

void FileDumper::init(pcap_t* thePcapHandler)
{
	processorM.process(0, &FileDumper::_init, this, thePcapHandler);
}

//-----------------------------------------------------------------------------

void FileDumper::_init(pcap_t* thePcapHandler)
{
	pcapHandlerM = thePcapHandler;
	if (pcapHandlerM)
	{
		std::string filename = getFileName();
		if (pcapDumperM)
		{
			pcap_dump_close(pcapDumperM);
			pcapDumperM = NULL;
		}
		pcapDumperM = pcap_dump_open(pcapHandlerM, filename.c_str());
	}
}

//-----------------------------------------------------------------------------

void FileDumper::fini()
{
	processorM.process(0, &FileDumper::_fini, this);
}

//-----------------------------------------------------------------------------

void FileDumper::_fini()
{
	if (pcapDumperM)
	{
		pcap_dump_close(pcapDumperM);
		pcapDumperM = NULL;
	}
}

//-----------------------------------------------------------------------------

void FileDumper::reset(pcap_t* thePcapHandler)
{
	init(thePcapHandler);
}

//-----------------------------------------------------------------------------

void FileDumper::asynHandlePackage(
				boost::shared_ptr<struct pcap_pkthdr> theHeader, 
				boost::shared_array<u_char> thePacket)
{
	processorM.process(0, &FileDumper::handlePackage, this, theHeader, thePacket);
}

//-----------------------------------------------------------------------------

void FileDumper::handlePackage(
				boost::shared_ptr<struct pcap_pkthdr> theHeader, 
				boost::shared_array<u_char> thePacket)
{
	if (pcapDumperM)
	{
		pcap_dump((u_char*)pcapDumperM, theHeader.get(), thePacket.get());
	}
	else
	{
		LOG_WARN("drop a package");
	}
}

//-----------------------------------------------------------------------------

std::string FileDumper::getFileName()
{
	std::string filePre = ConfigCenter::instance()->get("pcap.file.pre", "./Test");
	std::string fileFormat = filePre + "%Y%m%d_%H%M%S.pcap";
	struct tm tm;
	char filename[256] = {0};
	time_t t;
	time(&t);
	if(t != ((time_t)-1))
	{
		tm = *localtime(&t);
		strftime(filename, sizeof(filename), fileFormat.c_str(), &tm);
	}	
	return std::string(filename);

}

//-----------------------------------------------------------------------------

