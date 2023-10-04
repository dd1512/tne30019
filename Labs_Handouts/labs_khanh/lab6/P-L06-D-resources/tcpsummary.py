#!/usr/local/bin/python3

import datetime
import time
import sys
import dpkt
import socket


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


def print_flow_summary_header():
    """Print the first line of output as required by the marking script"""
    print("\n--------------- PER-FLOW INFO -----------------\n")

def print_flow_summary_footer():
    """Print the last line of output as required by the marking script"""
    print("-----------------------------------------------")

def print_flow_summary_single(src_ip, src_port, dst_ip, dst_port, protocol, \
                                start_time, flow_packets, flow_bytes):
    """Print the lines of data as required by the marking script for ALL packets in ONE flow from the PCAP file

    The function will ensure formatting and convert IP addresses to strings for display

    Keyword arguments:
    src_ip          - 32-bit IP address
    src_port        - Source port number
    dst_ip          - 32-bit IP address
    dst_port        - Destination port number
    protocol        - String containing protocol (should be "tcp" or "udp")
    start_time      - Timestamp of first packet in the nominated flow
    flow_packets    - Integer count of total packets in this flow
    flow_bytes      - Integer sum of all packet sizes in this flow
    """
    
    print(f"Flow: {src_ip}:{src_port}-{dst_ip}:{dst_port}")
    print(f"Protocol: {protocol}")
    print(f"Start time: {datetime.datetime.utcfromtimestamp(start_time)}")
    print(f"Total packets: {flow_packets}\nTotal bytes: {flow_bytes}\n")

def run_example_P():
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

def run_example_D():
    """Main function of sample program to demonstrate use of provided functions

    1) Parse command line parameter. Fail if PCAP filename not provided
    2) Call print_flow_summary_header() to display initial output
    3) Generate fake data for flow 1 to print as an example
    4) Call print_flow_summary_single() to display fake data. You will need to call this once for each flow in the PCAP file
    5) Generate more fake data for flow 2 to print as an example
    6) Call print_flow_summary_single() to display flow 2 fake data
    7) Call print_flow_summary_footer() to display final output
    """

    global total_paket_count, total_bytes_count

    try:
        sys.argv[1]
        dmp_file = sys.argv[1]
        fp_dmp_file = open(dmp_file)
    except Exception as e:
        print("Error: please supply pcap filename!\n")
        return

    print_flow_summary_header()

    start_time=time.time()
    src_ip = "1.2.3.4"
    src_port = "99"
    dst_ip = "5.6.7.8"
    dst_port = "199"
    protocol = "tcp"

    print_flow_summary_single(src_ip, src_port, dst_ip, dst_port, protocol, start_time, 100, 150000)

    start_time = time.time()
    src_ip = "10.0.0.1"
    src_port = "123"
    dst_ip = "10.1.1.200"
    dst_port = "456"
    protocol = "udp"

    print_flow_summary_single(src_ip, src_port, dst_ip, dst_port, protocol, start_time, 8, 12000)

    print_flow_summary_footer()

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
    with open(filename, "rb") as pcap_file:
        pkts = dpkt.pcap.Reader(pcap_file)

        flows = {}
        pkt_count = 0
        byte_count = 0
        for ts, pkt in pkts:
            eth = dpkt.ethernet.Ethernet(pkt)

            if eth.type == dpkt.ethernet.ETH_TYPE_IP:
                ip = eth.data

                src_ip = inet_to_str(ip.src)
                dst_ip = inet_to_str(ip.dst)
                pkt_len = ip.len
                ttl = ip.ttl

                pkt_count += 1
                byte_count += pkt_len

                if ip.p == dpkt.ip.IP_PROTO_TCP:
                    protocol = "tcp"
                    tcp = ip.data
                    src_port = tcp.sport
                    dst_port = tcp.dport
                elif ip.p == dpkt.ip.IP_PROTO_UDP:
                    protocol = "udp"
                    udp = ip.data
                    src_port = udp.sport
                    dst_port = udp.dport
                else:
                    continue
            else:
                continue

            flow_id = (src_ip, src_port, dst_ip, dst_port, protocol)
            if flow_id not in flows:
                flows[flow_id] = {"start_time": ts,
                        "flow_packets": 1,
                        "flow_bytes": pkt_len}
            else:
                flows[flow_id]["flow_packets"] += 1
                flows[flow_id]["flow_bytes"] += pkt_len
                


        print_flow_summary_header()

        for flow_id, flow_stats in flows.items():
            print_flow_summary_single(*flow_id, 
                    flow_stats["start_time"], 
                    flow_stats["flow_packets"], 
                    flow_stats["flow_bytes"]) 

        print_flow_summary_footer()


if __name__ == "__main__":
    # run_example_P()
    # run_example_D()
    main()
