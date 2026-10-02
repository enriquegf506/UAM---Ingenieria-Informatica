#!/usr/bin/env python3
import json
import sys

#obtenemos el id actual del usuario daniel en clientes.json

def obtener_id_usuario(username):
    """
    Obtiene el ID del usuario a partir de su nombre de usuario.
    
    Args:
        username (str): Nombre de usuario.
    
    Returns:
        str: ID del usuario o None si no se encuentra.
    """
    with open('clientes.json', 'r') as f:
        data = json.load(f)
        for user_id, user_info in data.items():
            if user_info['username'] == username:
                return user_id
    return None

# Obtenemos los estados de los pedidos del usuario con id obtenido anteriormente con formato de pedidos, en pedidos.json: 

def obtener_pedidos_usuario(cliente_id):
    """
    Obtiene los pedidos de un usuario a partir de su ID.
    
    Args:
        cliente_id (str): ID del cliente.
    
    Returns:
        list: Lista de pedidos del cliente o None si no se encuentra.
    """
    with open('pedidos.json', 'r') as f:
        data = json.load(f)
        pedidos = []
        for pedido_id, pedido_info in data.items():
            if pedido_info['cliente_id'] == cliente_id:
                pedidos.append(pedido_info)
    return pedidos

# Verificar si todos los pedidos en estado "entregado" o "cancelado", si es asi devolvemos 0 si no devolvemos 1
def verificar_pedidos(pedidos):
    """
    Verifica si todos los pedidos están en estado "entregado" o "cancelado".
    
    Args:
        pedidos (list): Lista de pedidos.
    
    Returns:
        int: 0 si todos los pedidos están en estado "entregado" o "cancelado", 1 si no.
    """
    for pedido in pedidos:
        if pedido['estado'] not in ['entregado', 'cancelado']:
            return 1
    return 0

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python check_orders.py <username>")
        sys.exit(1)

    username = sys.argv[1]
    cliente_id = obtener_id_usuario(username)
    
    if cliente_id is None:
        print(f"Usuario {username} no encontrado.")
        sys.exit(1)

    pedidos = obtener_pedidos_usuario(cliente_id)
    
    if not pedidos:
        print(f"No se encontraron pedidos para el usuario {username}.")
        sys.exit(1)

    estado = verificar_pedidos(pedidos)
    
    if estado == 0:
        print(f"Todos los pedidos del usuario {username} están en estado 'entregado' o 'cancelado'.")
    else:
        print(f"Algunos pedidos del usuario {username} no están en estado 'entregado' o 'cancelado'.")
        sys.exit(1)
    sys.exit(0)