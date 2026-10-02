#!/usr/bin/env python
import pika
import json
import os
import sys
import datetime
import uuid
import logging
from logging.handlers import RotatingFileHandler
sys.path.append(os.path.dirname(__file__))
from config import ARCHIVO_CLIENTES, ARCHIVO_PEDIDOS, RABBITMQ_HOST, RABBITMQ_PASS, RABBITMQ_PORT, RABBITMQ_USER, RABBITMQ_VHOST


class Controlador:
    #define los posibles estados de un pedido durante su ciclo de vida
    ESTADOS_PEDIDO = ["recibido", "en_almacen", "en_cinta", "en_reparto", "entregado", "cancelado"]
    # define los estados en los que un pedido puede ser cancelado
    ESTADOS_CANCELABLES = ["recibido", "en_almacen", "en_cinta"]

    def __init__(self):
        self.conexion = None                        # conexión a RabbitMQ
        self.canal = None                           # canal de comunicación
        self.logger = self.configurar_logger()      # logger con el registro de los eventos

    def configurar_logger(self):
        """
        Configura el sistema de logging con rotación de archivos para registrar eventos del controlador.

        Ret:
            Logger configurado para registrar los eventos del sistema.
        """
        if not os.path.exists('logs'):
            os.makedirs('logs')
            
        logger = logging.getLogger('controlador')
        logger.setLevel(logging.INFO)
        
        handler = RotatingFileHandler(
            'logs/controlador.log',
            maxBytes=10*1024*1024,  
            backupCount=5
        )
        
        formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
        handler.setFormatter(formatter)
        
        logger.addHandler(handler)
        
        return logger


    def cargar_clientes(self):
        """
        Carga desde el archivo la información de los clientes registrados.

        Ret:
            Diccionario con los datos de los clientes.
        """
        if os.path.exists(ARCHIVO_CLIENTES):
            with open(ARCHIVO_CLIENTES, 'r') as archivo:
                try:
                    return json.load(archivo)
                except json.JSONDecodeError:
                    return {}
        return {}

    def guardar_clientes(self, clientes):
        """
        Guarda en archivo los datos actualizados de los clientes.

        Args:
            clientes: Diccionario con la información de los clientes a guardar.

        Ret:
            No retorna ningún valor.
        """
        with open(ARCHIVO_CLIENTES, 'w') as archivo:
            json.dump(clientes, archivo, indent=4)
        self.logger.info(f"Clientes guardados en {ARCHIVO_CLIENTES}")

    def cargar_pedidos(self):
        """
        Carga desde disco los pedidos existentes si el archivo está disponible.

        Ret:
            Diccionario con los datos de los pedidos.
        """
        if os.path.exists(ARCHIVO_PEDIDOS):
            with open(ARCHIVO_PEDIDOS, 'r') as archivo:
                try:
                    return json.load(archivo)
                except json.JSONDecodeError:
                    return {}
        return {}

    def guardar_pedidos(self, pedidos):
        """
        Almacena en disco el estado actual de los pedidos.

        Args:
            pedidos: Diccionario con la información de los pedidos a guardar.

        Ret:
            No retorna ningún valor.
        """
        with open(ARCHIVO_PEDIDOS, 'w') as archivo:
            json.dump(pedidos, archivo, indent=4)
        self.logger.info(f"Pedidos guardados en {ARCHIVO_PEDIDOS}")

    def conectar_rabbitmq(self):
        """
        Establece la conexión con el servidor RabbitMQ utilizando las credenciales de configuración.

        Ret:
            True si la conexión salió bien, False en caso contrario.
        """
        try:
            credentials = pika.PlainCredentials(RABBITMQ_USER, RABBITMQ_PASS)
            parameters = pika.ConnectionParameters(RABBITMQ_HOST, RABBITMQ_PORT, RABBITMQ_VHOST, credentials)
            self.conexion = pika.BlockingConnection(parameters)
            self.canal = self.conexion.channel()
            return True
        except Exception as e:
            selg.logger.error(f"Error al conectar con RabbitMQ: {e}")
            return False

    def enviar_respuesta(self, props, respuesta):
        """
        Envía una respuesta al cliente a través de la cola indicada en 'reply_to'.

        Args:
            props: Propiedades del mensaje original, incluyendo reply_to y correlation_id.
            respuesta: Objeto serializable a JSON que será enviado como respuesta.

        Ret:
            No retorna ningún valor.
        """
        if props.reply_to:
            try:
                self.canal.basic_publish(
                    exchange='',
                    routing_key=props.reply_to,
                    properties=pika.BasicProperties(correlation_id=props.correlation_id),
                    body=json.dumps(respuesta).encode()
                )
                self.logger.info(f"Respuesta enviada con correlation_id {props.correlation_id}: {respuesta}")
            except Exception as e:
                self.logger.error(f"Error al enviar respuesta: {e}")

    def procesar_respuesta_robot(self, ch, method, properties, body):
        """
        Procesa la respuesta recibida desde el robot tras la búsqueda de un producto.

        Args:
            ch: Canal de comunicación de RabbitMQ.
            method: Método que contiene la etiqueta de entrega.
            properties: Propiedades del mensaje recibido.
            body: Cuerpo del mensaje recibido (respuesta del robot).

        Ret:
            No retorna ningún valor. Confirma o rechaza la recepción del mensaje.
        """
        try:
            mensaje = body.decode()
            self.logger.info(f"Respuesta de robot recibida: {mensaje}")
            parts = mensaje.split(" ", 2)
            if len(parts) != 3:
                self.logger.error(f"Mensaje malformado: {mensaje}")
                ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)
                return

            accion, pedido_id, producto = parts
            pedidos = self.cargar_pedidos()
            if pedido_id not in pedidos:
                self.logger.warning(f"Pedido {pedido_id} no encontrado.")
                ch.basic_ack(delivery_tag=method.delivery_tag)
                return

            pedido = pedidos[pedido_id]
            if pedido["estado"] == "cancelado":
                self.logger.info(f"Pedido {pedido_id} ya está cancelado, ignorando respuesta.")
                ch.basic_ack(delivery_tag=method.delivery_tag)
                return

            if "productos_procesados" not in pedido:
                pedido["productos_procesados"] = {}

            if accion == "MOVED":
                pedido["productos_procesados"][producto] = "encontrado"
                self.logger.info(f"Producto {producto} del pedido {pedido_id} encontrado.")
            elif accion == "NOT_FOUND":
                pedido["productos_procesados"][producto] = "no_encontrado"
                self.logger.info(f"Producto {producto} del pedido {pedido_id} no encontrado.")

            total_productos = len(pedido["productos"])
            productos_procesados = len(pedido["productos_procesados"])
            productos_encontrados = sum(1 for estado in pedido["productos_procesados"].values() if estado == "encontrado")
            productos_no_encontrados = sum(1 for estado in pedido["productos_procesados"].values() if estado == "no_encontrado")

            if productos_no_encontrados > 0:
                pedido["estado"] = "cancelado"
                pedido["historial_estados"].append({
                    "estado": "cancelado",
                    "fecha": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                    "motivo": f"Producto {producto} no encontrado"
                })
                self.logger.info(f"Pedido {pedido_id} cancelado porque el producto {producto} no fue encontrado.")
            elif productos_encontrados == total_productos:
                pedido["estado"] = "en_cinta"
                pedido["historial_estados"].append({
                    "estado": "en_cinta",
                    "fecha": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                })
                self.logger.info(f"Todos los productos del pedido {pedido_id} encontrados. Pedido pasado a en_cinta.")
                # Enviar mensaje al repartidor
                mensaje_distribuir = f"DISTRIBUTE {pedido_id}"
                self.canal.basic_publish(
                    exchange='',
                    routing_key='delivery_tasks',
                    body=mensaje_distribuir.encode()
                )
                self.logger.info(f"Mensaje enviado a repartidor: {mensaje_distribuir}")
            else:
                self.logger.info(f"Pedido {pedido_id} sigue en_almacen. Productos procesados: {productos_procesados}/{total_productos}")

            self.guardar_pedidos(pedidos)
            ch.basic_ack(delivery_tag=method.delivery_tag)

        except Exception as e:
            self.logger.error(f"Error al procesar respuesta de robot: {e}")
            ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)

    def procesar_respuesta_repartidor(self, ch, method, properties, body):
        """
        Procesa la respuesta enviada por el repartidor con el estado final de un pedido.

        Args:
            ch: Canal de comunicación de RabbitMQ.
            method: Método con la etiqueta de entrega del mensaje.
            properties: Propiedades del mensaje recibido.
            body: Cuerpo del mensaje con los datos del estado del pedido.

        Ret:
            No retorna ningún valor. Confirma la recepción del mensaje.
        """
        try:
            mensaje = body.decode()
            self.logger.info(f"Respuesta de repartidor recibida: {mensaje}")
            parts = mensaje.split(" ", 1)
            if len(parts) < 2:
                self.logger.error(f"Mensaje malformado: {mensaje}")
                ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)
                return

            accion, resto = parts

            if accion == "DELIVERY_FAILED":
                pedido_id = resto.split(" Intento")[0]
            else:
                pedido_id = resto.split(" ")[0]

            pedidos = self.cargar_pedidos()
            if pedido_id not in pedidos:
                self.logger.warning(f"Pedido {pedido_id} no encontrado.")
                ch.basic_ack(delivery_tag=method.delivery_tag)
                return

            pedido = pedidos[pedido_id]
            if pedido["estado"] == "cancelado":
                self.logger.info(f"Pedido {pedido_id} ya está cancelado, ignorando respuesta.")
                ch.basic_ack(delivery_tag=method.delivery_tag)
                return

            if accion == "DISTRIBUTING":
                pedido["estado"] = "en_reparto"
                pedido["historial_estados"].append({
                    "estado": "en_reparto",
                    "fecha": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                })
                self.logger.info(f"Pedido {pedido_id} ahora en_reparto.")
            elif accion == "DISTRIBUTED":
                pedido["estado"] = "entregado"
                pedido["historial_estados"].append({
                    "estado": "entregado",
                    "fecha": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                })
                self.logger.info(f"Pedido {pedido_id} entregado exitosamente.")
            elif accion == "DELIVERY_FAILED_FINAL":
                pedido["estado"] = "cancelado"
                pedido["historial_estados"].append({
                    "estado": "cancelado",
                    "fecha": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                    "motivo": "Fallo en la entrega tras múltiples intentos"
                })
                self.logger.info(f"Pedido {pedido_id} cancelado por fallo en la entrega.")
            elif accion == "DELIVERY_FAILED":
                self.logger.info(f"Intento de entrega fallido para pedido {pedido_id}, esperando más intentos.")
            else:
                self.logger.error(f"Acción de repartidor desconocida: {accion}")
                ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)
                return

            self.guardar_pedidos(pedidos)
            ch.basic_ack(delivery_tag=method.delivery_tag)

        except Exception as e:
            self.logger.error(f"Error al procesar respuesta de repartidor: {e}")
            ch.basic_nack(delivery_tag=method.delivery_tag, requeue=False)

    def procesar_registro(self, datos, props):
        """
        Procesa una solicitud de registro de un nuevo cliente, validando la información y almacenándola.

        Args:
            datos: Diccionario con los datos del cliente, incluyendo 'cliente_username' y 'cliente_password'.
            props: Propiedades del mensaje, utilizado para mantener el contexto en RabbitMQ.

        Ret:
            Diccionario con claves 'exito', 'mensaje' y opcionalmente 'cliente_id' si el registro fue exitoso.
        """
        try:
            username = datos.get("cliente_username")
            password_hash = datos.get("cliente_password")
            if not username or not password_hash:
                return {"exito": False, "mensaje": "Faltan datos obligatorios para el registro"}

            self.logger.info(f"Recibida solicitud de registro para usuario: {username}")
            clientes = self.cargar_clientes()
            for _, datos_cliente in clientes.items():
                if datos_cliente.get("username") == username:
                    return {"exito": False, "mensaje": "El nombre de usuario ya está en uso"}

            id_cliente = str(uuid.uuid4())
            clientes[id_cliente] = {
                "username": username,
                "password": password_hash,
                "fecha_registro": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            }
            self.logger.info(f"Nuevo cliente registrado con ID: {id_cliente}")
            self.guardar_clientes(clientes)
            return {"exito": True, "mensaje": "Usuario registrado correctamente", "cliente_id": id_cliente}

        except Exception as e:
            self.logger.error(f"Error al procesar registro: {e}")
            return {"exito": False, "mensaje": f"Error al procesar registro: {str(e)}"}

    def procesar_login(self, datos, props):
        """
        Valida las credenciales proporcionadas por un cliente para iniciar sesión.

        Args:
            datos: Diccionario con los campos 'cliente_username' y 'cliente_password'.
            props: Propiedades del mensaje, utilizado para mantener el contexto en RabbitMQ.

        Ret:
            Diccionario con claves 'exito', 'mensaje' y en caso de éxito, 'cliente_id' y 'cliente_username'.
        """
        try:
            username = datos.get("cliente_username")
            password_hash = datos.get("cliente_password")
            if not username or not password_hash:
                return {"exito": False, "mensaje": "Faltan datos de autenticación"}

            self.logger.info(f"Recibida solicitud de login para usuario: {username}")
            clientes = self.cargar_clientes()
            for id_cliente, datos_cliente in clientes.items():
                if datos_cliente.get("username") == username and datos_cliente.get("password") == password_hash:
                    self.logger.info(f"Usuario {username} autenticado correctamente")
                    return {
                        "exito": True,
                        "mensaje": "Autenticación exitosa",
                        "cliente_id": id_cliente,
                        "cliente_username": username
                    }
            return {"exito": False, "mensaje": "Usuario o contraseña incorrectos"}

        except Exception as e:
            self.logger.error(f"Error al procesar login: {e}")
            return {"exito": False, "mensaje": f"Error en autenticación: {str(e)}"}

    def procesar_hacer_pedido(self, datos, props):
        """
        Registra un nuevo pedido para un cliente, lo guarda y envía instrucciones a los robots.

        Args:
            datos: Diccionario con 'cliente_id' y una lista de 'productos'.
            props: Propiedades del mensaje, utilizado para mantener el contexto en RabbitMQ.

        Ret:
            Diccionario con claves 'exito', 'mensaje' y 'pedido_id' si el pedido fue exitoso.
        """
        try:
            cliente_id = datos.get("cliente_id")
            productos = datos.get("productos", [])
            self.logger.info(f"Solicitud de pedido recibida de cliente {cliente_id}")

            if not productos:
                return {"exito": False, "mensaje": "El pedido debe contener al menos un producto"}

            clientes = self.cargar_clientes()
            if cliente_id not in clientes:
                return {"exito": False, "mensaje": "El cliente no está registrado."}

            pedidos = self.cargar_pedidos()
            pedido_id = str(uuid.uuid4())
            ahora = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

            pedidos[pedido_id] = {
                "cliente_id": cliente_id,
                "productos": productos,
                "fecha": ahora,
                "estado": self.ESTADOS_PEDIDO[0],
                "historial_estados": [{"estado": self.ESTADOS_PEDIDO[0], "fecha": ahora}]
            }
            self.guardar_pedidos(pedidos)

            for producto in productos:
                mensaje = f"Move {pedido_id} {producto}"
                self.canal.basic_publish(
                    exchange='',
                    routing_key='robot_tasks',
                    body=mensaje.encode()
                )
                self.logger.info(f"Mensaje enviado a robot: {mensaje}")

            pedidos[pedido_id]["estado"] = "en_almacen"
            pedidos[pedido_id]["historial_estados"].append({
                "estado": "en_almacen",
                "fecha": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            })
            self.guardar_pedidos(pedidos)

            return {
                "exito": True,
                "mensaje": f"Pedido recibido con ID: {pedido_id}. Contiene {len(productos)} productos.",
                "pedido_id": pedido_id
            }

        except Exception as e:
            self.logger.error(f"Error al procesar pedido: {e}")
            return {"exito": False, "mensaje": f"Error al procesar pedido: {str(e)}"}

    def procesar_ver_pedidos(self, datos, props):
        """
        Devuelve la lista de pedidos realizados por un cliente.

        Args:
            datos: Diccionario que contiene el 'cliente_id' del usuario.
            props: Propiedades del mensaje, utilizado para mantener el contexto en RabbitMQ.

        Ret:
            Diccionario con clave 'exito' y una lista de pedidos bajo la clave 'pedidos' si fue exitoso.
        """
        try:
            cliente_id = datos.get("cliente_id")
            self.logger.info(f"Solicitud para ver pedidos de cliente {cliente_id}")

            clientes = self.cargar_clientes()
            if cliente_id not in clientes:
                return {"exito": False, "mensaje": "El cliente no está registrado."}

            pedidos = self.cargar_pedidos()
            pedidos_cliente = [
                {"id": pid, "productos": p["productos"], "fecha": p["fecha"], "estado": p["estado"]}
                for pid, p in pedidos.items() if p["cliente_id"] == cliente_id
            ]
            return {"exito": True, "pedidos": pedidos_cliente}

        except Exception as e:
            self.logger.error(f"Error al consultar pedidos: {e}")
            return {"exito": False, "mensaje": f"Error al consultar pedidos: {str(e)}"}

    def procesar_cancelar_pedido(self, datos, props):
        """
        Intenta cancelar un pedido existente si cumple con las condiciones para ello.

        Args:
            datos: Diccionario con el 'cliente_id' y el 'pedido_id' del pedido a cancelar.
            props: Propiedades del mensaje, utilizado para mantener el contexto en RabbitMQ.

        Ret:
            Diccionario con claves 'exito' y 'mensaje' informando si se logró cancelar el pedido.
        """
        try:
            cliente_id = datos.get("cliente_id")
            pedido_id = datos.get("pedido_id")
            self.logger.info(f"Solicitud para cancelar pedido {pedido_id} de cliente {cliente_id}")

            pedidos = self.cargar_pedidos()
            if pedido_id not in pedidos:
                return {"exito": False, "mensaje": "El pedido no existe."}

            pedido = pedidos[pedido_id]
            if pedido["cliente_id"] != cliente_id:
                return {"exito": False, "mensaje": "El pedido no pertenece a este cliente."}

            if pedido["estado"] == "cancelado":
                return {"exito": False, "mensaje": "El pedido ya fue cancelado."}

            if pedido["estado"] not in self.ESTADOS_CANCELABLES:
                return {
                    "exito": False,
                    "mensaje": f"No se puede cancelar un pedido en estado '{pedido['estado']}'. Solo se pueden cancelar pedidos que estén en almacén."
                }

            pedido["estado"] = "cancelado"
            pedido["historial_estados"].append({
                "estado": "cancelado",
                "fecha": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            })
            self.guardar_pedidos(pedidos)
            return {"exito": True, "mensaje": "Pedido cancelado exitosamente. Los productos han vuelto al almacén."}

        except Exception as e:
            self.logger.error(f"Error al cancelar pedido: {e}")
            return {"exito": False, "mensaje": f"Error al cancelar pedido: {str(e)}"}

    def procesar_solicitud(self, ch, method, properties, body):
        """
        Gestiona solicitudes entrantes desde clientes, llamando a la función de la acción correspondiente.

        Args:
            ch: Canal de comunicación RabbitMQ.
            method: Información del método que entrega el mensaje.
            properties: Propiedades del mensaje, incluyendo 'reply_to' y 'correlation_id'.
            body: Cuerpo del mensaje que contiene los datos de la solicitud.

        Ret:
            No retorna valor. Envía la respuesta apropiada al cliente.
        """
        try:
            datos = json.loads(body.decode())
            accion = datos.get("accion")
            self.logger.info(f"Solicitud recibida - Acción: {accion}, Correlation ID: {properties.correlation_id}")

            if accion == "registrar":
                respuesta = self.procesar_registro(datos, properties)
            elif accion == "login":
                respuesta = self.procesar_login(datos, properties)
            elif accion == "hacer_pedido":
                respuesta = self.procesar_hacer_pedido(datos, properties)
            elif accion == "ver_pedidos":
                respuesta = self.procesar_ver_pedidos(datos, properties)
            elif accion == "cancelar_pedido":
                respuesta = self.procesar_cancelar_pedido(datos, properties)
            else:
                respuesta = {"exito": False, "mensaje": f"Acción no reconocida: {accion}"}

            self.enviar_respuesta(properties, respuesta)

        except Exception as e:
            self.logger.error(f"Error al procesar solicitud RPC: {e}")
            if hasattr(properties, 'reply_to') and properties.reply_to:
                self.enviar_respuesta(properties, {
                    "exito": False,
                    "mensaje": f"Error interno del servidor: {str(e)}"
                })
        finally:
            ch.basic_ack(delivery_tag=method.delivery_tag)

    def iniciar(self):
        print("Iniciando controlador...")
        if not self.conectar_rabbitmq():
            self.logger.error("No se pudo conectar a RabbitMQ. Asegúrese de que el servidor está en ejecución.")
            print("No se pudo conectar a RabbitMQ. Asegúrese de que el servidor está en ejecución.")
            return

        self.canal.queue_declare(queue="cola_rpc", durable=False, auto_delete=True)
        self.canal.queue_declare(queue="robot_tasks", durable=False, auto_delete=True)
        self.canal.queue_declare(queue="robot_responses", durable=False, auto_delete=True)
        self.canal.queue_declare(queue="delivery_tasks", durable=False, auto_delete=True)
        self.canal.queue_declare(queue="delivery_responses", durable=False, auto_delete=True)

        self.canal.basic_qos(prefetch_count=1)
        self.canal.basic_consume(queue="cola_rpc", on_message_callback=self.procesar_solicitud)
        self.canal.basic_consume(queue="robot_responses", on_message_callback=self.procesar_respuesta_robot)
        self.canal.basic_consume(queue="delivery_responses", on_message_callback=self.procesar_respuesta_repartidor)

        print("Controlador iniciado.")
        self.logger.info("Controlador iniciado.")
        
        try:
            self.canal.start_consuming()
        except KeyboardInterrupt:
            print("Controlador detenido.")
            self.logger.info("Controlador detenido.")
        finally:
            if self.conexion and self.conexion.is_open:
                self.conexion.close()

if __name__ == "__main__":
    controlador = Controlador()
    controlador.iniciar()
