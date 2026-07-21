import hashlib
import requests
import re
import math
import getpass
import difflib


# qwerty
# @nk!T-!!t-9@Tn@
# Modi@1234


USERNAME = "ankit_2201ai47"
MIN_LENGTH = 12

COMMON_PATTERNS = ["qwerty", "12345", "password", "admin", "welcome"]
SPECIAL_CHARS = "!@#$%^&*()_+-=[]{}|;:,.<>?"

# Breach check fxn 

def ankit_2201ai47_check_breach(hash_prefix):

    # Uses HaveIBeenPwned Passwords API (k-anonymity).
    
    url = f"https://api.pwnedpasswords.com/range/{hash_prefix}"
    try:
        response = requests.get(url, timeout=5)
        if response.status_code == 200:
            return response.text
        return None
    except requests.RequestException:
        print("Warning: Could not connect to breach database.")
        return None


# Strength scoring function

def ankit_2201ai47_password_strength(password):
    length = len(password)
    pool = 0

    if re.search(r"[a-z]", password): pool += 26
    if re.search(r"[A-Z]", password): pool += 26
    if re.search(r"[0-9]", password): pool += 10
    if re.search(r"[" + re.escape(SPECIAL_CHARS) + r"]", password): pool += 32

    entropy = length * math.log2(pool) if pool > 0 else 0
    score = min(100, int(entropy * 0.8))

    guesses = 2 ** entropy
    seconds = guesses / 1e10

    if seconds > 365 * 24 * 3600:
        crack_time = f"{int(seconds / (365*24*3600))} years"
    elif seconds > 24 * 3600:
        crack_time = f"{int(seconds / (24*3600))} days"
    elif seconds > 3600:
        crack_time = f"{int(seconds / 3600)} hours"
    else:
        crack_time = "Instantly"

    return {
        "entropy": int(entropy),
        "score": score,
        "crack_time": crack_time
    }


# Password Validation

def ankit_2201ai47_validate_password(password):
    results = {
        "checks": [],
        "errors": [],
        "breached": False,
        "mandatory_failed": False
    }

    lower_pw = password.lower()

    # Length check
    if len(password) >= MIN_LENGTH:
        results["checks"].append(f"Length: {len(password)} characters (minimum {MIN_LENGTH})")
    else:
        results["errors"].append(f"Length: {len(password)} characters (minimum {MIN_LENGTH})")
        results["mandatory_failed"] = True

    # Character rules
    rules = {
        "uppercase letters": r"[A-Z]",
        "lowercase letters": r"[a-z]",
        "numbers": r"[0-9]",
        "special characters": r"[" + re.escape(SPECIAL_CHARS) + r"]"
    }

    for name, pattern in rules.items():
        count = len(re.findall(pattern, password))
        if count > 0:
            results["checks"].append(f"Contains {name}: {count}")
        else:
            results["errors"].append(f"Contains {name}: 0")
            results["mandatory_failed"] = True

    # Common patterns
    for pattern in COMMON_PATTERNS:
        if pattern in lower_pw:
            results["errors"].append(f'Contains common pattern: "{pattern}" detected')
            break
    else:
        results["checks"].append("No common patterns detected")

    # Username similarity
    similarity = difflib.SequenceMatcher(None, lower_pw, USERNAME.lower()).ratio()
    if similarity > 0.5:
        results["errors"].append(f'Similar to username "{USERNAME}": {int(similarity*100)}% match')
    else:
        results["checks"].append(f'Not similar to username "{USERNAME}"')

    # Breach check
    sha1 = hashlib.sha1(password.encode()).hexdigest().upper()
    prefix, suffix = sha1[:5], sha1[5:]

    breach_data = ankit_2201ai47_check_breach(prefix)
    if breach_data:
        for line in breach_data.splitlines():
            h, count = line.split(":")
            if h == suffix:
                results["errors"].append(f"Found in known data breaches ({count} times)")
                results["breached"] = True
                break
        else:
            results["checks"].append("Not in known data breaches")

    return results


# Recommendations

def ankit_2201ai47_recommendations(validation, strength):
    recs = []

    if validation["mandatory_failed"]:
        recs.append("Ensure password has at least 12 characters with uppercase, lowercase, number, and special character")

    if any("pattern" in e.lower() for e in validation["errors"]):
        recs.append('Avoid keyboard patterns like "qwerty"')

    if any("username" in e.lower() for e in validation["errors"]):
        recs.append("Make password less similar to personal information")

    if strength["entropy"] < 60:
        recs.append("Consider adding more random characters")

    if validation["breached"]:
        recs.append("Do not reuse passwords found in data breaches")

    return recs


def main():
    print("\n=== PASSWORD VALIDATION ===")
    print(f"User Account: {USERNAME}")
    print("-" * 45)

    password = getpass.getpass("Enter new password: ")
    if not password:
        password = input("Enter new password (visible): ")

    # Call logic ONCE
    validation = ankit_2201ai47_validate_password(password)
    strength = ankit_2201ai47_password_strength(password)
    recs = ankit_2201ai47_recommendations(validation, strength)

    print("\nVALIDATION RESULTS:")
    for c in validation["checks"]:
        print("✓", c)
    for e in validation["errors"]:
        print("✗", e)

    print("\nSTRENGTH ANALYSIS:")
    print(f"Entropy: {strength['entropy']} bits")
    print(f"Estimated crack time: {strength['crack_time']}")
    print(f"Strength score: {strength['score']}/100")

    print("\nRECOMMENDATIONS:")
    if recs:
        for i, r in enumerate(recs, 1):
            print(f"{i}. {r}")
    else:
        print("No recommendations. Password is strong.")

    print("\nFINAL STATUS:")
    if validation["mandatory_failed"]:
        print("Password rejected")
    elif strength["score"] >= 80 and strength["entropy"] >= 60 and not validation["breached"]:
        print("Password acceptable")
    else:
        print("Password acceptable but could be stronger")


if __name__ == "__main__":
    main()
