from scapy.all import IP, ICMP, Raw, sr1
import socket
import time

def ping(destination, count=4, ttl=64, packet_size=64, timeout=1):
    try:
        # Validate destination IP
        destination_ip = socket.gethostbyname(destination)
    except socket.gaierror:
        print(f"Invalid IP address or hostname: {destination}")
        return
    print(f"Pinging {destination_ip} with {packet_size} bytes of data:")
    
    # Initialize variables for statistics
    sent_packets = 0
    received_packets = 0
    rtt_list = []
    
    for i in range(count):
        packet = IP(dst=destination_ip, ttl=ttl)/ICMP()/Raw(load='X'*packet_size)
        try:
            # Send the packet and measure the time
            start_time = time.time()
            reply = sr1(packet, verbose=0, timeout=timeout)
            end_time = time.time()
            
            sent_packets += 1
            if reply:
                received_packets += 1
                rtt = (end_time - start_time) * 1000  # in milliseconds
                rtt_list.append(rtt)
                print(f"Reply from {destination_ip}: bytes={packet_size} ttl={reply.ttl} time={rtt:.2f}ms")
            else:
                print(f"Request timed out.")
            
        except Exception as e:
            print(f"An error occurred: {e}")
        
        time.sleep(1)  # Sleep for 1 second between pings
    
    # Calculate packet loss
    packet_loss = ((sent_packets - received_packets) / sent_packets) * 100
    
    # Calculate RTT statistics
    if rtt_list:
        min_rtt = min(rtt_list)
        max_rtt = max(rtt_list)
        avg_rtt = sum(rtt_list) / len(rtt_list)
    else:
        min_rtt = max_rtt = avg_rtt = 0

    # Display statistics
    print(f"\nPing statistics for {destination_ip}:")
    print(f"    Packets: Sent = {sent_packets}, Received = {received_packets}, Lost = {sent_packets - received_packets} ({packet_loss:.2f}% loss),")
    print()
    print(f"Approximate round trip times in milli-seconds:")
    print(f"    Minimum = {min_rtt:.2f}ms, Maximum = {max_rtt:.2f}ms, Average = {avg_rtt:.2f}ms")




#-------------------------------------------------------Main----------------------------------------------------------------#
print("Enter the destination IP address or hostname: ")
destination = input()

try:
    print("Enter number of ping requests (default: 4): ")
    count = int(input() or 4)
    if count < 1:
        raise ValueError("Count must be a positive integer.")
except ValueError as e:
    print(f"Invalid count value: {e}")
    exit(1)

try:
    print("Enter TTL (default: 64): ")
    ttl = int(input() or 64)
    if ttl < 1:
        raise ValueError("TTL must be a positive integer.")
except ValueError as e:
    print(f"Invalid TTL value: {e}")
    exit(1)

try:
    print("Enter the packet size (default: 64): ")
    packet_size = int(input() or 64)
    if packet_size < 1:
        raise ValueError("Packet size must be a positive integer.")
except ValueError as e:
    print(f"Invalid packet size: {e}")
    exit(1)

try:
    print("Enter timeout in seconds (default: 1): ")
    timeout = int(input() or 1)
    if timeout < 1:
        raise ValueError("Timeout must be a positive integer.")
except ValueError as e:
    print(f"Invalid timeout value: {e}")
    exit(1)

ping(destination, count, ttl, packet_size, timeout)
print()