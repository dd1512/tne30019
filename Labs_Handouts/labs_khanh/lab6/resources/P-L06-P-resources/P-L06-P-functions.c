/********************************************************************************
 * Sample C program with printing functionaility for P-Lab-06-PCAP-Programming-P
 * 
 * You should compile this program with:
 * 
 *    gcc -l pcap -o P-L06-P-functions P-L06-P-functions.c
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
void print_packet_info(struct timeval *ts, u_int isrc_ip,
        u_short src_port, u_int idst_ip, u_short dst_port,
        char *protocol, u_int pkt_len, u_char ip_ttl);
void print_summary(u_int total_pkts, u_int total_bytes,u_int avg_pkt);

/**********
 * Sample program to demonstrate use of provided functions
 * 
 * 1) Parse command line parameter. Fail if PCAP filename not provided
 * 2) Generate fake data to print as an example
 * 3) Call print_packet_info() to display fake data. You will need to do this once for each packet in the PCAP file
 * 4) Call print_summary() to display a fake summary. You will need to do this with your accumulated data
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

    gettimeofday(&ts, NULL);
    srcip = inet_addr("1.2.3.4");
    srcport = 99;
    dstip = inet_addr("5.6.7.8");
    dstport = 199;
    strcpy(proto, "tcp");
    
    print_packet_info(&ts, srcip, srcport, dstip, dstport, proto, 1000, 64);
    
    print_summary(100, 1500, 1500 / 100);
    
    return 0;
}

/**********
 * iptostr(char *strop, uint32_t addr)
 * 
 * Take the binary IP address stored in addr, convert it to a string and store in strip
 * Function does no error checking, it is callers responsibility to ensure that strip
 * is pre-allocated and contains enough space to store an IP address as string representation
 * 16 bytes should be enough (4 octets at three characters, 3 periods, and one string terminator
**********/
void iptostr(char *strip, uint32_t addr){
    sprintf(strip, "%u.%u.%u.%u", addr & 0xff,  addr >> 8 & 0xff,
        addr >> 16 & 0xff,  addr >> 24 & 0xff);
}

/**********
 * print_packet_info(struct timeval *ts, u_int isrc_ip, u_short src_port, u_int idst_ip, u_short dst_port,
 *                   char *protocol, u_int pkt_len, u_char ip_ttl) 
 * 
 * ts       - Packet timestamp
 * isrc_ip  - 32-bit binary IP address
 * src_port - Source port number
 * idst_ip  - 32-bit binary IP address
 * dst_port - Destination port number
 * protocol - String containing protocol (should be 'tcp' or 'udp')
 * pkt_len  - Packet length as integer
 * ip_ttl   - IP Packet TTL as byte value
 * 
 * Print the line of data as required by the marking script for ONE packet in the PCAP file
 * The function will ensure formatting and convert IP addresses to strings for display
**********/
void print_packet_info(struct timeval *ts, u_int isrc_ip,
        u_short src_port, u_int idst_ip, u_short dst_port,
        char *protocol, u_int pkt_len, u_char ip_ttl) {
            
    char src_ip[16], dst_ip[16], ip_proto[4], ftime[100];
    struct tm *tm_time;

    tm_time = gmtime (&ts->tv_sec);
    strftime(ftime, sizeof(ftime), "%F %H:%M:%S", tm_time);

    iptostr(src_ip, isrc_ip);
    iptostr(dst_ip, idst_ip);

    printf("[%s.%06u] - ", ftime, ts->tv_usec);
    printf("%s:%u -> %s:%u (%s, len=%u, ttl=%u)\n",
     src_ip, src_port,  dst_ip, dst_port, protocol, pkt_len, ip_ttl);
}

/**********
 * print_summary(u_int total_pkts, u_int total_bytes,u_int avg_pkt)
 * 
 * total_pkts - Integer count of total packets
 * total_bytes - Integer count of sum of packet sizes
 * avg_pkt     - Integer representation of average packet size
 * 
 * Print the three parameters in the format as required by the marking script
**********/
void print_summary(u_int total_pkts, u_int total_bytes,u_int avg_pkt) {
    printf("\n------------------- SUMMARY -------------------\n\n");
    printf("Total packets: %u\n", total_pkts);
    printf("Total bytes (bytes): %u\n", total_bytes);
    printf("Average packet size (bytes): %u\n\n", avg_pkt);
    printf("-----------------------------------------------\n");
}
