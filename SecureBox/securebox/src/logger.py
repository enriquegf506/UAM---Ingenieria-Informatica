import logging

class SecureBoxLogger:
    """
    Clase para manejar los logs de SecureBox
    """
    def __init__(self, log_file="securebox.log"):
        """
        Inicializa el manejador de logs
        """
        self.logger = logging.getLogger('SecureBox')
        self.logger.setLevel(logging.INFO)
        
        formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
        
        file_handler = logging.FileHandler(log_file)
        file_handler.setFormatter(formatter)
        self.logger.addHandler(file_handler)
        
        console_handler = logging.StreamHandler()
        console_handler.setFormatter(formatter)
        self.logger.addHandler(console_handler)