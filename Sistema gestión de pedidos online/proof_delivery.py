#!/usr/bin/env python
from src.client import Cliente
import time

def simular_cliente():
    c = Cliente()
    
    if not c.conectar_rabbitmq():
        print("No se pudo conectar a RabbitMQ")
        return

    # Registro e inicio de sesión
    username = "daniel"  # Unique username
    password = "daniel1234"

    print("Iniciando sesión")
    respuesta_login = c.iniciar_sesion(username, password)
    if isinstance(respuesta_login, dict) and not respuesta_login.get("exito"):
        print(f"Error en inicio de sesión: {respuesta_login.get('mensaje', 'Error desconocido')}")
        c.cerrar_conexion()
        return
    elif respuesta_login is False:
        print("Fallo en inicio de sesión: Credenciales inválidas o error desconocido")
        c.cerrar_conexion()
        return
    
    # Hacer un pedido
    print("Haciendo un pedido")
    productos = ["Leche", "Pepino", "Galletas"]
    respuesta_pedido = c.hacer_pedido(productos)
    if isinstance(respuesta_pedido, dict) and not respuesta_pedido.get("exito"):
        print(f"Error al hacer pedido: {respuesta_pedido.get('mensaje', 'Error desconocido')}")
        c.cerrar_conexion()
        return
    elif respuesta_pedido is False:
        print("Error al hacer pedido: Fallo desconocido")
        c.cerrar_conexion()
        return
    

    # Esperar a que el pedido se complete
    print("Esperando a que el pedido se complete...")

if __name__ == "__main__":
    simular_cliente()