import random

class SMPCProtocol:
    def __init__(self, participants):
        self.n = len(participants)
        self.participants = participants
        self.modulus = 2**32  # 32-bit modulus

    def share_secret(self, secret, participant_id):
        """Generate n-1 random shares; last share = secret - sum(shares) mod modulus."""
        shares = {}
        other_pids = [p for p in self.participants if p != participant_id]

        # Generate n-1 random shares for other participants
        random_shares = [random.randint(1, 100) for _ in range(self.n - 1)]

        for i, pid in enumerate(other_pids):
            shares[pid] = random_shares[i]

        # Own share = secret - sum(other shares) mod modulus
        own_share = (secret - sum(random_shares)) % self.modulus
        shares[participant_id] = own_share

        return shares

    def compute_partial_sum(self, received_shares):
        """Sum all received shares mod modulus."""
        return sum(received_shares.values()) % self.modulus

    def reconstruct_result(self, partial_sums):
        """Sum all partial sums mod modulus."""
        return sum(partial_sums.values()) % self.modulus


def run_smpc(participant_names, private_inputs):
    """
    Run the full SMPC protocol.

    Args:
        participant_names : list of str   e.g. ["Alice", "Bob", "Charlie", "David"]
        private_inputs    : list of int   e.g. [42, 17, 100, 255]
    """
    assert len(participant_names) == len(private_inputs), \
        "Each participant must have exactly one private input."

    protocol = SMPCProtocol(participant_names)
    secrets = dict(zip(participant_names, private_inputs))
    expected_sum = sum(private_inputs)

    # ── Header ──────────────────────────────────────────────────────────────
    print("=" * 45)
    print("    SECURE MULTIPARTY COMPUTATION")
    print("=" * 45)
    print(f"Participants: {protocol.n} ({', '.join(participant_names)})")
    print(f"Modulus: 2^32 ({protocol.modulus})")
    print("\nPrivate inputs:")
    for name, val in secrets.items():
        print(f"  {name}: {val}")
    sum_expr = " + ".join(str(v) for v in private_inputs)
    print(f"\nExpected sum: {sum_expr} = {expected_sum}")

    input("\nPress Enter for Secret Sharing... ")

    # ── Phase 1: Secret Sharing ──────────────────────────────────────────────
    print("\n" + "=" * 45)
    print("  PHASE 1: SECRET SHARING")
    print("=" * 45)

    all_shares = {}   # all_shares[sharer][receiver] = share value

    for name in participant_names:
        secret = secrets[name]
        shares = protocol.share_secret(secret, name)
        all_shares[name] = shares

        print(f"\n{name}'s secret: {secret}")
        print("  Generating shares:")
        for receiver, share in shares.items():
            if receiver != name:
                print(f"    Share to {receiver}: {share}")
        own_raw = secret - sum(v for k, v in shares.items() if k != name)
        own_mod = shares[name]
        if own_raw < 0:
            print(f"    {name}'s own share: {own_raw} (mod 2^32: {own_mod})")
        else:
            print(f"    {name}'s own share: {own_mod}")

    input("\nPress Enter to Share Distribution... ")
    # ── Phase 2: Share Distribution ──────────────────────────────────────────
    print("\n" + "=" * 45)
    print("  PHASE 2: SHARE DISTRIBUTION")
    print("=" * 45)

    # received_shares[receiver] = {sender: share}
    received_shares = {name: {} for name in participant_names}
    for sharer, shares in all_shares.items():
        for receiver, share in shares.items():
            received_shares[receiver][sharer] = share

    for receiver in participant_names:
        others = {s: v for s, v in received_shares[receiver].items() if s != receiver}
        parts = [f"{v} from {s}" for s, v in others.items()]
        print(f"\n{receiver} receives: [{', '.join(parts)}]")

    input("\nPress Enter for Partial Sum Computation... ")
    # ── Phase 3: Partial Sum Computation ─────────────────────────────────────
    print("\n" + "=" * 45)
    print("  PHASE 3: PARTIAL SUM COMPUTATION")
    print("=" * 45)

    partial_sums = {}

    for name in participant_names:
        shares_for_name = received_shares[name]   # includes own share
        print(f"\n{name} computes partial sum:")
        print(f"  Own share: {shares_for_name[name]}")
        for sender, val in shares_for_name.items():
            if sender != name:
                print(f"  From {sender}: {val}")
        raw_total = sum(shares_for_name.values())
        partial = raw_total % protocol.modulus
        partial_sums[name] = partial
        if raw_total != partial:
            print(f"  Partial sum: {raw_total} mod 2^32 = {partial}")
        else:
            print(f"  Partial sum: {partial}")

    input("\nPress Enter for Result Reconstruction... ")
    # ── Phase 4: Result Reconstruction ───────────────────────────────────────
    print("\n" + "=" * 45)
    print("  PHASE 4: RESULT RECONSTRUCTION")
    print("=" * 45)
    print("\nCollecting partial sums:")
    for name, ps in partial_sums.items():
        print(f"  {name}: {ps}")

    ps_expr = " + ".join(str(v) for v in partial_sums.values())
    final_result = protocol.reconstruct_result(partial_sums)
    print(f"\nFinal sum: {ps_expr} = {final_result} mod 2^32")

    input("\nPress Enter for Verification... ")

    # ── Verification ─────────────────────────────────────────────────────────
    print("\n" + "=" * 45)
    print("  VERIFICATION")
    print("=" * 45)
    print(f"\nComputed sum : {final_result}")
    print(f"Expected sum : {expected_sum}")
    if final_result == expected_sum:
        print("✓  Result is CORRECT — privacy preserved throughout!")
    else:
        print("✗  Mismatch — something went wrong.")

    return final_result


# ── Entry point ───────────────────────────────────────────────────────────────
if __name__ == "__main__":
    print("=" * 45)
    print("    SECURE MULTIPARTY COMPUTATION SETUP")
    print("=" * 45)

    # ── Number of participants ───────────────────────────────────────────────
    while True:
        try:
            n = int(input("\nEnter number of participants: "))
            if n < 2:
                print("  ✗ Need at least 2 participants. Try again.")
            else:
                break
        except ValueError:
            print("  ✗ Please enter a valid integer.")

    # ── Participant names ────────────────────────────────────────────────────
    participant_names = []
    print(f"\nEnter {n} participant names:")
    for i in range(n):
        while True:
            name = input(f"  Participant {i+1} name: ").strip()
            if not name:
                print("  ✗ Name cannot be empty.")
            elif name in participant_names:
                print("  ✗ Name already used. Choose a different name.")
            else:
                participant_names.append(name)
                break

    # ── Private inputs ───────────────────────────────────────────────────────
    private_inputs = []
    print(f"\nEnter private integer value for each participant:")
    for name in participant_names:
        while True:
            try:
                val = int(input(f"  {name}'s secret value: "))
                private_inputs.append(val)
                break
            except ValueError:
                print("  ✗ Please enter a valid integer.")

    # ── Run the protocol ─────────────────────────────────────────────────────
    print()
    run_smpc(participant_names, private_inputs)