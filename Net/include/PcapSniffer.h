#ifndef SNIFFER_H
#define SNIFFER_H

#include <string>
#include <boost/thread.hpp>
#include <pcap.h>

namespace Net
{
namespace Pcap
{
    class Sniffer
    {
    public:
        Sniffer();
        ~Sniffer();

		void start();
		void stop();

		//thread function
		void sniffLoop();

		static void handlePackage(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

    private:
        pcap_t *pcapHandlerM;        /* Session handle */
        std::string devNameM;               /* Device to sniff on */
        std::string filterExpM; /* The filter expression */
        boost::thread_group threadsM;
		
		bool isToStop; 
    };
}
}
#endif /* SNIFFER_H */

