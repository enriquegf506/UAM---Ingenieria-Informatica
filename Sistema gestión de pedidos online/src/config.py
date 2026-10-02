# config.py

# Archivos de almacenamiento
ARCHIVO_CLIENTES = "clientes.json"  # Archivo donde se guardan los datos de los clientes registrados
ARCHIVO_PEDIDOS = "pedidos.json"    # Archivo donde se almacenan los pedidos realizados

# RabbitMQ
RABBITMQ_HOST = 'localhost'         # Dirección del servidor RabbitMQ (en este caso, el mismo equipo)
RABBITMQ_PORT = 5672                # Puerto por defecto de RabbitMQ para conexiones AMQP
RABBITMQ_USER = 'guest'             # Usuario por defecto de RabbitMQ
RABBITMQ_PASS = 'guest'             # Contraseña del usuario por defecto
RABBITMQ_VHOST = '/'                # Virtual host por defecto utilizado en RabbitMQ

# Repartidor
P_ENTREGA = 0.7  # Probabilidad de éxito en cada intento de entrega

# Robot
P_ALMACEN = 0.8  # Probabilidad de éxito en cada búsqueda de producto

# Cliente
TIMEOUT = 5  # Tiempo de espera (en segundos) antes de que el cliente considere que la operación falló