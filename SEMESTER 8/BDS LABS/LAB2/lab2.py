# Submission by Ankit Singh 
# 2201AI47 

import os, json, base64, hashlib, datetime
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

KEY_DIR = "keys"

# ===================== KEY GENERATION =====================
def generate_key_pair(ankit_2201ai47_user_id, ankit_2201ai47_password):
    
    private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048,
        backend=default_backend()
    )
    public_key = private_key.public_key()

    os.makedirs(KEY_DIR, exist_ok=True)
    print("Generating RSA-2048 key pair...")

    pub_path = f"{KEY_DIR}/{ankit_2201ai47_user_id}_pub.pem"
    priv_path = f"{KEY_DIR}/{ankit_2201ai47_user_id}_priv.pem"

    with open(pub_path, "wb") as f:
        f.write(public_key.public_bytes(
            serialization.Encoding.PEM,
            serialization.PublicFormat.SubjectPublicKeyInfo
        ))

    with open(priv_path, "wb") as f:
        f.write(private_key.private_bytes(
            serialization.Encoding.PEM,
            serialization.PrivateFormat.PKCS8,
            serialization.BestAvailableEncryption(ankit_2201ai47_password.encode())
        ))

    fingerprint = hashlib.sha256(
        public_key.public_bytes(
            serialization.Encoding.DER,
            serialization.PublicFormat.SubjectPublicKeyInfo
        )
    ).hexdigest()[:16]
    
    print(f"Public key fingerprint: SHA256:{fingerprint}...")
    print("Private key encrypted with password")
    print(f"Keys saved to: {priv_path}, {pub_path}\n")

    return private_key, public_key


# ===================== LOAD KEYS =====================
def load_private_key(ankit_2201ai47_user_id, ankit_2201ai47_password):
    with open(f"{KEY_DIR}/{ankit_2201ai47_user_id}_priv.pem", "rb") as f:
        return serialization.load_pem_private_key(
            f.read(),
            password=ankit_2201ai47_password.encode(),
            backend=default_backend()
        )


def load_public_key(ankit_2201ai47_user_id):
    with open(f"{KEY_DIR}/{ankit_2201ai47_user_id}_pub.pem", "rb") as f:
        return serialization.load_pem_public_key(
            f.read(),
            backend=default_backend()
        )


# ===================== ENCRYPT MESSAGE =====================
def encrypt_message(
    ankit_2201ai47_message,
    ankit_2201ai47_sender_private_key,
    ankit_2201ai47_recipient_public_key,
    ankit_2201ai47_sender_id
):
    session_key = os.urandom(16)
    print(f"Session key: aes128-{session_key.hex()[:6]}...")

    iv = os.urandom(16)

    cipher = Cipher(algorithms.AES(session_key), modes.CBC(iv))
    encryptor = cipher.encryptor()

    padded = ankit_2201ai47_message.encode()
    padded += b" " * (16 - len(padded) % 16)

    ciphertext = encryptor.update(padded) + encryptor.finalize()
    full_ciphertext = iv + ciphertext

    encrypted_key = ankit_2201ai47_recipient_public_key.encrypt(
        session_key,
        padding.OAEP(
            mgf=padding.MGF1(hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )

    timestamp = datetime.datetime.utcnow().isoformat() + "Z"

    signature = ankit_2201ai47_sender_private_key.sign(
        full_ciphertext,
        padding.PSS(
            mgf=padding.MGF1(hashes.SHA256()),
            salt_length=padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )

    package = {
        "ciphertext": base64.b64encode(full_ciphertext).decode(),
        "encrypted_key": base64.b64encode(encrypted_key).decode(),
        "signature": base64.b64encode(signature).decode(),
        "sender": ankit_2201ai47_sender_id,
        "timestamp": timestamp
    }

    return json.dumps(package, indent=4)


# ===================== DECRYPT MESSAGE =====================
def decrypt_message(
    ankit_2201ai47_encrypted_package,
    ankit_2201ai47_recipient_private_key,
    ankit_2201ai47_sender_public_key
):
    print("Loading encrypted message...")
    package = json.loads(ankit_2201ai47_encrypted_package)

    full_ciphertext = base64.b64decode(package["ciphertext"])
    encrypted_key = base64.b64decode(package["encrypted_key"])
    signature = base64.b64decode(package["signature"])
    sender_id = package.get("sender", "unknown")

    print(f"Verifying {sender_id}'s signature: ", end="")
    ankit_2201ai47_sender_public_key.verify(
        signature,
        full_ciphertext,
        padding.PSS(
            mgf=padding.MGF1(hashes.SHA256()),
            salt_length=padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )
    print("Valid")

    session_key = ankit_2201ai47_recipient_private_key.decrypt(
        encrypted_key,
        padding.OAEP(
            mgf=padding.MGF1(hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )

    iv = full_ciphertext[:16]
    ciphertext = full_ciphertext[16:]

    cipher = Cipher(algorithms.AES(session_key), modes.CBC(iv))
    decryptor = cipher.decryptor()
    plaintext = decryptor.update(ciphertext) + decryptor.finalize()

    return plaintext.decode().strip()


# ===================== MAIN RUN =====================
if __name__ == "__main__":

    print("\n=== USER REGISTRATION ===")
    ankit_2201ai47_alice_priv, ankit_2201ai47_alice_pub = generate_key_pair(
        "ankit_2201ai47_alice", "alice123"
    )

    ankit_2201ai47_bob_priv, ankit_2201ai47_bob_pub = generate_key_pair(
        "ankit_2201ai47_bob", "bob123"
    )

    print("=== SENDING MESSAGE ===")
    ankit_2201ai47_message = "Meeting at 3 PM tomorrow"

    encrypted_package = encrypt_message(
        ankit_2201ai47_message,
        ankit_2201ai47_alice_priv,
        ankit_2201ai47_bob_pub,
        "ankit_2201ai47_alice"
    )

    print("Encrypted package:")
    print(encrypted_package)

    print("\n=== RECEIVING MESSAGE ===")
    decrypted = decrypt_message(
        encrypted_package,
        ankit_2201ai47_bob_priv,
        ankit_2201ai47_alice_pub
    )

    print(f"Plaintext: \"{decrypted}\"")
    print("Message integrity: Verified")
    print("Sender authentication: Confirmed")
