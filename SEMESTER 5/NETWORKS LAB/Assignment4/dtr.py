import socket
import time
from scapy.all import IP, ICMP, sr1

# Function to perform the Scapy-based traceroute
def tracert(destination, max_ttl=30, num_pings=3, packet_size=64, timeout_value=2, delay_between_pings=1):
    try:
        # Resolve the destination IP
        destination_ip = socket.gethostbyname(destination)
        print(f"Traceroute to {destination} ({destination_ip}), {max_ttl} hops max.")
    except socket.error as e:
        print(f"Invalid destination IP: {e}")
        return

    try:
        for ttl in range(1, max_ttl + 1):
            for ping in range(num_pings):
                # Build the packet with specified TTL
                pkt = IP(dst=destination_ip, ttl=ttl) / ICMP() / (b' ' * (packet_size - 28))  # ICMP header is 28 bytes
                
                # Start time to calculate RTT
                start_time = time.time()
                reply = sr1(pkt, timeout=timeout_value, verbose=0)
                rtt = (time.time() - start_time) * 1000  # RTT in ms

                if reply:
                    hop_ip = reply.src
                    print(f"Hop {ttl}, Ping {ping+1}: {hop_ip}, RTT: {rtt:.2f} ms")
                else:
                    print(f"Hop {ttl}, Ping {ping+1}: Request timed out")

                # Delay between pings
                time.sleep(delay_between_pings)

            print()  # Newline for better readability between TTLs

            # Exit the loop if the destination is reached
            if reply and reply.src == destination_ip:
                print(f"Destination {destination_ip} reached in {ttl} hops.")
                break
        else:
            print(f"Max TTL reached without reaching the destination.")

    except Exception as e:
        print(f"An error occurred: {e}")

# User input for parameters
destination = input("Enter the destination (IP or hostname): ")
max_ttl = int(input("Enter the max TTL (default 30): ") or 30)
num_pings = int(input("Enter the number of pings per hop (default 3): ") or 3)
packet_size = int(input("Enter the packet size in bytes (default 64): ") or 64)
timeout_value = int(input("Enter the timeout per ping in seconds (default 2): ") or 2)
delay_between_pings = float(input("Enter the delay between pings in seconds (default 1): ") or 1)

# Run the traceroute
tracert(destination, max_ttl=max_ttl, num_pings=num_pings, packet_size=packet_size, timeout_value=timeout_value, delay_between_pings=delay_between_pings)