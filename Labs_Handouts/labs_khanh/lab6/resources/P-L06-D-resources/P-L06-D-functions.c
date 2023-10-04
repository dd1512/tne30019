/********************************************************************************
 * Sample C program with printing functionaility for P-Lab-06-PCAP-Programming-D
 * 
 * You should compile this program with:
 * 
 *    gcc -l pcap -o P-L06-D-functions P-L06-D-functions.c
 *
 *    -l pcap              = Link the PCAP library during compile
 *    -o P-L06-P-functions = Output executable name
********************************************************************************/

/**********
 * You will probably want all these headers for your actual program
**********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pcap.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

/**********
 * Pre-declare provided display functions
**********/
void print_flow_summary_header();
void print_flow_summary_footer();
void print_flow_summary_single(u_int isrc_ip, u_short src_port, u_int idst_ip, u_short dst_port,
                               char *protocol, struct timeval *start_time, u_int flow_packets, u_int flow_bytes);

/**********
 * Sample program to demonstrate use of provided functions
 * 
 * 1) Parse command line parameter. Fail if PCAP filename not provided
 * 2) Call print_flow_summary_header() to display initial output
 * 3) Generate fake data for flow 1 to print as an example
 * 4) Call print_flow_summary_single() to display fake data. You will need to this once for each flow in the PCAP file
 * 5) Generate more fake data for flow 2 to print as an example
 * 6) Call print_flow_summary_single() to display flow 2 fake data
 * 7) Call print_flow_summary_footer() to display final output
**********/
int main(int argc, char *argv[]) {
    char filename[100];
    struct timeval ts;
    
    u_int srcip, dstip;
    u_short srcport, dstport;
    char proto[10];

    if (argc > 1)
        strncpy(filename, argv[1], sizeof(filename));
    else {
        printf("Error: please supply pcap filename!\n");
        return 2;
    }

    print_flow_summary_header();

    gettimeofday(&ts, NULL);
    srcip = inet_addr("1.2.3.4");
    srcport = 99;
    dstip = inet_addr("5.6.7.8");
    dstport = 199;
    strcpy(proto, "tcp");
    
    print_flow_summary_single(srcip, srcport, dstip, dstport,
        proto, &ts, 100, 150000);

    gettimeofday(&ts, NULL);
    srcip = inet_addr("10.0.0.1");
    srcport = 123;
    dstip = inet_addr("10.1.1.200");
    dstport = 456;
    strcpy(proto, "udp");
    print_flow_summary_single(srcip, srcport, dstip, dstport,
        proto, &ts, 8, 12000);
        
    print_flow_summary_footer();
}

/**********
 * iptostr(char *strop, uint32_t addr)
 * 
 * Take the binary IP address stored in addr, convert it to a string and store in strip
 * Function does no error checking, it is callers responsibility to ensure that strip
 * is pre-allocated and contains enough space to store an IP address as string representation
 * 16 bytes should be enough (4 octets at three characters, 3 periods, and one string terminator
**********/
void iptostr(char *strip, uint32_t addr) {
    sprintf(strip, "%u.%u.%u.%u", addr & 0xff,  addr >> 8 & 0xff,
        addr >> 16 & 0xff,  addr >> 24 & 0xff);
}

/**********
 * print_flow_summary_header()
 * 
 * Prints the first lines of output as required by the marking script
**********/
void print_flow_summary_header() {
    printf("\n--------------- PER-FLOW INFO -----------------\n\n");
}

/**********
 * print_flow_summary_footer()
 * 
 * Prints the last line of output as required by the marking script
**********/
void print_flow_summary_footer() {
    printf("-----------------------------------------------\n");
}

/**********
 * print_flow_summary_single(u_int isrc_ip, u_short src_port, u_int idst_ip, u_short dst_port,
 *                           char *protocol, struct timeval *start_time, u_int flow_packets, u_int flow_bytes) 
 * 
 * isrc_ip      - 32-bit binary IP address
 * src_port     - Source port number
 * idst_ip      - 32-bit binary IP address
 * dst_port     - Destination port number
 * protocol     - String containing protocol (should be 'tcp' or 'udp')
 * start_time   - Timestamp of first packet in the nominated flow
 * flow_packets - Integer count of total packets in this flow
 * flow_bytes   - Integer sum of all packet sizes in this flow
 * 
 * Print the lines of data as required by the marking script for ALL packets in ONE flow from the PCAP file
 * The function will ensure formatting and convert IP addresses to strings for display
**********/
void print_flow_summary_single(u_int isrc_ip, u_short src_port, u_int idst_ip, u_short dst_port,
                               char *protocol, struct timeval *start_time, u_int flow_packets, u_int flow_bytes) {

    struct tm *tm_time;
    char ftime[100], src_ip[16], dst_ip[16];

    iptostr(src_ip, isrc_ip);
    iptostr(dst_ip, idst_ip);

    printf("Flow: %s:%u-%s:%u\n", src_ip, src_port, dst_ip, dst_port);
    printf("Protocol: %s\n", protocol);

    tm_time = gmtime (&start_time->tv_sec);
    strftime(ftime, sizeof(ftime), "%F %H:%M:%S", tm_time);

    printf("Start time: %s.%06u\n", ftime, start_time->tv_usec);
    printf("Total packets: %u\nTotal bytes: %u\n\n", flow_packets , flow_bytes);
}
