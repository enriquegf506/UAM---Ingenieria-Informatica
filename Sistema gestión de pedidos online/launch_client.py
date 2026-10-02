# launch_client.py
from src.client import Cliente
import time

def simular_cliente():
    """Simula un cliente y realiza varias acciones"""
    c = Cliente()
    
    if not c.conectar_rabbitmq():
        print("No se pudo conectar a RabbitMQ")
        return

    # Registro e inicio de sesión
    username = "cliente ejemplo_" + str(int(time.time()))
    password = "contraseña ejemplo"

    print("Registrando cliente")
    c.registrar(username, password)
    
    print("Iniciando sesión")
    if not c.iniciar_sesion(username, password):
        return
    
    # Hacer un pedido
    print("Haciendo un pedido")
    productos = ["Leche", "Pepino", "Galletas"]
    c.hacer_pedido(productos)

    time.sleep(1)

    # Ver pedidos
    print("Consultando pedidos")
    c.ver_pedidos()

    time.sleep(1)

    # Intentar cancelar un pedido
    print("→ Intentando cancelar el primer pedido...")
    respuesta = c.enviar_solicitud("ver_pedidos", {"cliente_id": c.cliente_id})
    if respuesta and respuesta.get("exito") and respuesta.get("pedidos"):
        pedido_id = respuesta["pedidos"][0]["id"]
        c.cancelar_pedido(pedido_id)

    c.cerrar_conexion()

if __name__ == "__main__":
    simular_cliente()
