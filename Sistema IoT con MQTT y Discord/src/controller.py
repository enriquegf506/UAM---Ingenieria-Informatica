import re
import requests
import argparse
import logging
from flask import Flask, request
from utils import MQTTClient, Persistence
from rule_engine import RuleEngine
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

def setup_logging(log_file='controller.log'):
    """
    Configura el sistema de logging para la aplicación.
    
    Args:
        log_file (str): Ruta al archivo de logs.
    
    Returns:
        logging.Logger: Logger configurado.
    """
    logger = logging.getLogger('controller')
    logger.setLevel(logging.INFO)
    
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(logging.INFO)
    
    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.WARNING)
    
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    file_handler.setFormatter(formatter)
    console_handler.setFormatter(formatter)
    
    logger.addHandler(file_handler)
    logger.addHandler(console_handler)
    
    return logger

app = Flask(__name__)

"""
    Controlador principal para gestionar dispositivos IoT a través de MQTT.
    
    Esta clase extiende MQTTClient para manejar la comunicación con dispositivos,
    gestionar reglas de automatización y mantener la persistencia de estados y eventos.
    
    Attributes:
        persistence (Persistence): Gestor de persistencia para almacenar estados y eventos.
        api_url (str): URL de la API de notificaciones para enviar eventos.
        rule_engine (RuleEngine): Motor de reglas para evaluar condiciones y ejecutar acciones.
"""

class Controller(MQTTClient):
    
    def __init__(self, config_path):
        """
        Inicializa el controlador con la configuración especificada.
        
        Args:
            config_path (str): Ruta al archivo de configuración YAML.
        """
        super().__init__(config_path)
        self.persistence = Persistence()
        self.api_url = "http://localhost:5000"
        self.rule_engine = RuleEngine(self.base_topic, self.persistence, self.api_url)
        self.logger = logging.getLogger('controller.Controller')
    
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
        self.logger.info(f"Connected with result code {rc}")
        self.client.subscribe(f"{self.base_topic}/+")

    def on_message(self, client, userdata, msg):
        """
        Callback que se ejecuta cuando se recibe un mensaje MQTT.
        
        Procesa los mensajes recibidos, actualiza el estado de los dispositivos,
        registra eventos y evalúa las reglas de automatización.
        
        Args:
            client: Cliente MQTT que recibió el mensaje.
            userdata: Datos de usuario proporcionados por el cliente.
            msg: Objeto de mensaje MQTT que contiene topic y payload.
            
        Returns:
            None
        """
        topic_parts = msg.topic.split("/")
        if len(topic_parts) < 4:
            self.logger.warning(f"Invalid topic format: {msg.topic}")
            return
        device_id = topic_parts[-2] if msg.topic.endswith("/set") else topic_parts[-1]
        try:
            state = msg.payload.decode()
            devices = self.persistence.get_devices()
            full_device_id = f"{self.base_topic}/{device_id}"
            if not any(d['id'] == full_device_id for d in devices):
                self.logger.warning(f"Ignoring message for unregistered device: {full_device_id}")
                return
            if self.persistence.update_device_state(full_device_id, state):
                self.logger.info(f"Updated state for device {full_device_id} to {state}")
                self.persistence.log_event(full_device_id, state)
                self.rule_engine.evaluate_rules(full_device_id, state, self.publish)
                requests.post(f"{self.api_url}/event", json={
                    "device_id": full_device_id,
                    "state": state
                })
            else:
                self.logger.error(f"Failed to update state for device: {full_device_id}")
        except ValueError as e:
            self.logger.error(f"Error processing message for {device_id}: {e}")
            

    def add_device(self, device_id, device_type):
        """
        Añade un nuevo dispositivo al sistema.
        
        Args:
            device_id (str): Identificador único del dispositivo.
            device_type (str): Tipo de dispositivo (switch, sensor, clock, etc.).
            
        Returns:
            bool: True si el dispositivo se añadió correctamente, False en caso contrario.
        """
        result = self.persistence.add_device(device_id, device_type)
        if result:
            self.logger.info(f"Added new device: {device_id} of type {device_type}")
        else:
            self.logger.warning(f"Failed to add device: {device_id}")
        return result
    

    def delete_device(self, device_id):
        """
        Elimina un dispositivo existente del sistema.
        
        Args:
            device_id (str): Identificador único del dispositivo a eliminar.
            
        Returns:
            bool: True si el dispositivo fue eliminado, False si no existía.
        """
        result = self.persistence.delete_device(device_id)
        if result:
            self.logger.info(f"Deleted device: {device_id}")
        else:
            self.logger.warning(f"Attempted to delete non-existent device: {device_id}")
        return result

    

    def add_rule(self, condition, action):
        """
        Añade una nueva regla de automatización.
        
        Args:
            condition (str): Condición que debe cumplirse para ejecutar la acción.
            action (str): Acción a ejecutar cuando se cumpla la condición.
            
        Returns:
            None
        """
        self.rule_engine.add_rule(condition, action)
        self.logger.info(f"Added new rule with condition '{condition}' and action '{action}'")
    

    def get_devices(self):
        """
        Obtiene la lista de todos los dispositivos registrados.
        
        Returns:
            list: Lista de diccionarios con información de los dispositivos.
        """
        devices = self.persistence.get_devices()
        self.logger.debug(f"Retrieved {len(devices)} devices")
        return devices
    

    def get_rules(self):
        """
        Obtiene la lista de todas las reglas de automatización.
        
        Returns:
            list: Lista de diccionarios con información de las reglas.
        """
        rules = self.rule_engine.get_rules()
        self.logger.debug(f"Retrieved {len(rules)} rules")
        return rules
    

    def get_events(self):
        """
        Obtiene el historial de eventos registrados.
        
        Returns:
            list: Lista de diccionarios con información de los eventos.
        """
        events = self.persistence.get_events()
        self.logger.debug(f"Retrieved {len(events)} events")
        return events

controller = Controller("config/config.yaml")


@app.route("/add_device", methods=["POST"])
def add_device():
    """
    Endpoint para añadir un nuevo dispositivo.
    """
    data = request.json
    device_id = data.get("device_id")
    device_type = data.get("device_type")
    
    logger.info(f"API request to add device: {device_id}")
    if controller.add_device(device_id, device_type):
        return "", 200
    else:
        return {"error": f"Device with ID '{device_id}' already exists or couldn't be added."}, 400



@app.route("/delete_device", methods=["POST"])
def delete_device():
    """
    Endpoint para eliminar un dispositivo existente.
    
    Recibe un JSON con device_id y elimina el dispositivo del sistema.
    
    Returns:
        tuple: Respuesta vacía con código 200 (OK).
    """
    data = request.json
    device_id = data.get("device_id")

    logger.info(f"Request to delete device: {device_id}")
    if controller.delete_device(device_id):
        return "", 200
    else:
        return {"error": f"Device with ID '{device_id}' doesn't exists or couldn't be deleted."}, 404


@app.route("/set_switch", methods=["POST"])
def set_switch():
    """
    Endpoint para cambiar el estado de un interruptor.
    
    Recibe un JSON con device_id y state y publica un mensaje MQTT para cambiar el estado.
    
    Returns:
        tuple: Respuesta vacía con código 200 (OK).
    """
    data = request.json
    logger.info(f"API request to set switch {data['device_id']} to state {data['state']}")
    controller.publish(f"{data['device_id']}/set", data["state"])
    return "", 200


@app.route("/add_rule", methods=["POST"])
def add_rule():
    """
    Endpoint para añadir una nueva regla de automatización.
    
    Recibe un JSON con condition y action y registra la regla en el sistema.
    
    Returns:
        tuple: Respuesta vacía con código 200 (OK).
    """
    data = request.json
    logger.info(f"API request to add rule: {data['condition']} -> {data['action']}")
    controller.add_rule(data["condition"], data["action"])
    return "", 200


@app.route("/devices", methods=["GET"])
def get_devices():
    """
    Endpoint para obtener la lista de dispositivos.
    
    Returns:
        tuple: Lista de dispositivos en formato JSON y código 200 (OK).
    """
    logger.info("API request to get devices")
    return controller.get_devices(), 200


@app.route("/rules", methods=["GET"])
def get_rules():
    """
    Endpoint para obtener la lista de reglas de automatización.
    
    Returns:
        tuple: Lista de reglas en formato JSON y código 200 (OK).
    """
    logger.info("API request to get rules")
    return controller.get_rules(), 200


@app.route("/events", methods=["GET"])
def get_events():
    """
    Endpoint para obtener el historial de eventos registrados.
    
    Returns:
        tuple: Lista de eventos en formato JSON y código 200 (OK).   
    """
    logger.info("API request to get events")
    return controller.get_events(), 200    



def main():
    """
    Función principal para iniciar el controlador y la API Flask.
    
    Configura el controlador MQTT y la API Flask, y establece la conexión
    con el broker MQTT.
    """
    parser = argparse.ArgumentParser(description="Controlador MQTT para dispositivos IoT")
    parser.add_argument('--host', type=str, default='localhost', help='Dirección del broker MQTT')
    parser.add_argument('--port', type=int, default=1883, help='Puerto del broker MQTT')
    args = parser.parse_args()

    global logger
    logger = setup_logging()
    logger.info("Starting IoT Controller")

    controller.connect(args.host, args.port)
    logger.info(f"Connected to MQTT broker at {args.host}:{args.port}")
    
    logger.info("Starting Flask API server on port 8081")
    app.run(port=8081)

if __name__ == "__main__":
    main()