import pika
import time
import random
from threading import Thread
import os
import sys
sys.path.append(os.path.dirname(__file__))
from config import RABBITMQ_HOST, P_ALMACEN

# Configuración
QUEUE_NAME = 'robot_tasks'              # Cola para las peticiones de robot
RESPONSE_QUEUE = 'robot_responses'      # Cola para las respuestas de robot

class Robot:
    def __init__(self, robot_id):
        self.robot_id = robot_id    # ID único del repartidor
        self.connection = None      # Conexión con RabbitMQ
        self.channel = None         # Canal de comunicación con RabbitMQ

    def connect(self):
        """Establece conexión con RabbitMQ y configura colas."""
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(host=RABBITMQ_HOST))
        self.channel = self.connection.channel()
        self.channel.queue_declare(queue=QUEUE_NAME, durable=False, auto_delete=True)
        self.channel.queue_declare(queue=RESPONSE_QUEUE, durable=False, auto_delete=True)

    def callback(self, ch, method, properties, body):
        """
        Procesa los mensajes recibidos de la cola de robots.

        Args:
            ch: Canal de comunicación de RabbitMQ.
            method: Información sobre el mensaje.
            properties: Propiedades del mensaje.
            body: Cuerpo del mensaje recibido.

        Ret:
            None
        """
        mensaje = body.decode()
        print(f"Robot {self.robot_id} recibió: {mensaje}")

        # Procesar solo mensajes de trabajo (Move)
        if not mensaje.startswith("Move "):
            print(f"Robot {self.robot_id} recibió mensaje inesperado: {mensaje}")
            ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)
            return

        try:
            _, pedido_id, producto = mensaje.split(" ", 2)

            # Simular trabajo (espera aleatoria entre 5 y 10 segundos)
            tiempo_trabajo = random.uniform(5, 10)
            time.sleep(tiempo_trabajo)

            # Determinar si se encuentra el producto
            exito = random.random() < P_ALMACEN

            if exito:
                # Respuesta exitosa
                respuesta = f"MOVED {pedido_id} {producto}"
            else:
                # Respuesta de fallo
                respuesta = f"NOT_FOUND {pedido_id} {producto}"

            # Enviar respuesta a la cola de respuestas
            self.channel.basic_publish(
                exchange='',
                routing_key=RESPONSE_QUEUE,
                body=respuesta.encode()
            )

            # Confirmar que el mensaje fue procesado
            ch.basic_ack(delivery_tag=method.delivery_tag)

        except Exception as e:
            print(f"Robot {self.robot_id} error al procesar mensaje: {e}")
            ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)

    def start(self):
        """Inicia el robot y comienza a consumir mensajes."""
        self.connect()
        self.channel.basic_consume(queue=QUEUE_NAME, on_message_callback=self.callback)
        print(f"Robot {self.robot_id} iniciado y escuchando en la cola {QUEUE_NAME}")
        self.channel.start_consuming()

    def run(self):
        """Ejecuta el robot en un hilo separado."""
        thread = Thread(target=self.start)
        thread.daemon = True
        thread.start()
        return thread