from scapy.all import IP, ICMP, sr1
import time

def tracert(dest_ip, max_ttl, packet_size, timeout, source_ip=None):
    print(f"Tracing route to {dest_ip} with max TTL {max_ttl}, packet size {packet_size} bytes, timeout {timeout}s")
    
    for ttl in range(1, max_ttl + 1):
        packet = IP(dst=dest_ip, ttl=ttl)
        
        if source_ip:
            packet.src = source_ip
        
        # Add ICMP layer and set the packet size
        icmp_request = ICMP() / ("X" * (packet_size - 28))  # Subtract header size (20 IP + 8 ICMP)
        
        start_time = time.time()
        
        # Send the packet and wait for a reply
        reply = sr1(packet/icmp_request, timeout=timeout, verbose=0)
        rtt = (time.time() - start_time) * 1000  # RTT in ms

        if reply is None:
            print(f"{ttl}\t*\tRequest timed out.")
        elif reply.type == 11:  # TTL expired
            print(f"{ttl}\t{reply.src}\t{round(rtt, 2)} ms (TTL expired)")
        elif reply.type == 0:  # Echo reply (destination reached)
            print(f"{ttl}\t{reply.src}\t{round(rtt, 2)} ms (Destination reached)")
            break
        else:
            print(f"{ttl}\t{reply.src}\t{round(rtt, 2)} ms (Unknown response)")
            
if __name__ == "__main__":
    # Take inputs from user
    dest_ip = input("Enter the destination IP/Domain: ")
    max_ttl = int(input("Enter the maximum TTL (e.g., 30): "))
    packet_size = int(input("Enter the packet size in bytes (default 60): "))
    timeout = float(input("Enter the timeout in seconds (e.g., 1.0): "))
    source_ip = input("Enter the source IP (optional, press Enter to skip): ") or None
    
    # Call the function
    tracert(dest_ip, max_ttl, packet_size, timeout, source_ip)
