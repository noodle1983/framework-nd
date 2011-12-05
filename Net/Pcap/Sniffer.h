#ifndef SNIFFER_H
#define SNIFFER_H

namespace Net
{
namespace Pcap
{
    class Sniffer
    {
    public:
        Sniffer();
        ~Sniffer();

        void setFielter();

    private:
        pcap_t *m_pcapHandler;        /* Session handle */
        std::string m_devName;               /* Device to sniff on */
        std::string m_filterExp; /* The filter expression */
    };
}
}
#endif /* SNIFFER_H */

