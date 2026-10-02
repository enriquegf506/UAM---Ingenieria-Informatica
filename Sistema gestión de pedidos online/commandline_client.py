import argparse
from src.client import Cliente

def main():
    parser = argparse.ArgumentParser(description="Simulación de cliente de Saimazoom por línea de comandos")

    parser.add_argument("--registrar", action="store_true", help="Registrar un nuevo usuario")
    parser.add_argument("--login", action="store_true", help="Iniciar sesión")
    parser.add_argument("--username", type=str, help="Nombre de usuario")
    parser.add_argument("--password", type=str, help="Contraseña")

    parser.add_argument("--hacer-pedido", nargs="+", help="Lista de productos a pedir (ej: --hacer-pedido Cafe Te Leche)")
    parser.add_argument("--ver-pedidos", action="store_true", help="Consultar pedidos del cliente")
    parser.add_argument("--cancelar-pedido", type=str, help="Cancelar pedido por ID")

    args = parser.parse_args()

    cliente = Cliente()
    if not cliente.conectar_rabbitmq():
        print("Error al conectar con RabbitMQ.")
        return

    # Registro
    if args.registrar:
        if not args.username or not args.password:
            print("Para registrar, proporcione --username y --password")
            return
        cliente.registrar(args.username, args.password)

    # Login
    if args.login:
        if not args.username or not args.password:
            print("Para iniciar sesión, proporcione --username y --password")
            return
        if not cliente.iniciar_sesion(args.username, args.password):
            return

    # Hacer pedido
    if args.hacer_pedido:
        if not cliente.cliente_id:
            print("Debe iniciar sesión antes de hacer un pedido.")
            return
        cliente.hacer_pedido(args.hacer_pedido)

    # Ver pedidos
    if args.ver_pedidos:
        if not cliente.cliente_id:
            print("Debe iniciar sesión antes de ver pedidos.")
            return
        cliente.ver_pedidos()

    # Cancelar pedido
    if args.cancelar_pedido:
        if not cliente.cliente_id:
            print("Debe iniciar sesión antes de cancelar pedidos.")
            return
        cliente.cancelar_pedido(args.cancelar_pedido)

    cliente.cerrar_conexion()


if __name__ == "__main__":
    main()