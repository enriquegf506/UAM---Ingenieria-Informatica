
# Índice
1. [Introdución](#1-introducción)
2. [Entorno de desarrollo](#2-entorno-de-desarrollo)
3. [Decisiones de diseño](#3-decisiones-de-diseño)
4. [Arquitectura del sistema](#4-arquitectura-del-sistema)
5. [Implementación](#5-implementación)
6. [Scripts](#6-scripts)
7. [Tests](#7-tests)
8. [Conclusión](#8-conclusión)

---

# 1. Introducción
El objetivo de Saimazoom es el de crear un sistema para la gestión de pedidos online. Este sistema debe incluir a los actores:
* **Cliente**, que realiza y gestiona pedidos de productos.
* **Controlador** central, que gestiona todo el proceso.
* **Robots**, que se encargan de buscar los productos en el almacén y colocarlos en las cintas transportadoras.
* **Repartidores**, encargados de transportar el producto a la casa del cliente
* **Admin** encargados de gestionar la base de datos del controlador central

El sistema debe de gestionar las interacciones entre todos estos actores, para las comunicaciones correspondientes se empleará una cola de mensajes.

---

# 2. Entorno de desarrollo

## Requisitos de Software

- Python: Versión 3.7 o superior
- RabbitMQ: Versión 3.8 o superior
- Pika: Biblioteca cliente para RabbitMQ (versión 1.2.0 o superior)
- Editor de código: Visual Studio Code
- Control de versiones: Git para gestión del código fuente


### Configuración del Entorno de Desarrollo

#### Instalación de Dependencias:

```
pip install pika
```
#### Instalación de RabbitMQ Local en Linux:

```
sudo apt-get install rabbitmq-server
```

#### Configuración de RabbitMQ:

```
sudo service rabbitmq-server start

sudo rabbitmq-plugins enable rabbitmq_management
```

---

# 3. Decisiones de diseño
## 1. Implementación híbrida de patrones de comunicación

El sistema emplea dos patrones de comunicación distintos según el tipo de interacción:

### Patrón RPC (Remote Procedure Call)
 
Para comunicaciones cliente-controlador debido a que el patrón RPC es adecuado para operaciones que requieren respuesta inmediata (como autenticación o las acciones como ver pedidos).

### Patrón productor-consumidor

Para tareas de robots y repartidores debido a que el patrón productor-consumidor permite desacoplar totalmente los componentes del sistema, facilitando la escalabilidad.

## 2. Almacenamiento basado en archivos JSON

La elección de utilizar archivos JSON ofrece ventajas como la simplicidad al evitar la complejidad de configurar y mantener una base de datos, la portabilidad al ser independientes de cualquier sistema gestor de bases de datos, y la legibilidad al facilitar la depuración y el análisis manual de los datos. Sin embargo,  sacrifica la capacidad de realizar consultas complejas, pero debido a que el sistema no conlleva hacer consultas de gran dificultad, se hace completamente compatible.

## 3. Captura de excepciones en cada función

La decisión de implementar una captura de excepciones en cada función busca mejorar la robustez del código al manejar errores de manera controlada, facilitando la identificación y solución de problemas. Sin embargo, esto puede requerir un esfuerzo adicional en el desarrollo, ya que cada función debe incluir su propio bloque de manejo de excepciones. A pesar de esto, prevenir fallos inesperados siempre es mejor que lidiar con sus consecuencias después.

## 4. Timeout en espera de respuestas RPC

Implementar un timeout en las llamadas RPC es clave para evitar que el sistema quede bloqueado indefinidamente si un servicio remoto falla o no responde. Al definir un límite de espera, se garantiza que la aplicación pueda manejar errores, ya sea reintentando la operación, usando datos en caché o notificando al usuario. Sin embargo, hay que equilibrar el tiempo asignado: un timeout demasiado corto puede causar falsos errores por latencia temporal, mientras que uno muy largo retrasa la detección de fallos reales. Por ello hemos pensado que 5 segundos es un tiempo suficiente para el procesamiento y no hacer esperar demasiado al usuario.

## 5. Retardos en el procesamiento

En robots

```
tiempo_trabajo = random.uniform(5, 10)
time.sleep(tiempo_trabajo)
```

En repartidores

```
tiempo_entrega = random.uniform(10, 20)
time.sleep(tiempo_entrega)
```  

La implementación de retardos aleatorios en el procesamiento, como en robots (5-10 segundos) y repartidores (10-20 segundos), busca emular de manera realista los tiempos variables que ocurren en sistemas físicos y operaciones del mundo real. Esta variabilidad intencional ayuda a simular condiciones más auténticas, donde los procesos no siempre tardan exactamente lo mismo debido a factores externos o complejidades inherentes a cada tarea. En particular, el rango más amplio para repartidores refleja la mayor imprevisibilidad y complejidad de las entregas frente a tareas más controladas como las de los robots. Además, este enfoque permite validar el correcto funcionamiento asíncrono del sistema, asegurando que pueda manejar fluctuaciones en los tiempos de respuesta sin fallos. 

## 6. Correlation IDs en la comunicación cliente-controlador

Al asignar un identificador único a cada cadena de solicitudes y respuestas, se habilita un seguimiento end-to-end que permite correlacionar operaciones. Esto no solo mejora la capacidad de debugging al facilitar el rastreo de flujos completos de comunicación en logs distribuidos, sino que también permite a los clientes filtrar respuestas no esperadas ignorando aquellas cuyo Correlation ID no coincida con su solicitud actual, evitando así procesar datos obsoletos o duplicados. Además, en un escenario con alto nivel de concurrencia, esta técnica ayuda a mantener la coherencia al asegurar que cada respuesta sea correctamente emparejada con su solicitud original. 

---

# 4. Arquitectura del sistema
## 1. Cliente
El cliente implementa una interfaz de línea de comandos (CLI) que permite:

- Registrar nuevo cliente
- Iniciar sesión
- Hacer pedidos
- Ver pedidos realizados
- Cancelar pedidos
- Cerrar sesión

Utiliza una cola de respuestas exclusiva para cada cliente, generando un ID de correlación único para cada solicitud. Implementa un mecanismo de timeout para evitar esperar indefinidamente por respuestas. Mantiene el estado de la sesión del usuario en memoria, almacenando el ID de cliente y nombre de usuario.


## 2. Controlador
El controlador es el núcleo del sistema, procesando las solicitudes de los clientes y coordinando los robots y repartidores. Implementa:

- Registro e inicio de sesión de usuarios
- Gestión de pedidos (creación, visualización, cancelación)
- Seguimiento del estado de los pedidos
- Comunicación con robots y repartidores

Utiliza archivos JSON para almacenar información de clientes y pedidos. Implementa un sistema de estados para los pedidos:

recibido -> en_almacen -> en_cinta -> en_reparto -> entregado ----- cancelado (posible tras cualquiera de los primeros tres estados)

También registra el historial completo de cambios de estado de cada pedido, incluyendo fechas y motivos de cancelación.

Por último, creará un archivo de llamado controlador.log en la carpeta logs donde llevará un seguimiento de todos los pasos realizados por él mismo.


## 3. Robot
El robot simula la búsqueda de productos en el almacén:

Utiliza una probabilidad configurable para simular si un producto se encuentra en el almacén e introduce esperas aleatorias entre 5 y 10 segundos para simular el tiempo de búsqueda. Cada robot se ejecuta en un hilo independiente.


## 4. Repartidor

El repartidor simula el proceso de entrega de pedidos:

Realiza hasta tres intentos de entrega antes de considerar un pedido como no entregable. Utiliza una probabilidad configurable para cada intento de entrega. Introduce esperas aleatorias entre 10 y 20 segundos para simular el tiempo de entrega. Informa al controlador sobre el inicio de la distribución y el resultado de cada intento. Por último, cada repartidor se ejecuta en un hilo independiente.

## 5. Config

Archivo de configuración del sistema con los parámetros configurables.

Estructura:

```
# Archivos de almacenamiento
ARCHIVO_CLIENTES = "clientes.json"
ARCHIVO_PEDIDOS = "pedidos.json"

# RabbitMQ
RABBITMQ_HOST = 'localhost'
RABBITMQ_PORT = 5672
RABBITMQ_USER = 'guest'
RABBITMQ_PASS = 'guest'
RABBITMQ_VHOST = '/'

# Repartidor
P_ENTREGA = 0.7

# Robot
P_ALMACEN = 0.8  

# Cliente
TIMEOUT = 5
```

A continuación, se detallan los parámetros definidos:

- **ARCHIVO_CLIENTES**: Nombre del archivo JSON que almacena de forma persistente la información de los clientes registrados en el sistema.
- **ARCHIVO_PEDIDOS**: Archivo donde se registran los pedidos realizados por los clientes. Este archivo actúa como base de datos persistente durante la ejecución del sistema.
- **RABBITMQ_HOST**: Dirección del host que ejecuta el servidor de colas RabbitMQ. En entornos locales, se utiliza 'localhost'.
- **RABBITMQ_PORT**: Puerto de comunicación del servidor RabbitMQ para conexiones AMQP. El valor por defecto (5672) se mantiene, ya que es el estándar en instalaciones típicas.
- **RABBITMQ_USER y RABBITMQ_PASS**: Credenciales por defecto del usuario 'guest' de RabbitMQ, válidas únicamente para conexiones desde localhost salvo que se configure lo contrario.
- **RABBITMQ_VHOST**: Virtual host utilizado dentro de RabbitMQ. El sistema trabaja sobre el entorno predeterminado '/'.
- **P_ENTREGA** (float): Probabilidad de éxito en cada intento de entrega de un pedido por parte del repartidor. En este caso, el valor está fijado en 0.7, lo que implica un 70% de probabilidad de éxito en cada intento individual.
- **P_ALMACEN** (float): Probabilidad de que el robot logre localizar un producto dentro del almacén. Este valor se ha fijado en 0.8, reflejando un 80% de éxito en cada búsqueda.
- **TIMEOUT** (int): Tiempo de espera (en segundos) que el cliente considerará como máximo para obtener una respuesta del sistema antes de asumir que ha ocurrido un fallo. Esto simula tiempos de espera en sistemas distribuidos reales.

---

# 5. Implementación
## Fase 1: Análisis y Diseño

### Análisis de Requisitos

- Identificación de actores (clientes, robots, repartidores)
- Especificación de requisitos funcionales y no funcionales


### Diseño de Arquitectura

- Selección del patrón de arquitectura distribuida basada en mensajes
- Diseño del modelo de comunicación RPC y flujo de mensajes


### Diseño de Datos

- Estructura de mensajes JSON para comunicación cliente-controlador
- Formato de protocolos de texto para robots y repartidores
- Estructura de almacenamiento para clientes y pedidos


## Fase 2: Sistema Base

### Implementación del Middleware

- Configuración básica de RabbitMQ
- Pruebas iniciales de envío y recepción de mensajes


### Desarrollo del Controlador Básico

- Implementación del servidor RPC para atender solicitudes
- Estructura básica para almacenar clientes y pedidos
- Funciones de registro e inicio de sesión


### Cliente de Prueba

- Funcionalidad de registro y autenticación
- Mecanismo RPC para comunicación con el controlador


## Fase 3: Gestión de Pedidos y Máquina de Estados

### Implementación de la Máquina de Estados

- Definición de estados del pedido
- Transiciones entre estados
- Historial de cambios de estado

### Funcionalidad de Pedidos

- Creación de pedidos
- Consulta de pedidos existentes
- Cancelación de pedidos


### Persistencia de Datos

- Almacenamiento en archivos JSON


## Fase 4: Implementación de Robots y Repartidores

### Desarrollo del Robot

- Simulación de búsqueda de productos
- Probabilidad configurable de éxito
- Comunicación asíncrona con el controlador


### Desarrollo del Repartidor

- Implementación de múltiples intentos de entrega
- Simulación de tiempo de entrega
- Gestión de éxitos y fallos


### Integración con el Controlador

- Coordinación de pedidos con robots
- Procesamiento de respuestas de robots y repartidores
- Actualización de estados en consecuencia



## Fase 5: Mejoras y Robustez

### Manejo de Errores y Excepciones

- Captura y gestión de problemas
- Respuestas de error

### Implementación de un sistema de logs y un archivo de configuración
- Implementar sistema de loggeer en el controlador.py
- Un archivo de configuracióin con los parámetros configurables del proyecto config.py


## Fase 6: Scripts y Tests

- Implementación de scripts de lanzamiento para cada componente
- Desarrollo de scripts de simulación para pruebas automáticas
- Creación de escenarios de prueba simulando situaciones reales
- Implementación de cliente de línea de comandos avanzado


## Fase 7: Documentación

- Documentación Técnica
- Diagrama de clases
- Diagrama de estados de un pedido
- Tres casos de uso
- Descripción de los mensajes
- Documento de requisitos 

---

# 6. Scripts
## Explicación de los scripts disponibles

### 1. launch_client

Este script simula las acciones de un cliente en un sistema que usa RabbitMQ para comunicación. Realiza:

- Conexión a RabbitMQ → Intenta establecer conexión con el servidor.
- Registro e inicio de sesión → Crea un usuario único (con timestamp) y contraseña de ejemplo.
- Pedido → Solicita productos ("Leche", "Pepino", "Galletas").
- Consulta de pedidos → Verifica los pedidos realizados.
- Cancelación de pedido → Intenta cancelar el primer pedido de la lista.

Las pausas (time.sleep(1)) simulan esperas entre operaciones.

### 2. launch_controler

Su función es inicializar el sistema llamando al Controlador.

### 3. commandline_client

Este script implementa una interfaz de línea de comandos (CLI) para interactuar con el sistema Saimazoom, permitiendo gestionar clientes.

#### Funcionalidades:

    **--registrar**: Crea un nuevo usuario (requiere --username y --password).

    **--login**: Inicia sesión (requiere credenciales).

    **--hacer-pedido** [productos]: Realiza un pedido (ej: --hacer-pedido Cafe Pan Leche).

    **--ver-pedidos**: Muestra los pedidos del cliente.

    **--cancelar-pedido** [ID]: Cancela un pedido por su ID.

#### Ejemplo de ejecución:

Registrar nuevo usuario:
```
python cliente_cli.py --registrar --username "usuario1" --password "1234"
```

Iniciar sesión y hacer pedido:
```
python cliente_cli.py --login --username "usuario1" --password "1234" --hacer-pedido Cafe Galletas
```

Ver pedidos:
```
python cliente_cli.py --login --username "usuario1" --password "1234" --ver-pedidos
```

Cancelar pedido (ej: ID=123):
```
python cliente_cli.py --login --username "usuario1" --password "1234" --cancelar-pedido 123
```


### 4. launch_delivery

Este script inicia múltiples repartidores en paralelo (simulando un sistema de delivery distribuido). Cada repartidor opera como un consumidor independiente de pedidos.
- Crea NUM_REPARTIDORES instancias de la clase Repartidor.
- Cada uno tiene un ID único generado con uuid (ej: Repartidor-3a7b9c2f).
- Llama al método run() de cada repartidor (que presumiblemente inicia un bucle de consumo de pedidos).
- El script se mantiene activo hasta que se presiona Ctrl+C (KeyboardInterrupt).

### 5. launch_robot
Este script inicia múltiples robots en paralelo.
- Crea NUM_ROBOTS instancias de la clase Robot (por defecto 1).
- Genera IDs únicos cortos con uuid (ej: Robot-d4e6f2).
- Ejecuta robot.run() (presumiblemente inicia un bucle de procesamiento de tareas).
- Permanece activo hasta recibir Ctrl+C, mostrando estado con prints.

### 6. proof_delivery
Simula el flujo completo de un cliente en un sistema de pedidos con RabbitMQ:
1. Establece conexión con RabbitMQ (falla si no puede conectar)
2. Inicia sesión con credenciales fijas ("daniel"/"daniel1234")
3. Realiza pedido con productos fijos (Leche, Pepino, Galletas)
4. Valida respuesta similar al login
5. Espera activamente la finalización del pedido
6. Cierra conexiones limpiamente ante cualquier fallo

### 7. register_login
Simula el flujo de registro y autenticación de un cliente en el sistema de pedidos Saimazoom con RabbitMQ:
1. Establece conexión con RabbitMQ: Intenta conectar al servidor de mensajería. Si falla, muestra un mensaje de error y termina la ejecución.
2. Registra un usuario con credenciales fijas: Usa el nombre de usuario "daniel" y la contraseña "daniel1234" para registrar un nuevo cliente.
3. Valida la respuesta del registro: Comprueba si el registro fue exitoso. Si falla (por ejemplo, usuario ya existente), muestra un mensaje de error y cierra la conexión.
4. Inicia sesión con las mismas credenciales: Intenta autenticar al usuario recién registrado.
5. Valida la respuesta del inicio de sesión: Verifica si la autenticación fue exitosa. Si falla, muestra un mensaje de error y cierra la conexión.
6. Cierra conexiones limpiamente: Asegura que la conexión con RabbitMQ se cierre correctamente, incluso en caso de fallo.

---

# 7. Tests
Se han creado los siguientes tests que comprueban un funcionamiento completo del sistema comprobando distintos casos:

Todos ellos utilizan el script: check_orders.py, el cual verifica que para un usuario, todos los pedidos están en estado "cancelado" o "entregado", y asi poder finalizar los test correctemente.
- Uso: python check_orders.py "username"

IMPORTANTE: Se aconseja ejecutar los scripts en orden, ya que aseguran un entorno de funcionamiento lógico, sin errores de por medio, que no serían posibles en un entorno real. Para ello ejecutar el script en python register_login, para el usurio daniel, contraseña daniel1234

### 1. run_register_login.sh
Simula un flujo completo de registro e inicio de sesión de un cliente en el sistema, verificando credenciales.

1. Verifica que RabbitMQ esté en ejecución: Comprueba si el servidor RabbitMQ está activo. Si no lo está, muestra un mensaje de error y termina la ejecución.
2. Configura el entorno: Define variables como el nombre de usuario (daniel), contraseña (daniel1234), rutas de scripts, y parámetros como el intervalo de verificación (CHECK_INTERVAL) y tiempo máximo de espera (TIMEOUT).
3. Inicia el controlador: Ejecuta el script launch_controler.py en segundo plano para coordinar las operaciones del sistema y verifica que se inicie correctamente.
4. Inicia el cliente: Ejecuta el script register_login.py en segundo plano para simular el registro y autenticación del usuario daniel. Verifica si el proceso del cliente se inicia correctamente.
5. Espera a que el cliente finalice: Aguarda la finalización del script register_login.py antes de continuar.
6. Limpia los procesos: Asegura que todos los procesos iniciados (controlador y cliente) se terminen correctamente, incluso si el script es interrumpido (usando un mecanismo de trap).
7. Maneja errores y limpieza: Cierra conexiones y procesos limpiamente ante fallos o interrupciones, garantizando que no queden procesos huérfanos

### 2. run_basics.sh
Simula un flujo completo de almacén con un cliente, un robot y un repartidor, verificando el estado de los pedidos hasta su finalización.
Funcionalidades principales:

1. Comprueba que RabbitMQ esté en ejecución antes de comenzar.
2. Crea un directorio de logs con timestamp para almacenar los registros.
3. Inicia los componentes (en orden):
   - Controlador: Gestor principal (launch_controler.py).
   - Robot: Procesa pedidos (launch_robot.py).
   - Repartidor: Maneja entregas (launch_repartidor.py).
   - Cliente: Simula un usuario (proof_delivery.py).
4. Espera a que el cliente complete su ejecución.
5. Verifica periódicamente (cada 10 segundos) el estado de los pedidos mediante check_orders.py.
6. Si hay pedidos pendientes después de 5 minutos (timeout), muestra un mensaje de advertencia.
7. Limpieza automática: Mata todos los procesos en segundo plano al finalizar o si ocurre un error.
8. Registra los logs de cada componente en archivos separados.
9. Muestra un resumen del estado de los pedidos.

### 3. run_high_load.sh
Este script simula un escenario de alta carga con múltiples robots, repartidores y clientes. Verifica que RabbitMQ esté en ejecución, lanza los componentes necesarios (controlador, robots, repartidores y clientes), y espera a que los pedidos sean completados. Realiza verificaciones periódicas de estado, y si todos los pedidos no se completan dentro de un tiempo máximo de 10 minutos, finaliza con un mensaje de timeout. Al final, limpia los procesos y muestra el estado final de los pedidos. Los logs se guardan para su revisión.

### 4. run_multiple_clients.sh
Este script automatiza una prueba del sistema con múltiples clientes haciendo pedidos al mismo tiempo. Lanza los componentes clave (controlador, robot, repartidor y clientes), y espera a que los clientes terminen. Luego, verifica periódicamente si todos los pedidos han sido entregados o cancelados, con un tiempo máximo de espera de 5 minutos. Todos los procesos quedan registrados en logs y se limpian automáticamente al finalizar o si ocurre una interrupción.

### 5. run_failure.sh
Este script simula un escenario de fallo en un sistema distribuido de pedidos, lanzando un controlador, un solo robot y varios clientes, pero sin repartidores, para provocar cuellos de botella. Verifica si RabbitMQ está activo, ejecuta los procesos, espera a que los clientes terminen y comprueba si hay pedidos atascados. Además, guarda logs y limpia todos los procesos al finalizar.

---

# 8. Conclusión
El proyecto muestra cómo construir un sistema distribuido para gestionar pedidos en un e-commerce, usando RabbitMQ como elemento clave para coordinar la comunicación entre diferentes partes (como el cliente, el controlador, los robots del almacén y los repartidores). Esto permite que cada componente funcione de manera independiente y pueda escalar o actualizarse sin afectar a los demás.

El sistema utiliza dos formas de comunicación, RPC (Remote Procedure Call) para tareas que necesitan una respuesta inmediata, como iniciar sesión o cancelar pedidos y colas de trabajo para el procesamiento de pedidos que pueden demorar más, lo que ayuda a que el sistema sea más resistente ante posibles problemas.

Además, el proyecto organiza el proceso de un pedido en varios pasos claros (por ejemplo, desde que se recibe el pedido hasta que se entrega o cancela) y permite controlar las transiciones entre estos estados. Un aspecto destacado es que el módulo de repartidores implementa reintentos automáticos para cuando las entregas fallan, mostrando un buen manejo de fallos, algo muy importante en sistemas distribuidos.

También es notable la estrategia de pruebas, que incluye scripts diseñados para simular diferentes situaciones: desde pruebas simples de flujo completo hasta escenarios de alta carga con muchos robots y repartidores, e incluso situaciones donde se generan fallos para comprobar la robustez del sistema.

Por último, el proyecto utiliza archivos JSON para guardar información, lo cual es una solución sencilla pero efectiva para mantener el estado del sistema entre reinicios. Además, ofrece tanto una interfaz de línea de comandos como un menú interactivo, lo que facilita su uso tanto para automatización como para la interacción directa con los usuarios.