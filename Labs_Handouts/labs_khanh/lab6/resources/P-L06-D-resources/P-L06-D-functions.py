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
# print_flow_summary_header()
# 
# Prints the first lines of output as required by the marking script
##########
def print_flow_summary_header():
    print '\n--------------- PER-FLOW INFO -----------------\n'

##########
# print_flow_summary_footer()
# 
# Prints the last line of output as required by the marking script
##########
def print_flow_summary_footer():
    print'-----------------------------------------------'

##########
# print_flow_summary_single(u_int isrc_ip, u_short src_port, u_int idst_ip, u_short dst_port,
#                           char *protocol, struct timeval *start_time, u_int flow_packets, u_int flow_bytes) 
# 
# isrc_ip      - 32-bit IP address
# src_port     - Source port number
# idst_ip      - 32-bit IP address
# dst_port     - Destination port number
# protocol     - String containing protocol (should be 'tcp' or 'udp')
# start_time   - Timestamp of first packet in the nominated flow
# flow_packets - Integer count of total packets in this flow
# flow_bytes   - Integer sum of all packet sizes in this flow
# 
# Print the lines of data as required by the marking script for ALL packets in ONE flow from the PCAP file
# The function will ensure formatting and convert IP addresses to strings for display
##########
def print_flow_summary_single(src_ip, src_port, dst_ip, dst_port, protocol, \
			      start_time, flow_packets, flow_bytes):
  
    print 'Flow: %s:%s-%s:%s' % (src_ip, src_port, dst_ip, dst_port)
    print 'Protocol: %s' % (protocol)
    print 'Start time: %s' % (datetime.datetime.utcfromtimestamp(start_time)) 
    print 'Total packets: %u\nTotal bytes: %u\n' % (flow_packets, flow_bytes)
    
##########
# run_example
#
# Main function of sample program to demonstrate use of provided functions
#
# 1) Parse command line parameter. Fail if PCAP filename not provided
# 2) Call print_flow_summary_header() to display initial output
# 3) Generate fake data for flow 1 to print as an example
# 4) Call print_flow_summary_single() to display fake data. You will need to this once for each flow in the PCAP file
# 5) Generate more fake data for flow 2 to print as an example
# 6) Call print_flow_summary_single() to display flow 2 fake data
# 7) Call print_flow_summary_footer() to display final output
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

    print_flow_summary_header()

    start_time = time.time()
    src_ip = '1.2.3.4'
    src_port = '99'
    dst_ip = '5.6.7.8'
    dst_port = '199'
    protocol = 'tcp'
    
    print_flow_summary_single(src_ip, src_port, dst_ip, dst_port, protocol, start_time, 100, 150000)
      
    start_time = time.time()
    src_ip = '10.0.0.1'
    src_port = '123'
    dst_ip = '10.1.1.200'
    dst_port = '456'
    protocol = 'udp'
    
    print_flow_summary_single(src_ip, src_port, dst_ip, dst_port, protocol, start_time, 8, 12000)  
  
    print_flow_summary_footer()
    
    fp_dmp_file.close()

if __name__ == '__main__':
    run_example()
