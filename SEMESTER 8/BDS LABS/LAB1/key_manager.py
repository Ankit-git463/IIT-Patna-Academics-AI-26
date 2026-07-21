import os
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization


def generate_rsa_keypair():
    """Generates a private and public RSA key pair (2048-bit)."""
    ankit_2201ai47_private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048
    )
    ankit_2201ai47_public_key = ankit_2201ai47_private_key.public_key()
    return ankit_2201ai47_private_key, ankit_2201ai47_public_key


def save_private_key( ankit_2201ai47_private_key, ankit_2201ai47_filename, ankit_2201ai47_password ):
    """Saves private key to file, encrypted with a password."""
    if isinstance(ankit_2201ai47_password, str):
        ankit_2201ai47_password = ankit_2201ai47_password.encode()

    ankit_2201ai47_pem = ankit_2201ai47_private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.BestAvailableEncryption(
            ankit_2201ai47_password
        )
    )

    with open(ankit_2201ai47_filename, 'wb') as f:
        f.write(ankit_2201ai47_pem)


def load_private_key( ankit_2201ai47_filename, ankit_2201ai47_password ):
    """Loads a private key from file using a password."""
    if isinstance(ankit_2201ai47_password, str):
        ankit_2201ai47_password = ankit_2201ai47_password.encode()

    with open(ankit_2201ai47_filename, 'rb') as f:
        ankit_2201ai47_pem_data = f.read()

    return serialization.load_pem_private_key(
        ankit_2201ai47_pem_data,
        password=ankit_2201ai47_password
    )


def save_public_key( ankit_2201ai47_public_key, ankit_2201ai47_filename ):
    """Saves public key to file (PEM format)."""
    ankit_2201ai47_pem = ankit_2201ai47_public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

    with open(ankit_2201ai47_filename, 'wb') as f:
        f.write(ankit_2201ai47_pem)


def load_public_key( ankit_2201ai47_filename ):
    """Loads a public key from file."""
    with open(ankit_2201ai47_filename, 'rb') as f:
        ankit_2201ai47_pem_data = f.read()

    return serialization.load_pem_public_key(
        ankit_2201ai47_pem_data
    )
