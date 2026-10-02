import os
import json
import yaml
import logging
import paho.mqtt.client as mqtt
from datetime import datetime

# Configurar logger para utils

class MQTTClient:
    def __init__(self, config_path):
        """
        Inicializa el cliente MQTT y carga la configuración desde un archivo YAML.
        Args:
            config_path (str): Ruta al archivo de configuración YAML.
            
        Raises:
            FileNotFoundError: Si el archivo de configuración no se encuentra.
            yaml.YAMLError: Si hay un error al cargar el archivo YAML.
        """
        try:
            with open(config_path, 'r') as f:
                self.config = yaml.safe_load(f)
        except FileNotFoundError:
            raise
        except yaml.YAMLError as e:
            raise
            
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.base_topic = f"redes2/{self.config['mqtt']['group']}/{self.config['mqtt']['pair']}"
    
    def connect(self, host, port):
        """
        Conecta al broker MQTT utilizando la configuración proporcionada.
        Args:
            host (str): Dirección del broker MQTT.
            port (int): Puerto del broker MQTT.
        
        """
        try:
            self.client.connect(host, port)
            self.client.loop_start()
        except Exception as e:
            raise
    
    def on_connect(self, client, userdata, flags, rc):
        """
        Callback que se ejecuta cuando se establece la conexión con el broker MQTT.
        Se suscribe a los topics configurados y publica el estado inicial.
        Args:
            client: Cliente MQTT que llamó al callback.
            userdata: Datos de usuario proporcionados por el cliente.
            flags: Indicadores proporcionados por el broker.
            rc (int): Código de resultado de la conexión.
        """
        pass
    
    def on_message(self, client, userdata, msg):
        """
        Callback que se ejecuta cuando se recibe un mensaje MQTT.
        Procesa los mensajes recibidos, actualiza el estado de los dispositivos,
        registra eventos y evalúa las reglas de automatización.
        Args:
            client: Cliente MQTT que recibió el mensaje.
            userdata: Datos de usuario proporcionados por el cliente.
            msg: Objeto de mensaje MQTT que contiene topic y payload.
        """
        pass
    
    def publish(self, topic, payload):
        """
        Publica un mensaje en el topic especificado.
        Args:
            topic (str): Topic donde se publicará el mensaje.
            payload (str): Contenido del mensaje a publicar.
        """
        self.client.publish(topic, payload)
    
    def disconnect(self):
        """
        Desconecta el cliente MQTT y detiene el bucle de eventos.
        """
        self.client.loop_stop()
        self.client.disconnect()

"""
    Clase para manejar la persistencia de datos en archivos JSON.
    Esta clase se encarga de la creación, actualización y eliminación
    de dispositivos, reglas y eventos. Utiliza archivos JSON para almacenar
    la información de manera persistente. Permite la lectura y escritura
    de dispositivos, reglas y eventos, así como la inicialización de archivos
    vacíos o inválidos.
"""

class Persistence:
    def __init__(self, devices_file="config/devices.json", rules_file="config/rules.json", events_file="config/events.json"):
        """
        Inicializa la clase de persistencia con los nombres de archivo para dispositivos,
        reglas y eventos. Si los archivos no existen o son inválidos,
        se crean archivos vacíos.
        Args:
            devices_file (str): Ruta al archivo JSON para dispositivos.
            rules_file (str): Ruta al archivo JSON para reglas.
            events_file (str): Ruta al archivo JSON para eventos.
        """
        self.devices_file = devices_file
        self.rules_file = rules_file
        self.events_file = events_file
        self.logger = logging.getLogger('controller.Persistence')
        self.logger.info(f"Persistence initialized with files: {devices_file}, {rules_file}, {events_file}")
        self._init_files()
    
    def _init_files(self):
        """
        Inicializa los archivos de dispositivos, reglas y eventos.
        Si los archivos no existen o son inválidos, se crean archivos vacíos.
        """
        for file in [self.devices_file, self.rules_file, self.events_file]:
            try:
                if not os.path.exists(file):
                    os.makedirs(os.path.dirname(file), exist_ok=True)
                    with open(file, 'w') as f:
                        json.dump([], f)
                    self.logger.info(f"File created: {file}")
                else:
                    with open(file, 'r') as f:
                        content = f.read().strip()
                        if not content:
                            with open(file, 'w') as fw:
                                json.dump([], fw)
                            self.logger.info(f"Empty file initialized: {file}")
                        else:
                            json.loads(content)
                            self.logger.debug(f"File validated: {file}")
            except (json.JSONDecodeError, IOError) as e:
                self.logger.error(f"Error initializing {file}: {e}. Resetting to empty list.")
                with open(file, 'w') as f:
                    json.dump([], f)
    
    def add_device(self, device_id, device_type):
        """
        Añade un nuevo dispositivo a la lista de dispositivos.
        Args:
            device_id (str): ID del dispositivo a añadir.
            device_type (str): Tipo del dispositivo a añadir.
            
        Returns:
            bool: True si el dispositivo fue añadido, False si ya existe.
        """
        devices = self.get_devices()
        if any(d['id'] == device_id for d in devices):
            self.logger.warning(f"Attempt to add existing device: {device_id}")
            return False
        devices.append({"id": device_id, "type": device_type, "state": None})
        with open(self.devices_file, 'w') as f:
            json.dump(devices, f)
        self.logger.info(f"Device added: {device_id} of type {device_type}")
        return True
    
    def update_device_state(self, device_id, state):
        """
        Actualiza el estado de un dispositivo existente.
        
        Args:
            device_id (str): ID del dispositivo a actualizar.
            state (str): Nuevo estado del dispositivo.
            
        Returns:
            bool: True si el estado fue actualizado, False si el dispositivo no existe.
        """
        devices = self.get_devices()
        for device in devices:
            if device['id'] == device_id:
                device['state'] = state
                with open(self.devices_file, 'w') as f:
                    json.dump(devices, f)
                self.logger.debug(f"State updated for {device_id}: {state}")
                return True
        self.logger.warning(f"Attempt to update non-existing device: {device_id}")
        return False
    
    def get_device(self, device_id):
        """
        Obtiene la información de un dispositivo específico.
        Args:
            device_id (str): ID del dispositivo a obtener.
            
        Returns:
            dict: Información del dispositivo o None si no existe.
        """
        devices = self.get_devices()
        for device in devices:
            if device['id'] == device_id:
                return device
        error_msg = f"Device not found: {device_id}"
        self.logger.error(error_msg)
        raise ValueError(error_msg)
    
    def delete_device(self, device_id):
        """
        Elimina un dispositivo de la lista de dispositivos.
        Args:
            device_id (str): ID del dispositivo a eliminar.
        """
        devices = self.get_devices()
        original_count = len(devices)
        devices = [d for d in devices if d['id'] != device_id]
        
        if len(devices) < original_count:
            with open(self.devices_file, 'w') as f:
                json.dump(devices, f)
            self.logger.info(f"Device deleted: {device_id}")
            return True
        else:
            self.logger.warning(f"Attempt to delete non-existing device: {device_id}")
            return False
    
    def get_devices(self):
        """
        Obtiene la lista de todos los dispositivos registrados.
        Returns:
            list: Lista de diccionarios con la información de los dispositivos.
        """
        try:
            with open(self.devices_file, 'r') as f:
                content = f.read().strip()
                if not content:
                    return []
                devices = json.loads(content)
                self.logger.debug(f"Retrieved {len(devices)} devices")
                return devices
        except (json.JSONDecodeError, IOError) as e:
            self.logger.error(f"Error reading {self.devices_file}: {e}. Returning empty list.")
            return []
    
    def add_rule(self, condition, action):
        """
        Añade una nueva regla de automatización.
        Args:
            condition (str): Condición que debe cumplirse para ejecutar la acción.
            action (str): Acción a ejecutar   
        """
        rules = self.get_rules()
        rules.append({"condition": condition, "action": action})
        with open(self.rules_file, 'w') as f:
            json.dump(rules, f)
        self.logger.info(f"Rule added: {condition} -> {action}")
    
    def get_rules(self):
        """
        Obtiene la lista de todas las reglas de automatización.
        Returns:
            list: Lista de reglas.
        """
        try:
            with open(self.rules_file, 'r') as f:
                content = f.read().strip()
                if not content:
                    return []
                rules = json.loads(content)
                self.logger.debug(f"Retrieved {len(rules)} rules")
                return rules
        except (json.JSONDecodeError, IOError) as e:
            self.logger.error(f"Error reading {self.rules_file}: {e}. Returning empty list.")
            return []
    
    def log_event(self, device_id, state):
        """
        Registra un evento en el historial de eventos.
        Args:
            device_id (str): ID del dispositivo que disparó el evento.
            state (str): Nuevo estado del dispositivo.
        """
        events = self.get_events()
        timestamp = datetime.now().isoformat()
        events.append({
            "device_id": device_id, 
            "state": state, 
            "timestamp": timestamp
        })
        with open(self.events_file, 'w') as f:
            json.dump(events, f)
        self.logger.info(f"Event logged: {device_id} -> {state} at {timestamp}")
    
    def get_events(self):
        """
        Obtiene el historial de eventos.
        Returns:
            list: Lista de eventos.
        """
        try:
            with open(self.events_file, 'r') as f:
                content = f.read().strip()
                if not content:
                    return []
                events = json.loads(content)
                self.logger.debug(f"Retrieved {len(events)} events")
                return events
        except (json.JSONDecodeError, IOError) as e:
            self.logger.error(f"Error reading {self.events_file}: {e}. Returning empty list.")
            return []
