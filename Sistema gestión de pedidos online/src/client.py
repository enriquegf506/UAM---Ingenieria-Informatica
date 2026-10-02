#!/usr/bin/env python
import pika
import json
import sys
import uuid
import time
import argparse
import os
sys.path.append(os.path.dirname(__file__))
from config import RABBITMQ_HOST, TIMEOUT

# Esta clase representa a un cliente del sistema de pedidos.
class Cliente:
    def __init__(self):
        self.cliente_id = None          # Id del cliente
        self.cliente_username = None    # Nombre de usuario del cliente
        self.correlation_id = None      # Id de correlación de respuesta

    def conectar_rabbitmq(self):
        """
        Establece la conexión con el servidor RabbitMQ y prepara la cola de respuestas para el cliente.

        Ret:
            True si la conexión salió bien, False si hubo un error.
        """
        try:
            self.conexion = pika.BlockingConnection(pika.ConnectionParameters(RABBITMQ_HOST))
            self.canal = self.conexion.channel()

            # Declaramos la cola principal usada para las llamadas RPC
            self.canal.queue_declare(queue="cola_rpc", durable=False, auto_delete=True)

            # Creamos una cola exclusiva para recibir las respuestas de este cliente
            result = self.canal.queue_declare(queue='', exclusive=True)
            self.cola_respuestas = result.method.queue

            return True
        except Exception as e:
            print(f"Error al conectar con RabbitMQ: {e}")
            return False

    def cerrar_conexion(self):
        """
        Cierra la conexión con el servidor RabbitMQ, si está abierta.
        No retorna nada.
        """
        if hasattr(self, 'conexion') and self.conexion and self.conexion.is_open:
            self.conexion.close()

    def enviar_solicitud(self, accion, datos):
        """
        Envia una solicitud RPC con una acción específica y espera la respuesta correspondiente.

        Args:
            accion: El tipo de accion que se quiere realizar
            datos: Diccionario con los datos necesarios para la operación.

        Ret:
            Diccionario con la respuesta del servidor o None si hay timeout o error.
        """
        if not hasattr(self, 'canal') or not self.canal:
            if not self.conectar_rabbitmq():
                return None

        self.correlation_id = str(uuid.uuid4())

        mensaje = {
            "accion": accion,
            **datos
        }

        # Publicamos el mensaje en la cola RPC
        self.canal.basic_publish(
            exchange='',
            routing_key="cola_rpc",
            properties=pika.BasicProperties(
                reply_to=self.cola_respuestas,
                correlation_id=self.correlation_id,
                delivery_mode=2,
            ),
            body=json.dumps(mensaje).encode()
        )


        return self.esperar_respuesta()

    def registrar(self, username, password):
        """
        Registra un nuevo usuario en el sistema.

        Args:
            username: Nombre de usuario deseado.
            password: Contraseña del usuario.

        Ret:
            True si el registro salió bien, False en caso contrario.
        """
        datos = {
            "cliente_username": username,
            "cliente_password": password
        }

        respuesta = self.enviar_solicitud("registrar", datos)

        if respuesta:
            if respuesta.get("exito"):
                print(f"Registro exitoso. Usuario '{username}' creado.")
                return True
            else:
                print(f"Error en el registro: {respuesta.get('mensaje')}")
                return False
        else:
            print("No se recibió respuesta del servidor.")
            return False

    def iniciar_sesion(self, username, password):
        """
        Intenta iniciar sesión con las credenciales del usuario.

        Args:
            username: Nombre de usuario.
            password: Contraseña.

        Ret:
            True si se inició sesión correctamente, False si falló.
        """
        datos = {
            "cliente_username": username,
            "cliente_password": password
        }

        respuesta = self.enviar_solicitud("login", datos)

        if respuesta:
            if respuesta.get("exito"):
                self.cliente_id = respuesta.get("cliente_id")
                self.cliente_username = username
                print(f"Sesión iniciada correctamente para {username}")
                return True
            else:
                print(f"Error al iniciar sesión: {respuesta.get('mensaje')}")
                return False
        else:
            print("No se recibió respuesta del servidor.")
            return False

    def hacer_pedido(self, productos):
        """
        Realiza un nuevo pedido con los productos proporcionados.

        Args:
            productos: Lista de nombres de productos.

        Ret:
            True si el pedido fue procesado correctamente, False en caso de error.
        """
        if not self.cliente_id:
            print("Debe iniciar sesión antes de hacer un pedido.")
            return False

        datos = {
            "cliente_id": self.cliente_id,
            "productos": productos
        }

        respuesta = self.enviar_solicitud("hacer_pedido", datos)

        if respuesta:
            if respuesta.get("exito"):
                print(f"→ {respuesta.get('mensaje')}")
                return True
            else:
                print(f"Error: {respuesta.get('mensaje')}")
                return False
        else:
            print("No se recibió respuesta del servidor.")
            return False

    def ver_pedidos(self):
        """
        Recupera y muestra en pantalla los pedidos del usuario autenticado.

        Ret:
            True si se obtuvieron los pedidos, False si hubo algún problema.
        """
        if not self.cliente_id:
            print("Debe iniciar sesión antes de ver sus pedidos.")
            return False

        datos = {
            "cliente_id": self.cliente_id
        }

        respuesta = self.enviar_solicitud("ver_pedidos", datos)

        if respuesta:
            if respuesta.get("exito"):
                pedidos = respuesta.get("pedidos", [])
                if pedidos:
                    print("\n\n\n=============== TUS PEDIDOS ===============")
                    for p in pedidos:
                        productos_str = ", ".join(p['productos']) if isinstance(p['productos'], list) else p['productos']
                        print(f"- ID: {p['id']} | Productos: {productos_str} | Estado: {p['estado']} | Fecha: {p['fecha']}")
                else:
                    print("No tienes pedidos registrados.")
                return True
            else:
                print(f"Error: {respuesta.get('mensaje')}")
                return False
        else:
            print("No se recibió respuesta del servidor.")
            return False

    def cancelar_pedido(self, pedido_id):
        """
        Permite al usuario cancelar un pedido previamente realizado.

        Args:
            pedido_id: Identificador del pedido a cancelar.

        Ret:
            True si se canceló correctamente, False en caso contrario.
        """
        if not self.cliente_id:
            print("Debe iniciar sesión antes de cancelar pedidos.")
            return False

        datos = {
            "cliente_id": self.cliente_id,
            "pedido_id": pedido_id
        }

        respuesta = self.enviar_solicitud("cancelar_pedido", datos)

        if respuesta:
            print(f"{respuesta.get('mensaje')}")
            return respuesta.get("exito", False)
        else:
            print("No se recibió respuesta del servidor.")
            return False

    def esperar_respuesta(self):
        """
        Espera la respuesta del servidor dentro del TIMEOUT.

        Ret:
            Diccionario con la respuesta si llega a tiempo, o None si expira el tiempo de espera.
        """
        inicio = time.time()

        while time.time() - inicio < TIMEOUT:
            method_frame, properties, body = self.canal.basic_get(
                queue=self.cola_respuestas,
                auto_ack=True
            )

            if method_frame:
                if properties.correlation_id == self.correlation_id:
                    return json.loads(body.decode())

            time.sleep(0.1)

        return None


# Esta función representa el menú de opciones que verá el usuario.
def menu_principal():
    cliente = Cliente()

    while True:
        print("\n\n\n\n===== SISTEMA DE PEDIDOS SAIMAZOOM =====")
        if cliente.cliente_id:
            print(f"Usuario: {cliente.cliente_username} (ID: {cliente.cliente_id})")
        else:
            print("No hay sesión iniciada")

        print("\n1. Registrarse")
        print("2. Iniciar sesión")
        print("3. Hacer un pedido")
        print("4. Ver mis pedidos")
        print("5. Cancelar un pedido")
        print("6. Cerrar sesión")
        print("7. Salir")

        opcion = input("\nSeleccione una opción: ")

        if opcion == "1":
            username = input("Ingrese nombre de usuario: ")
            password = input("Ingrese contraseña: ")
            cliente.registrar(username, password)

        elif opcion == "2":
            username = input("Ingrese nombre de usuario: ")
            password = input("Ingrese contraseña: ")
            cliente.iniciar_sesion(username, password)

        elif opcion == "3":
            if cliente.cliente_id:
                productos_str = input("Ingrese nombres de productos separados por coma (ej: Manzanas,Peras,Leche): ")
                productos = [p.strip() for p in productos_str.split(",") if p.strip()]
                if productos:
                    cliente.hacer_pedido(productos)
                else:
                    print("No se ingresaron productos válidos.")
            else:
                print("Debe iniciar sesión primero.")

        elif opcion == "4":
            if cliente.cliente_id:
                cliente.ver_pedidos()
            else:
                print("Debe iniciar sesión primero.")

        elif opcion == "5":
            if cliente.cliente_id:
                if cliente.ver_pedidos():
                    pedido_id = input("\nIngrese el ID del pedido que desea cancelar: ")
                    cliente.cancelar_pedido(pedido_id)
            else:
                print("Debe iniciar sesión primero.")

        elif opcion == "6":
            if cliente.cliente_id:
                cliente.cliente_id = None
                cliente.cliente_username = None
                print("Sesión cerrada correctamente.")
            else:
                print("No hay sesión activa.")

        elif opcion == "7":
            print("Saliendo del sistema...")
            cliente.cerrar_conexion()
            break

        else:
            print("Opción no válida. Intente de nuevo.")


# Punto de entrada del programa.
if __name__ == "__main__":
    menu_principal()