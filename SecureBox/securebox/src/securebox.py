import os
import json
import curses
import curses.textpad
from datetime import datetime
from .logger import SecureBoxLogger
from .driveHandler import GoogleDriveHandler
from .models import Container, ContainerVersion
from .crypto import CryptoHandler

class SecureBox:
    """
    Clase principal de SecureBox
    """
    def __init__(self, username, credentials_path="credentials.json"):
        """
        Inicializa SecureBox
        """
        self.username = username
        self.db_path = f"{username}.db" if username else None
        self.containers = {}
        self.master_key = None
        self.salt = None
        self.logger = SecureBoxLogger()
        self.drive_handler = GoogleDriveHandler(credentials_path)
        self.crypto_handler = None

    def initialize(self, master_password):
        """
        Inicializa el vault con una contraseña maestra
        """
        self.salt = os.urandom(16)
        self.master_key = CryptoHandler.derive_key(master_password, self.salt)
        self.crypto_handler = CryptoHandler(self.master_key)
        self.containers = {}
        self.container_versions = {}
        self._save_vault()
        
        try:
            self.drive_handler.authenticate()
            self.logger.logger.info("Google Drive configurado exitosamente")
        except Exception as e:
            self.logger.logger.error(f"Error configurando Google Drive: {e}")

    @staticmethod
    def user_exists(username):
        """
        Verifica si existe un vault para el usuario especificado
        """
        return os.path.exists(f"{username}.db")

    def list_containers(self):
        """
        Lista los contenedores existentes
        """
        return list(self.containers.keys())

    def _backup_to_drive(self):
        """
        Realiza backup automático a Google Drive
        """
        try:
            if not self.drive_handler.service:
                self.drive_handler.authenticate()
            
            print(f"Iniciando backup de {self.db_path} a Google Drive...")
            if self.drive_handler.upload(self.db_path):
                self.logger.logger.info("Backup realizado exitosamente")
                return True
            return False
        except Exception as e:
            self.logger.logger.error(f"Error en backup: {e}")
            return False

    def restore_from_drive(self):
        """
        Restaura el vault desde Google Drive
        """
        try:
            print(f"Iniciando restauración de {self.db_path} desde Google Drive...")
            temp_path = f"temp_{self.db_path}"
            
            if self.drive_handler.download(self.db_path, temp_path):
                with open(temp_path, 'rb') as f:
                    backup_data = f.read()
                    
                if len(backup_data) >= 16:
                    if os.path.exists(self.db_path):
                        backup = f"{self.db_path}.old"
                        os.rename(self.db_path, backup)
                    
                    os.rename(temp_path, self.db_path)
                    print("Restauración completada exitosamente")
                    return True
                else:
                    print("Error: Archivo corrupto o incompleto")
                    os.remove(temp_path)
                    return False
        except Exception as e:
            print(f"Error en restauración: {e}")
            if os.path.exists(temp_path):
                os.remove(temp_path)
            return False
        
    def create_container(self, name):
        """
        Crea un nuevo contenedor
        """
        if name not in self.containers:
            self.containers[name] = Container(name)
            self._save_vault()
            self.logger.logger.info(f"Contenedor creado: {name}")
            self._backup_to_drive()
            return True
        return False
    
    def edit_container(self, name):
        """
        Edita un contenedor existente
        """
        if name not in self.containers:
            return False
            
        def edit_window(stdscr):
            curses.start_color()
            curses.init_pair(1, curses.COLOR_WHITE, curses.COLOR_BLUE)
            
            stdscr.clear()
            stdscr.addstr(0, 0, f"Editando contenedor: {name}", curses.color_pair(1))
            stdscr.addstr(1, 0, "Presione Ctrl-G para guardar y salir", curses.color_pair(1))
            
            height = curses.LINES - 4
            width = curses.COLS - 2
            edit_win = curses.newwin(height, width, 3, 1)
            
            edit_win.addstr(0, 0, self.containers[name].content)
            edit_win.refresh()
            
            box = curses.textpad.Textbox(edit_win, insert_mode=True)
            content = box.edit()
            
            self.containers[name].edit(content.rstrip())
            
        try:
            curses.wrapper(edit_window)
            self._save_vault()
            self.logger.logger.info(f"Contenedor editado: {name}")
            self._backup_to_drive()
            return True
        except Exception as e:
            self.logger.logger.error(f"Error editando contenedor {name}: {e}")
            return False
    
    def view_container(self, name):
        """
        Obtiene el contenido de un contenedor
        """
        if name in self.containers:
            return self.containers[name].content
        return None
    
    def delete_container(self, name):
        """
        Elimina un contenedor
        """
        if name in self.containers:
            del self.containers[name]
            self._save_vault()
            self.logger.logger.info(f"Contenedor eliminado: {name}")
            return True
        return False
    
    def get_container_history(self, name):
        """
        Obtiene el historial de versiones de un contenedor
        """
        if name in self.containers:
            return self.containers[name].get_history()
        return []
    
    def restore_version(self, name, timestamp):
        """
        Restaura una versión específica de un contenedor
        """
        if name in self.containers:
            if self.containers[name].restore_version(timestamp):
                self._save_vault()
                self.logger.logger.info(f"Versión restaurada para {name}: {timestamp}")
                self._backup_to_drive()
                return True
        return False
    
    def _save_vault(self):
        """
        Guarda el estado del vault
        """
        data = {
            'containers': {
                name: container.to_dict() 
                for name, container in self.containers.items()
            }
        }

        encrypted_data = self.crypto_handler.encrypt(json.dumps(data))

        with open(self.db_path, 'wb') as f:
            f.write(self.salt + encrypted_data)

        self.logger.logger.info("Vault guardado correctamente.")
    
    def unlock(self, password):
        """
        Desbloquea el vault utilizando la contraseña maestra.
        """
        if not os.path.exists(self.db_path):
            return False, "El vault no existe."

        try:
            with open(self.db_path, 'rb') as f:
                salt = f.read(16)
                encrypted_data = f.read()

            self.salt = salt
            self.master_key = CryptoHandler.derive_key(password, self.salt)
            
            self.crypto_handler = CryptoHandler(self.master_key)

            decrypted_json = self.crypto_handler.decrypt(encrypted_data)
            data = json.loads(decrypted_json)

            self.containers = {
                name: Container.from_dict(container_data)
                for name, container_data in data['containers'].items()
            }
            
            try:
                self.drive_handler.authenticate()
                self.logger.logger.info("Google Drive configurado exitosamente")
            except Exception as e:
                self.logger.logger.error(f"Error configurando Google Drive: {e}")

            return True, None

        except Exception as e:
            return False, str(e)
