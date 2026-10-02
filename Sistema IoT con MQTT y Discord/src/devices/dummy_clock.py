import argparse
import time
from datetime import datetime, timedelta
from src.utils import MQTTClient
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))


"""
    Simulador de reloj virtual que publica marcas de tiempo a través de MQTT.
    
    Esta clase crea un reloj simulado que publica marcas de tiempo en un topic MQTT
    con una frecuencia configurable. Permite simular el paso del tiempo a diferentes
    velocidades para pruebas y simulaciones.
    
    Hereda de MQTTClient para manejar la conexión y comunicación MQTT.
    
    Attributes:
        device_id (str): Identificador único del dispositivo reloj.
        current_time (datetime): La hora actual del reloj simulado.
        increment (int): Incremento en segundos entre cada actualización de tiempo.
        rate (int): Velocidad de actualización (mensajes por segundo).
        topic (str): Topic MQTT donde se publicarán los mensajes de tiempo.
"""
class DummyClock(MQTTClient):
    
    """
        Inicializa el reloj simulado.
        Args:
            config_path (str): Ruta al archivo de configuración MQTT.
            device_id (str): Identificador único del dispositivo.
            start_time (str): Hora inicial en formato "HH:MM:SS".
            increment (int): Incremento en segundos entre cada actualización de tiempo.
            rate (int): Velocidad de actualización (mensajes por segundo).
    """

    def __init__(self, config_path, device_id, start_time, increment, rate):
        

        super().__init__(config_path)
        self.device_id = device_id
        self.current_time = datetime.strptime(start_time, "%H:%M:%S") if start_time else datetime.now()
        self.increment = increment
        self.rate = rate
        self.topic = f"{self.base_topic}/{device_id}"

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
        self.publish(self.topic, self.current_time.strftime("%H:%M:%S"))
   
    def run(self):
        
        """
            Ejecuta el bucle principal del reloj simulado.
            
            Publica continuamente la hora actual en el topic MQTT configurado
            y actualiza la hora según los parámetros de incremento y velocidad.
            Este método se ejecuta indefinidamente hasta que se interrumpa.
            
            Returns:
                None: Este método no retorna valores, se ejecuta hasta que se interrumpe.
                
            Raises:
                KeyboardInterrupt: Se captura en el bloque principal para desconectar correctamente.
        """ 
        
        while True:
            self.publish(self.topic, self.current_time.strftime("%H:%M:%S"))
            self.current_time += timedelta(seconds=self.increment)
            time.sleep(5 / self.rate)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--host', default='localhost')
    parser.add_argument('-p', '--port', type=int, default=1883)
    parser.add_argument('--time', type=str)
    parser.add_argument('--increment', type=int, default=1)
    parser.add_argument('--rate', type=int, default=1)
    parser.add_argument('id')
    args = parser.parse_args()
    
    clock = DummyClock("config/config.yaml", args.id, args.time, args.increment, args.rate)
    clock.connect(args.host, args.port)
    try:
        clock.run()
    except KeyboardInterrupt:
        clock.disconnect()