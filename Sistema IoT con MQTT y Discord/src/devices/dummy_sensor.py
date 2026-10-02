import argparse
import time
from src.utils import MQTTClient
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))


"""
    Simulador de sensor que genera valores fluctuantes y los publica mediante MQTT.
    
    Esta clase simula un sensor que genera valores que oscilan entre un mínimo y un máximo
    definido, cambiando de dirección al alcanzar los límites. Los valores se publican
    periódicamente en un topic MQTT.
    
    Hereda de MQTTClient para manejar la conexión y comunicación MQTT.
    
    Attributes:
        device_id (str): Identificador único del dispositivo sensor.
        min_val (int): Valor mínimo que puede alcanzar el sensor.
        max_val (int): Valor máximo que puede alcanzar el sensor.
        increment (int): Incremento aplicado en cada actualización.
        interval (int): Intervalo de tiempo en segundos entre actualizaciones.
        current (int): Valor actual del sensor.
        direction (int): Dirección del cambio (1: ascendente, -1: descendente).
        topic (str): Topic MQTT donde se publicarán los valores del sensor.
"""

class DummySensor(MQTTClient):

    def __init__(self, config_path, device_id, min_val, max_val, increment, interval):
        
        """
            Inicializa el sensor simulado.
            
            Args:
                config_path (str): Ruta al archivo de configuración MQTT.
                device_id (str): Identificador único del dispositivo.
                min_val (int): Valor mínimo que puede generar el sensor.
                max_val (int): Valor máximo que puede generar el sensor.
                increment (int): Cantidad a incrementar/decrementar en cada paso.
                interval (int): Tiempo en segundos entre cada actualización.
        """
        
        super().__init__(config_path)
        self.device_id = device_id
        self.min_val = min_val
        self.max_val = max_val
        self.increment = increment
        self.interval = interval
        self.current = min_val
        self.direction = 1
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
        self.publish(self.topic, str(self.current))

    def run(self):
        
        """
            Ejecuta el bucle principal del sensor simulado.
            
            Publica continuamente el valor actual del sensor en el topic MQTT configurado
            y actualiza el valor según los parámetros de incremento y dirección.
            El valor oscila entre los límites mínimo y máximo, cambiando de dirección
            cuando alcanza cualquiera de estos límites.
            
            Returns:
                None: Este método no retorna valores, se ejecuta hasta que se interrumpe.
                
            Raises:
                KeyboardInterrupt: Se captura en el bloque principal para desconectar correctamente.
        """
        
        while True:
            self.publish(self.topic, str(self.current))
            self.current += self.direction * self.increment
            if self.current >= self.max_val:
                self.direction = -1
            elif self.current <= self.min_val:
                self.direction = 1
            time.sleep(self.interval)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()  
    parser.add_argument('--host', default='localhost')
    parser.add_argument('-p', '--port', type=int, default=1883)
    parser.add_argument('-m', '--min', type=int, default=20)
    parser.add_argument('-M', '--max', type=int, default=30)
    parser.add_argument('--increment', type=int, default=1)
    parser.add_argument('-i', '--interval', type=int, default=1)
    parser.add_argument('id')
    args = parser.parse_args()
    
    sensor = DummySensor("config/config.yaml", args.id, args.min, args.max, args.increment, args.interval)
    sensor.connect(args.host, args.port)
    try:
        sensor.run()
    except KeyboardInterrupt:
        sensor.disconnect()