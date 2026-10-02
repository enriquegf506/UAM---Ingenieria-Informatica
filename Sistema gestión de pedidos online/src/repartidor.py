import pika
import time
import random
from threading import Thread
import os
import sys
sys.path.append(os.path.dirname(__file__))
from config import P_ENTREGA, RABBITMQ_HOST

# Configuración
DELIVERY_QUEUE = 'delivery_tasks'       # Cola para las peticiones de entrega
RESPONSE_QUEUE = 'delivery_responses'   # Cola para las respuestas de entrega

class Repartidor:
    def __init__(self, repartidor_id):
        self.repartidor_id = repartidor_id  # ID único del repartidor
        self.connection = None              # Conexión con RabbitMQ
        self.channel = None                 # Canal de comunicación con RabbitMQ

    def connect(self):
        """
        Establece la conexión con RabbitMQ y configura las colas necesarias.
        """ 
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(host=RABBITMQ_HOST))
        self.channel = self.connection.channel()
        self.channel.queue_declare(queue=DELIVERY_QUEUE, durable=False, auto_delete=True)
        self.channel.queue_declare(queue=RESPONSE_QUEUE, durable=False, auto_delete=True)

    def callback(self, ch, method, properties, body):
        """
        Procesa los mensajes recibidos de la cola de entregas.

        Args:
            ch: Canal de comunicación de RabbitMQ.
            method: Información sobre el mensaje.
            properties: Propiedades del mensaje.
            body: Cuerpo del mensaje recibido.

        Ret:
            None
        """
        mensaje = body.decode()
        print(f"Repartidor {self.repartidor_id} recibió: {mensaje}")

        # Procesar solo mensajes de entrega (DISTRIBUTE)
        if not mensaje.startswith("DISTRIBUTE "):
            print(f"Repartidor {self.repartidor_id} recibió mensaje inesperado: {mensaje}")
            ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)
            return

        try:
            _, pedido_id = mensaje.split(" ", 1)

            # Enviar mensaje DISTRIBUTING
            mensaje_distribuyendo = f"DISTRIBUTING {pedido_id}"
            self.channel.basic_publish(
                exchange='',
                routing_key=RESPONSE_QUEUE,
                body=mensaje_distribuyendo.encode()
            )

            # Intentar entrega hasta 3 veces
            for intento in range(1, 4):
                # Simular tiempo de entrega (espera aleatoria entre 10 y 20 segundos)
                tiempo_entrega = random.uniform(10, 20)
                time.sleep(tiempo_entrega)

                # Determinar si la entrega es exitosa
                exito = random.random() < P_ENTREGA

                if exito:
                    # Respuesta exitosa
                    respuesta = f"DISTRIBUTED {pedido_id}"
                    self.channel.basic_publish(
                        exchange='',
                        routing_key=RESPONSE_QUEUE,
                        body=respuesta.encode()
                    )
                    ch.basic_ack(delivery_tag=method.delivery_tag)
                    return

                else:
                    # Respuesta de fallo en el intento
                    respuesta = f"DELIVERY_FAILED {pedido_id} Intento {intento}"
                    self.channel.basic_publish(
                        exchange='',
                        routing_key=RESPONSE_QUEUE,
                        body=respuesta.encode()
                    )

            # Si los 3 intentos fallan, enviar mensaje de fallo final
            respuesta = f"DELIVERY_FAILED_FINAL {pedido_id}"
            self.channel.basic_publish(
                exchange='',
                routing_key=RESPONSE_QUEUE,
                body=respuesta.encode()
            )

            # Confirmar que el mensaje fue procesado
            ch.basic_ack(delivery_tag=method.delivery_tag)

        except Exception as e:
            print(f"Repartidor {self.repartidor_id} error al procesar mensaje: {e}")
            ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)

    def start(self):
        """Inicia el repartidor y comienza a consumir mensajes."""
        self.connect()
        self.channel.basic_consume(queue=DELIVERY_QUEUE, on_message_callback=self.callback)
        print(f"Repartidor {self.repartidor_id} iniciado y escuchando en la cola {DELIVERY_QUEUE}")
        self.channel.start_consuming()

    def run(self):
        """Ejecuta el repartidor en un hilo separado."""
        thread = Thread(target=self.start)
        thread.daemon = True
        thread.start()
        return thread