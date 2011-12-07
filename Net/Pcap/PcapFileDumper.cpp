#include "PcapFileDumper.h"

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
}

//-----------------------------------------------------------------------------

void FileDumper::init(pcap_t* thePcapHandler)
{
	pcapHandlerM(thePcapHandler);
	if (pcapHandlerM)
	{
		std::string filename = getFileName();
		if (pcapDumperM)
		{
			pcap_dump_close(myfile);
			pcapDumperM = NULL;
		}
		pcapDumperM = pcap_dump_open(pcapHandlerM, filename.c_str());
	}
}

//-----------------------------------------------------------------------------

void FileDumper::fini()
{
	if (pcapDumperM)
	{
		pcap_dump_close(myfile);
		pcapDumperM = NULL;
	}

}

//-----------------------------------------------------------------------------

void FileDumper::reset(pcap_t* thePcapHandler)
{
	init(thePcapHandler);
}

//-----------------------------------------------------------------------------

void FileDumper::asynHandlePackage(const struct pcap_pkthdr *theHeader, const u_char *thePacket)
{

}

//-----------------------------------------------------------------------------

void FileDumper::handlePackage(const struct pcap_pkthdr *theHeader, const u_char *thePacket)
{

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

