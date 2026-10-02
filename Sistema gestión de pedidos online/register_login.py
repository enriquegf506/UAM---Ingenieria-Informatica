#!/usr/bin/env python
from src.client import Cliente
import time

def register_and_login_daniel():
    c = Cliente()
    
    if not c.conectar_rabbitmq():
        print("No se pudo conectar a RabbitMQ")
        return

    # Registro e inicio de sesión
    username = "daniel"
    password = "daniel1234"

    # Registro
    print("Registrando usuario")
    respuesta_registro = c.registrar(username, password)
    if isinstance(respuesta_registro, dict) and not respuesta_registro.get("exito"):
        print(f"Error en registro: {respuesta_registro.get('mensaje', 'Error desconocido')}")
        c.cerrar_conexion()
        return
    elif respuesta_registro is False:
        print("Fallo en registro: Usuario ya existe o error desconocido")
        c.cerrar_conexion()
        return
    # Esperar un momento para simular el tiempo de registro
    time.sleep(1)

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
    
    print("Registro e inicio de sesión completados exitosamente.")
    c.cerrar_conexion()

if __name__ == "__main__":
    register_and_login_daniel()