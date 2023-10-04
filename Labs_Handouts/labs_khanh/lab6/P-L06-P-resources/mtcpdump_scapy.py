#!/usr/local/bin/python3

from scapy.all import *
import time
import sys
import socket
import dpkt
import datetime


def print_packet_info(ts, src_ip, src_port, dst_ip, dst_port, protocol, pkt_len, ttl):
    """Print the line of data as required by the marking script

    The function will ensure formatting an convert IP addresses to strings for 
    display
    
    Keyword arguments:
    ts          - Packet timestamp
    src_ip      - 32-bit IP address
    src_port    - Source port number
    dst_ip      - 32-bit IP address
    dst_port    - Destination port number
    protocol    - String containing protocol (should be "tcp" or "udp")
    pkt_len     - Packet length as interger
    ip_ttl      - IP Packet TTL as byte value
    """

    # get UTC timestamp, source/destination IP/port
    utc_timestamp = str(datetime.datetime.utcfromtimestamp(ts))

    # create log line
    log_line = f"[{utc_timestamp}] - {src_ip}:{src_port} -> {dst_ip}:{dst_port} ({protocol}, len={pkt_len}, ttl={ttl})"

    print(log_line)


def print_summary(total_pkt_count, total_byte_count, average_pkt_size):
    """Print the three parameters in the format as required by the marking script

    Keyword arguments:
    total_pkt_count     - Integer count of total packets
    total_byte_count    - Integer count of sum of packet size
    average_pkt_size    - Integer representation of average packet size
    """

    print("\n------------------- SUMMARY -------------------\n")
    print(f"Total packets: {total_pkt_count}")
    print(f"Total bytes (bytes): {total_byte_count}")
    print(f"Average packet size (bytes): {average_pkt_size}")
    print("\n-----------------------------------------------")


def run_example():
    """Main function of sample program to demonstrate use of provided funtions

    1) Parse command line parameter. Fail if PCAP filename not provided and file cannot be opened
    2) Generate fake data to print as an example
    3) Call print_packet_info() to display fake data. You will need to do this once for each packet in the PCAP file
    4) Call print_summary() to display a fake summary. You will need to do this with your accumulated data
    """

    global total_packet_count, total_byte_count

    try:
        sys.argv[1]
        dmp_file = sys.argv[1]
        fp_dmp_file = open(dmp_file)
    except Exception as e:
        print("Error: please supply pcap filename!\n")
        return

    ts = time.time()
    src_ip = "1.2.3.4"
    src_port = 99
    dst_ip = "5.6.7.8"
    dst_port = 199
    protocol = "tcp"

    print_packet_info(ts, src_ip, src_port, dst_ip, dst_port, protocol, 1000, 64)

    print_summary(100, 1500, 1500 / 100)

    fp_dmp_file.close()


def inet_to_str(inet):
    try:
        return socket.inet_ntop(socket.AF_INET, inet)
    except ValueError:
        return socket.inet_ntop(socket.AF_INET6, inet)


def main():
    if not sys.argv[1]:
        print("Error: please supply pcap filename!\n")
        return

    filename = sys.argv[1]

    pkts = rdpcap(filename)

    pkt_count = 0
    byte_count = 0
    for pkt in pkts:
        ts = float(pkt.time)
        src_ip = pkt[IP].src
        dst_ip = pkt[IP].dst
        pkt_len = pkt[IP].len
        ttl = pkt[IP].ttl
        protocol = pkt[IP].payload.name
        src_port = pkt[protocol].sport
        dst_port = pkt[protocol].dport

        pkt_count += 1
        byte_count += pkt_len

        print(src_ip, dst_ip)
        print_packet_info(ts, src_ip, src_port, dst_ip, dst_port, protocol.lower(), pkt_len, ttl)

    print_summary(pkt_count, byte_count, byte_count // pkt_count)
            

if __name__ == "__main__":
    # run_example()
    main()
