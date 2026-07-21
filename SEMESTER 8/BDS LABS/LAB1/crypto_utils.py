import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding, hashes, hmac
from cryptography.hazmat.backends import default_backend


def generate_aes_key():
    """Generates a 256-bit (32 bytes) random AES key."""
    return os.urandom(32)


def encrypt_data_aes_cbc(ankit_2201ai47_data, ankit_2201ai47_key):
    """
    Encrypts data using AES-256-CBC with PKCS7 padding and HMAC-SHA256.
    Returns: iv + ciphertext + hmac
    """

    # 1. Generate IV
    ankit_2201ai47_iv = os.urandom(16)

    # 2. Pad data (PKCS7)
    ankit_2201ai47_padder = padding.PKCS7(128).padder()
    ankit_2201ai47_padded_data = (
        ankit_2201ai47_padder.update(ankit_2201ai47_data)
        + ankit_2201ai47_padder.finalize()
    )

    # 3. Encrypt
    ankit_2201ai47_cipher = Cipher(
        algorithms.AES(ankit_2201ai47_key),
        modes.CBC(ankit_2201ai47_iv),
        backend=default_backend()
    )
    ankit_2201ai47_encryptor = ankit_2201ai47_cipher.encryptor()
    ankit_2201ai47_ciphertext = (
        ankit_2201ai47_encryptor.update(ankit_2201ai47_padded_data)
        + ankit_2201ai47_encryptor.finalize()
    )

    # 4. Compute HMAC (over IV + ciphertext)
    ankit_2201ai47_h = hmac.HMAC(
        ankit_2201ai47_key,
        hashes.SHA256(),
        backend=default_backend()
    )
    ankit_2201ai47_h.update(
        ankit_2201ai47_iv + ankit_2201ai47_ciphertext
    )
    ankit_2201ai47_mac = ankit_2201ai47_h.finalize()

    return (
        ankit_2201ai47_iv
        + ankit_2201ai47_ciphertext
        + ankit_2201ai47_mac
    )


def decrypt_data_aes_cbc(
    ankit_2201ai47_encrypted_blob,
    ankit_2201ai47_key
):
    """
    Verifies HMAC and decrypts data using AES-256-CBC.
    encrypted_blob: iv + ciphertext + hmac
    Returns: decrypted bytes
    """

    # Validate length (IV = 16 bytes, HMAC = 32 bytes)
    if len(ankit_2201ai47_encrypted_blob) < 16 + 32:
        raise ValueError("Data too short to contain IV and HMAC")

    ankit_2201ai47_mac = ankit_2201ai47_encrypted_blob[-32:]
    ankit_2201ai47_iv_ciphertext = ankit_2201ai47_encrypted_blob[:-32]
    ankit_2201ai47_iv = ankit_2201ai47_iv_ciphertext[:16]
    ankit_2201ai47_ciphertext = ankit_2201ai47_iv_ciphertext[16:]

    # 1. Verify HMAC
    ankit_2201ai47_h = hmac.HMAC(
        ankit_2201ai47_key,
        hashes.SHA256(),
        backend=default_backend()
    )
    ankit_2201ai47_h.update(ankit_2201ai47_iv_ciphertext)
    ankit_2201ai47_h.verify(ankit_2201ai47_mac)

    # 2. Decrypt
    ankit_2201ai47_cipher = Cipher(
        algorithms.AES(ankit_2201ai47_key),
        modes.CBC(ankit_2201ai47_iv),
        backend=default_backend()
    )
    ankit_2201ai47_decryptor = ankit_2201ai47_cipher.decryptor()
    ankit_2201ai47_padded_data = (
        ankit_2201ai47_decryptor.update(ankit_2201ai47_ciphertext)
        + ankit_2201ai47_decryptor.finalize()
    )

    # 3. Unpad
    ankit_2201ai47_unpadder = padding.PKCS7(128).unpadder()
    ankit_2201ai47_data = (
        ankit_2201ai47_unpadder.update(ankit_2201ai47_padded_data)
        + ankit_2201ai47_unpadder.finalize()
    )

    return ankit_2201ai47_data
