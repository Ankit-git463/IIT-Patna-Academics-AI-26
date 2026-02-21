import socket
import time
from scapy.all import IP, ICMP, sr1

# Function to perform the Scapy-based traceroute
def tracert(destination, max_ttl=30, num_pings=3, packet_size=64, timeout_value=2, delay_between_pings=1):
    try:
        # Resolve the destination IP
        destination_ip = socket.gethostbyname(destination)
        print(f"Traceroute to {destination} ({destination_ip}), {max_ttl} hops max.\n")
    except socket.error as e:
        print(f"Invalid destination IP: {e}")
        return

    try:
        for ttl in range(1, max_ttl + 1):
            successful_pings = 0  # Track successful replies for packet loss calculation
            total_rtt = 0  # Sum RTTs for averaging

            print(f"Hop {ttl}:")
            
            for ping in range(num_pings):
                # Build the packet with specified TTL
                pkt = IP(dst=destination_ip, ttl=ttl) / ICMP() / (b' ' * (packet_size - 28))  # ICMP header is 28 bytes
                
                # Start time to calculate RTT
                start_time = time.time()
                reply = sr1(pkt, timeout=timeout_value, verbose=0)
                rtt = (time.time() - start_time) * 1000  # RTT in ms

                if reply:
                    hop_ip = reply.src
                    successful_pings += 1
                    total_rtt += rtt
                    print(f"  Ping {ping+1}: {hop_ip}, RTT: {rtt:.2f} ms")
                else:
                    print(f"  Ping {ping+1}: Request timed out")

                # Delay between pings
                time.sleep(delay_between_pings)

            # Packet loss percentage
            packet_loss_percentage = ((num_pings - successful_pings) / num_pings) * 100

            # Display summary for the hop
            if successful_pings > 0:
                avg_rtt = total_rtt / successful_pings
                print(f"  Summary: {successful_pings}/{num_pings} pings received, Packet loss: {packet_loss_percentage:.2f}%, Avg RTT: {avg_rtt:.2f} ms\n")
            else:
                print(f"  Summary: 0/{num_pings} pings received, Packet loss: 100.00%\n")

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