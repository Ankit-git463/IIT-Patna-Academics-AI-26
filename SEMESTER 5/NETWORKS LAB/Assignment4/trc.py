import socket
from scapy.all import IP, ICMP, sr1
import time

def tracert(dest, max_ttl, packet_size, timeout, pings_per_hop, delay_between_pings, source_ip=None, output_file=None):
    results = []
    
    # Resolve the domain name to an IP address
    try:
        dest_ip = socket.gethostbyname(dest)
    except socket.gaierror:
        print(f"Error: Unable to resolve {dest}")
        return

    print(f"Tracing route to {dest} [{dest_ip}] with max TTL {max_ttl}, packet size {packet_size} bytes, timeout {timeout}s, {pings_per_hop} ping(s) per hop\n")
    
    # Header for output
    print(f"{'Hop':<5}{'Ping #':<8}{'IP Address':<20}{'RTT (ms)':<15}{'Packet Loss (%)':<15}")
    print("-" * 65)

    for ttl in range(1, max_ttl + 1):
        sent_packets = 0
        received_packets = 0
        rtt_list = []
        
        for ping in range(1, pings_per_hop + 1):
            sent_packets += 1
            packet = IP(dst=dest_ip, ttl=ttl)

            if source_ip:
                packet.src = source_ip

            icmp_request = ICMP() / ("X" * (packet_size - 28))  # Subtract header size (20 IP + 8 ICMP)

            start_time = time.time()

            try:
                reply = sr1(packet/icmp_request, timeout=timeout, verbose=0)
                rtt = (time.time() - start_time) * 1000  # RTT in ms
            except Exception as e:
                print(f"Error sending packet: {e}")
                if output_file:
                    results.append(f"{ttl}\t*\tError sending packet: {e}")
                continue

            if reply is None:
                rtt_list.append(None)  # No reply, so no RTT
                packet_loss = 100  # 100% packet loss for this specific ping
                print(f"{ttl:<5}{ping:<8}{'*':<20}{'Request timed out.':<15}{packet_loss:<15}")
            else:
                received_packets += 1
                rtt_list.append(round(rtt, 2))
                packet_loss = ((sent_packets - received_packets) / sent_packets) * 100
                ip_address = reply.src if reply else "*"
                print(f"{ttl:<5}{ping:<8}{ip_address:<20}{round(rtt, 2):<15}{packet_loss:<15.2f}")
                if reply.type == 0:  # Echo reply (destination reached)
                    print(f"Destination {reply.src} reached at hop {ttl}.")
                    results.append(f"{ttl}\t{ping}\t{ip_address}\t{round(rtt, 2)}\t{packet_loss}")
                    break
            
            results.append(f"{ttl}\t{ping}\t{ip_address}\t{round(rtt, 2) if rtt else '*'}\t{packet_loss}")
            time.sleep(delay_between_pings)
        
        # Break loop if destination is reached
        if reply and reply.type == 0:
            break
    
    # Save to file if specified
    if output_file:
        try:
            with open(output_file, "w") as f:
                f.write(f"{'Hop':<5}{'Ping #':<8}{'IP Address':<20}{'RTT (ms)':<15}{'Packet Loss (%)':<15}\n")
                f.write("-" * 65 + "\n")
                for line in results:
                    f.write(line + "\n")
            print(f"Results saved to {output_file}")
        except Exception as e:
            print(f"Error writing to file: {e}")

def validate_inputs(dest, max_ttl, packet_size, timeout, pings_per_hop, delay_between_pings, source_ip):
    try:
        # Resolve destination domain/IP
        socket.gethostbyname(dest)
    except socket.error:
        raise ValueError(f"Invalid destination IP or domain: {dest}")

    if not (1 <= max_ttl <= 255):
        raise ValueError(f"Invalid TTL value: {max_ttl}. It must be between 1 and 255.")
    
    if packet_size < 28:  # 28 is the minimum size considering IP and ICMP headers
        raise ValueError(f"Invalid packet size: {packet_size}. It must be at least 28 bytes.")
    
    if timeout <= 0:
        raise ValueError(f"Invalid timeout value: {timeout}. It must be greater than 0.")
    
    if pings_per_hop <= 0:
        raise ValueError(f"Invalid number of pings per hop: {pings_per_hop}. It must be a positive integer.")
    
    if delay_between_pings < 0:
        raise ValueError(f"Invalid delay between pings: {delay_between_pings}. It must be 0 or more.")
    
    if source_ip:
        try:
            socket.inet_aton(source_ip)
        except socket.error:
            raise ValueError(f"Invalid source IP: {source_ip}. Make sure it's correctly configured.")


if __name__ == "__main__":
    try:
        # Take inputs from user
        dest = input("Enter the destination IP/Domain: ")
        max_ttl = int(input("Enter the maximum TTL (e.g., 30): "))
        packet_size = int(input("Enter the packet size in bytes (default 60): "))
        timeout = float(input("Enter the timeout in seconds (e.g., 1.0): "))
        pings_per_hop = int(input("Enter the number of pings per hop (default 1): "))
        delay_between_pings = float(input("Enter the delay between pings in seconds (default 0.5): "))
        source_ip = input("Enter the source IP (optional, press Enter to skip): ") or None
        output_file = input("Enter the output file path (optional, press Enter to skip): ") or None

        # Validate inputs
        validate_inputs(dest, max_ttl, packet_size, timeout, pings_per_hop, delay_between_pings, source_ip)
        
        # Call the function
        tracert(dest, max_ttl, packet_size, timeout, pings_per_hop, delay_between_pings, source_ip, output_file)

    except ValueError as ve:
        print(f"Input Error: {ve}")
    except Exception as e:
        print(f"Unexpected error: {e}")
