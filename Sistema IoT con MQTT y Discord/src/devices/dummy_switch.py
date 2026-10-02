import argparse
import random
from src.utils import MQTTClient
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

"""
    Simulador de interruptor virtual controlable a través de MQTT.
    
    Esta clase simula un interruptor que puede estar en estado "ON" u "OFF", 
    y que puede ser controlado mediante comandos MQTT. El interruptor tiene 
    una probabilidad configurable de fallar al procesar un comando.
    
    Hereda de MQTTClient para manejar la conexión y comunicación MQTT.
    
    Attributes:
        device_id (str): Identificador único del dispositivo interruptor.
        probability (float): Probabilidad de fallo al procesar un comando (0-1).
        state (str): Estado actual del interruptor ("ON" u "OFF").
        topic (str): Topic MQTT donde se publica el estado actual.
        command_topic (str): Topic MQTT donde se reciben los comandos.
"""

class DummySwitch(MQTTClient):
    
    def __init__(self, config_path, device_id, probability):
        """
            Inicializa el interruptor simulado.
        
            Args:
                config_path (str): Ruta al archivo de configuración MQTT.
                device_id (str): Identificador único del dispositivo.
                probability (float): Probabilidad de fallo al procesar un comando (0-1).
        """
        super().__init__(config_path)
        self.device_id = device_id
        self.probability = probability
        self.state = "OFF"
        self.topic = f"{self.base_topic}/{device_id}"
        self.command_topic = f"{self.topic}/set"
    

    def on_connect(self, client, userdata, flags, rc):
        
        """
        Callback que se ejecuta cuando se establece conexión con el broker MQTT.
        
        Se suscribe al topic de comandos y publica el estado inicial.
        
        Args:
            client: Cliente MQTT que llamó al callback.
            userdata: Datos de usuario proporcionados por el cliente.
            flags: Indicadores proporcionados por el broker.
            rc (int): Código de resultado de la conexión.
        """
        self.client.subscribe(self.command_topic)
        self.publish(self.topic, self.state)

    def on_message(self, client, userdata, msg):
        """
        Callback que se ejecuta cuando se recibe un mensaje en un topic suscrito.
        
        Procesa los comandos recibidos en el command_topic y actualiza el estado
        del interruptor si el comando es válido y no hay fallo según la 
        probabilidad configurada.
        
        Args:
            client: Cliente MQTT que llamó al callback.
            userdata: Datos de usuario proporcionados por el cliente.
            msg: Objeto mensaje MQTT recibido, contiene topic y payload.
        """
        if msg.topic == self.command_topic:
            payload = msg.payload.decode()
            if payload in ["ON", "OFF"]:
                if random.random() > self.probability:
                    self.state = payload
                    self.publish(self.topic, self.state)
                else:
                    self.publish(self.topic, self.state)
                    print(f"Action failed for {self.device_id}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--host', default='localhost')
    parser.add_argument('-p','--port', type=int, default=1883)
    parser.add_argument('-P', '--probability', type=float, default=0.3)
    parser.add_argument('id')
    args = parser.parse_args()
    
    switch = DummySwitch("config/config.yaml", args.id, args.probability)
    switch.connect(args.host, args.port)
    try:
        while True:
            pass
    except KeyboardInterrupt:
        switch.disconnect()