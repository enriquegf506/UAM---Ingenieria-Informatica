import base64
import zlib
from cryptography.fernet import Fernet
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC

class CryptoHandler:
    """
    Clase para manejar la encriptación y desencriptación de datos
    """
    @staticmethod
    def derive_key(password, salt):
        """
        Deriva una clave a partir de una contraseña y un conjunto de bits aleatorios
        """
        kdf = PBKDF2HMAC(
            algorithm=hashes.SHA256(),
            length=32,
            salt=salt,
            iterations=100000,
        )
        return base64.urlsafe_b64encode(kdf.derive(password.encode()))

    @staticmethod
    def compress_data(data):
        """
        Comprime los datos utilizando zlib
        """
        return zlib.compress(data.encode())

    @staticmethod
    def decompress_data(data):
        """
        Descomprime los datos utilizando zlib
        """
        return zlib.decompress(data).decode()

    def __init__(self, master_key):
        """
        Inicializa el manejador de encriptación
        """
        self.master_key = master_key

    def encrypt(self, data):
        """
        Cifra y comprime los datos
        """
        compressed = self.compress_data(data)
        f = Fernet(self.master_key)
        return f.encrypt(compressed)

    def decrypt(self, encrypted_data):
        """
        Descifra y descomprime los datos
        """
        f = Fernet(self.master_key)
        decrypted = f.decrypt(encrypted_data)
        return self.decompress_data(decrypted)
