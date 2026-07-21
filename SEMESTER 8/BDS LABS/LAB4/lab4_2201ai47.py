import hashlib
import os
import hmac
from datetime import datetime, timedelta
from getpass import getpass


ankit_2201ai47_db = {}

# Fxn to register the user 

def register_user(ankit_2201ai47_username, ankit_2201ai47_password):
    print("\n=== USER REGISTRATION ===")
    print(f"Username: {ankit_2201ai47_username}")
    print("Password: **********")

    # Generate 32-byte random salt
    ankit_2201ai47_salt = os.urandom(32)
    print(f"Generating salt: {ankit_2201ai47_salt.hex()[:16]}...")

    ankit_2201ai47_iterations = 100000
    print("Applying PBKDF2-HMAC-SHA256 (100,000 iterations)...")

    # Generate password hash
    ankit_2201ai47_password_hash = hashlib.pbkdf2_hmac(
        'sha256',
        ankit_2201ai47_password.encode(),
        ankit_2201ai47_salt,
        ankit_2201ai47_iterations,
        dklen=32
    )

    print(f"Password hash: {ankit_2201ai47_password_hash.hex()[:16]}...")
    print("Storing in database...")

    # Store user details
    ankit_2201ai47_db[ankit_2201ai47_username] = {
        "salt": ankit_2201ai47_salt,
        "hash": ankit_2201ai47_password_hash,
        "iterations": ankit_2201ai47_iterations,
        "failed_attempts": 0,
        "locked_until": None
    }

    print("REGISTRATION SUCCESSFUL")
    print("Salt stored: [32 bytes]")
    print("Hash stored: [32 bytes]")
    print("Iterations: 100000")

# Authentication is done here
def authenticate(ankit_2201ai47_username, ankit_2201ai47_password):
    print("\n=== USER LOGIN ===")

    if ankit_2201ai47_username not in ankit_2201ai47_db:
        print("User not found!")
        return

    ankit_2201ai47_user = ankit_2201ai47_db[ankit_2201ai47_username]

    # Check if account is locked
    if ankit_2201ai47_user["locked_until"]:
        if datetime.now() < ankit_2201ai47_user["locked_until"]:
            print("Account locked. Please try again later.")
            return
        else:
            ankit_2201ai47_user["locked_until"] = None
            ankit_2201ai47_user["failed_attempts"] = 0

    print("Retrieving salt and hash...")
    print("Recomputing hash...")

    # Recompute hash
    ankit_2201ai47_new_hash = hashlib.pbkdf2_hmac(
        'sha256',
        ankit_2201ai47_password.encode(),
        ankit_2201ai47_user["salt"],
        ankit_2201ai47_user["iterations"],
        dklen=32
    )

    # Constant-time comparison
    if hmac.compare_digest(ankit_2201ai47_new_hash, ankit_2201ai47_user["hash"]):
        print("Hash comparison: ✓ Match")
        print("Login successful!")
        ankit_2201ai47_user["failed_attempts"] = 0
    else:
        ankit_2201ai47_user["failed_attempts"] += 1
        remaining = 5 - ankit_2201ai47_user["failed_attempts"]

        print("Hash comparison: ✗ No match")
        print(f"Failed attempts: {ankit_2201ai47_user['failed_attempts']}/5")

        if ankit_2201ai47_user["failed_attempts"] >= 5:
            lock_time = 2 ** ankit_2201ai47_user["failed_attempts"]
            ankit_2201ai47_user["locked_until"] = datetime.now() + timedelta(seconds=lock_time)
            print(f"Account locked for {lock_time} seconds due to multiple failures.")
        else:
            print(f"Warning: {remaining} attempts remaining before lockout")


while True:
    print("\n==============================")
    print(" Secure Password Storage Menu ")
    print("==============================")
    print("1. Register")
    print("2. Login")
    print("3. Exit")

    choice = input("Enter choice: ")

    if choice == "1":
        ankit_2201ai47_uname = input("Enter username: ")
        ankit_2201ai47_pwd = getpass("Enter password: ")
        register_user(ankit_2201ai47_uname, ankit_2201ai47_pwd)

    elif choice == "2":
        ankit_2201ai47_uname = input("Enter username: ")
        ankit_2201ai47_pwd = getpass("Enter password: ")
        authenticate(ankit_2201ai47_uname, ankit_2201ai47_pwd)

    elif choice == "3":
        print("Exiting system...")
        break

    else:
        print("Invalid choice. Try again.")