import os
import sys
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import padding
import crypto_utils
import key_manager


def main():
    print("=== SENDER SIDE ===")

    ankit_2201ai47_input_file = input("File to encrypt: ").strip()
    if not os.path.exists(ankit_2201ai47_input_file):
        print(f"Error: File '{ankit_2201ai47_input_file}' not found.")
        return

    # 1. Generate AES Key
    print("Generating AES-256 key...")
    ankit_2201ai47_aes_key = crypto_utils.generate_aes_key()

    # 2. Encrypt File
    print("Encrypting file...")
    try:
        with open(ankit_2201ai47_input_file, 'rb') as f:
            ankit_2201ai47_data = f.read()

        ankit_2201ai47_encrypted_data = crypto_utils.encrypt_data_aes_cbc(
            ankit_2201ai47_data,
            ankit_2201ai47_aes_key
        )

        # Extract info for display
        ankit_2201ai47_iv = ankit_2201ai47_encrypted_data[:16]
        ankit_2201ai47_mac = ankit_2201ai47_encrypted_data[-32:]

        print(f"IV generated: {ankit_2201ai47_iv.hex()[:16]}...")
        print(f"HMAC computed: sha256-{ankit_2201ai47_mac.hex()[:6]}...")

        ankit_2201ai47_output_enc_file = (
            ankit_2201ai47_input_file + ".enc"
        )
        with open(ankit_2201ai47_output_enc_file, 'wb') as f:
            f.write(ankit_2201ai47_encrypted_data)

        print(
            f"Encrypted file saved as: {ankit_2201ai47_output_enc_file}"
        )

    except Exception as e:
        print(f"Encryption failed: {e}")
        return

    # 3. Encrypt AES Key with Receiver's Public Key
    ankit_2201ai47_receiver_pub_file = "receiver_pub.pem"
    if not os.path.exists(ankit_2201ai47_receiver_pub_file):
        print(
            f"Error: Receiver public key '{ankit_2201ai47_receiver_pub_file}' not found."
        )
        print(
            "Please ensure the receiver has generated keys and 'receiver_pub.pem' is available."
        )
        return

    try:
        ankit_2201ai47_public_key = key_manager.load_public_key(
            ankit_2201ai47_receiver_pub_file
        )

        ankit_2201ai47_encrypted_key = ankit_2201ai47_public_key.encrypt(
            ankit_2201ai47_aes_key,
            padding.OAEP(
                mgf=padding.MGF1(algorithm=hashes.SHA256()),
                algorithm=hashes.SHA256(),
                label=None
            )
        )

        ankit_2201ai47_key_output = "rsa_encrypted_key.bin"
        with open(ankit_2201ai47_key_output, 'wb') as f:
            f.write(ankit_2201ai47_encrypted_key)

        print(
            f"AES key encrypted with RSA: {ankit_2201ai47_key_output}"
        )

    except Exception as e:
        print(f"Key encryption failed: {e}")


if __name__ == "__main__":
    main()
