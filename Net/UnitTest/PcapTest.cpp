#include <stdio.h>
#include <pcap.h>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
     pcap_t *handle;        /* Session handle */
     char dev[] = "em1";        /* Device to sniff on */
     char errbuf[PCAP_ERRBUF_SIZE]; /* Error string */
     struct bpf_program fp;     /* The compiled filter expression */
     char filter_exp[] = "port 22"; /* The filter expression */
     bpf_u_int32 mask;      /* The netmask of our sniffing device */
     bpf_u_int32 net;       /* The IP of our sniffing device */
    struct pcap_pkthdr header;  /* The header that pcap gives us */
    const u_char *packet;       /* The actual packet */

     if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
         fprintf(stderr, "Can't get netmask for device %s\n", dev);
         net = 0;
         mask = 0;
     }
     handle = pcap_open_live(dev, BUFSIZ, 0, 1000, errbuf);
     if (handle == NULL) {
         fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
         return(2);
     }
     if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
         fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
         return(2);
     }
     if (pcap_setfilter(handle, &fp) == -1) {
         fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
         return(2);
     }
    /* Grab a packet */
    packet = pcap_next(handle, &header);
    /* Print its length */
    printf("Jacked a packet with length of [%d]\n", header.len);
    
    pcap_dumper_t* myfile = pcap_dump_open(handle, "out.pcap");
    pcap_dump((u_char*)myfile, &header, packet);
    pcap_dump_close(myfile);
    
    /* And close the session */
    pcap_close(handle);
    return(0);
}
