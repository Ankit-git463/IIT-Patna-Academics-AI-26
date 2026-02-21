def ping(dest_ip, count=4, ttl=64, size=32, timeout=2):
    times = []
    try:
        if count <= 0 or ttl <= 0 or size <= 0 or timeout <= 0:
            raise ValueError("Count, TTL, size, and timeout must be positive integers.")

        print(f"Pinging {dest_ip} with {count} packets of {size} bytes and TTL={ttl}:")
        for i in range(count):
            pkt = IP(dst=dest_ip, ttl=ttl)/ICMP()/("X" * size)
            start_time = time.time()
            reply = sr1(pkt, timeout=timeout, verbose=0)
            end_time = time.time()

            if reply:
                rtt = (end_time - start_time) * 1000  # Round Trip Time in milliseconds
                times.append(rtt)
                print(f"Reply from {dest_ip}: bytes={len(reply[IP])} time={rtt:.2f}ms TTL={reply.ttl}")
            else:
                print(f"Request timed out.")
        
        # Summary
        if times:
            packet_loss = ((count - len(times)) / count) * 100
            min_rtt = min(times)
            max_rtt = max(times)
            avg_rtt = sum(times) / len(times)
            
            print(f"\n--- {dest_ip} ping statistics ---")
            print(f"{count} packets transmitted, {len(times)} received, {packet_loss:.2f}% packet loss")
            print(f"rtt min/avg/max = {min_rtt:.2f}/{avg_rtt:.2f}/{max_rtt:.2f} ms")
        else:
            print(f"\n--- {dest_ip} ping statistics ---")
            print(f"{count} packets transmitted, 0 received, 100% packet loss")
                
    except ValueError as e:
        print(f"Invalid input: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")
