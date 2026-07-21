import os
import sys
import getpass
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import padding
import crypto_utils
import key_manager
import hashlib


def setup_keys():
    print("Generating new RSA-2048 keypair for Receiver...")
    
    ankit_2201ai47_password = getpass.getpass(
        "Enter a password to protect your private key: "
    )
    ankit_2201ai47_confirm = getpass.getpass("Confirm password: ")
    
    if ankit_2201ai47_password != ankit_2201ai47_confirm:
        print("Passwords do not match!")
        return

    ankit_2201ai47_priv, ankit_2201ai47_pub = key_manager.generate_rsa_keypair()
    
    key_manager.save_private_key(
        ankit_2201ai47_priv,
        "receiver_priv.pem",
        ankit_2201ai47_password
    )
    key_manager.save_public_key(
        ankit_2201ai47_pub,
        "receiver_pub.pem"
    )
    
    print("Keys generated successfully: receiver_priv.pem, receiver_pub.pem")


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--setup":
        setup_keys()
        return

    print("=== RECEIVER SIDE ===")

    if not os.path.exists("receiver_priv.pem"):
        print(
            "Private key not found. Run 'python3 receiver.py --setup' first to generate keys."
        )
        return

    ankit_2201ai47_encrypted_key_file = "rsa_encrypted_key.bin"
    
    if not os.path.exists(ankit_2201ai47_encrypted_key_file):
        print(
            f"Error: '{ankit_2201ai47_encrypted_key_file}' not found. Waiting for sender..."
        )
        return

    # 1. Decrypt AES Key
    print(f"Received encrypted key: {ankit_2201ai47_encrypted_key_file}")
    
    ankit_2201ai47_password = getpass.getpass(
        "Enter password for private key: "
    )

    try:
        print("Decrypting AES key with private key...")
        
        ankit_2201ai47_private_key = key_manager.load_private_key(
            "receiver_priv.pem",
            ankit_2201ai47_password
        )

        with open(ankit_2201ai47_encrypted_key_file, 'rb') as f:
            ankit_2201ai47_encrypted_key_data = f.read()

        ankit_2201ai47_aes_key = ankit_2201ai47_private_key.decrypt(
            ankit_2201ai47_encrypted_key_data,
            padding.OAEP(
                mgf=padding.MGF1(algorithm=hashes.SHA256()),
                algorithm=hashes.SHA256(),
                label=None
            )
        )
        print("AES key recovered successfully")

    except ValueError:
        print("Error: Invalid password or corrupted key.")
        return
    except Exception as e:
        print(f"Key decryption failed: {e}")
        return

    # 2. Decrypt File
    ankit_2201ai47_input_file_enc = input(
        "Enter encrypted filename (e.g., document.pdf.enc): "
    ).strip()

    if not os.path.exists(ankit_2201ai47_input_file_enc):
        print(f"Error: File '{ankit_2201ai47_input_file_enc}' not found.")
        return

    print(f"Loading encrypted file: {ankit_2201ai47_input_file_enc}")

    try:
        with open(ankit_2201ai47_input_file_enc, 'rb') as f:
            ankit_2201ai47_encrypted_data = f.read()

        print("Verifying HMAC...", end=" ")
        
        ankit_2201ai47_decrypted_data = crypto_utils.decrypt_data_aes_cbc(
            ankit_2201ai47_encrypted_data,
            ankit_2201ai47_aes_key
        )
        print("✓ Valid")

        print("Decrypting file...")
        
        ankit_2201ai47_original_filename = ankit_2201ai47_input_file_enc.replace(
            ".enc", ""
        )
        ankit_2201ai47_output_filename = (
            "decrypted_" + ankit_2201ai47_original_filename
        )

        with open(ankit_2201ai47_output_filename, 'wb') as f:
            f.write(ankit_2201ai47_decrypted_data)

        print(f"Original file restored: {ankit_2201ai47_output_filename}")

        # Optional hash verification
        if os.path.exists(ankit_2201ai47_original_filename):
            with open(ankit_2201ai47_original_filename, 'rb') as f:
                ankit_2201ai47_orig_hash = hashlib.sha256(f.read()).hexdigest()
            
            with open(ankit_2201ai47_output_filename, 'rb') as f:
                ankit_2201ai47_new_hash = hashlib.sha256(f.read()).hexdigest()

            if ankit_2201ai47_orig_hash == ankit_2201ai47_new_hash:
                print("SHA256 match with original: ✓ Verified")
            else:
                print("SHA256 match with original: ✗ Mismatch")

    except Exception as e:
        print(f"\nDecryption failed: {e}")


if __name__ == "__main__":
    main()
