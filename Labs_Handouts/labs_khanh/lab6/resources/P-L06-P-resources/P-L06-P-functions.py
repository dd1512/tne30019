#!/usr/local/bin/python
################################################################################
# Sample Python program with printing functionaility for P-Lab-06-PCAP-Programming-P
################################################################################

##########
# You will probably want all these libraries for your actual program
##########
import datetime
import time
import sys
import dpkt
import socket

##########
# print_packet_info(ts, src_ip, src_port, dst_ip, dst_port, protocol, pkt_len, ttl)
# 
# ts       - Packet timestamp
# isrc_ip  - 32-bit IP address
# src_port - Source port number
# idst_ip  - 32-bit IP address
# dst_port - Destination port number
# protocol - String containing protocol (should be 'tcp' or 'udp')
# pkt_len  - Packet length as integer
# ip_ttl   - IP Packet TTL as byte value
# 
# Print the line of data as required by the marking script for ONE packet in the PCAP file
# The function will ensure formatting and convert IP addresses to strings for display
##########
def print_packet_info(ts, src_ip, src_port, dst_ip, dst_port, protocol, pkt_len, ttl):
    
    # get UTC timestamp, source/destination IP/port
    utc_timestamp = str(datetime.datetime.utcfromtimestamp(ts))
    
    # create log line
    log_line = '[%s] - %s:%s -> %s:%s (%s, len=%d, ttl=%d)' % \
	  (utc_timestamp, src_ip, src_port, dst_ip, dst_port, protocol, pkt_len, ttl)
	
    print log_line

##########
# print_summary(total_packet_count, total_bytes_count, average_pkt_size)
#
# total_pkts - Integer count of total packets
# total_bytes - Integer count of sum of packet sizes
# avg_pkt     - Integer representation of average packet size
# 
# Print the three parameters in the format as required by the marking script
##########
def print_summary(total_packet_count, total_bytes_count, average_pkt_size):

    print '\n------------------- SUMMARY -------------------\n'
    print 'Total packets: %i' % (total_packet_count)
    print 'Total bytes (bytes): %i' % (total_bytes_count)
    print 'Average packet size (bytes): %i' % (average_pkt_size)
    print '\n-----------------------------------------------'

##########
# run_example
#
# Main function of sample program to demonstrate use of provided functions
#
# 1) Parse command line parameter. Fail if PCAP filename not provided and file cannot be opened
# 2) Generate fake data to print as an example
# 3) Call print_packet_info() to display fake data. You will need to do this once for each packet in the PCAP file
# 4) Call print_summary() to display a fake summary. You will need to do this with your accumulated data
##########
def run_example():
  
    global total_packet_count, total_bytes_count
    
    try:
	sys.argv[1]
	dmp_file = sys.argv[1]
	fp_dmp_file = open(dmp_file)
    except Exception as e:
	print 'Error: please supply pcap filename!\n'
	return

    ts = time.time()
    src_ip = '1.2.3.4'
    src_port = 99
    dst_ip = '5.6.7.8'
    dst_port = 199
    protocol = 'tcp'
    
    print_packet_info(ts, src_ip, src_port, dst_ip, dst_port, protocol, 1000, 64)
    
    print_summary(100, 1500, 1500 / 100)
    
    fp_dmp_file.close()
    
if __name__ == '__main__':
    run_example()
