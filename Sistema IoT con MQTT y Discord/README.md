# Práctica 3 - Sistema IoT con MQTT y Discord
### Autores: Daniel Aquino Y Enrique Gómez

Este repositorio contiene la implementación de un sistema IoT que utiliza MQTT para la comunicación entre dispositivos y un puente que integra notificaciones a Discord. Incluye simuladores de dispositivos (reloj, interruptor, sensor), un controlador con un motor de reglas separado, y un puente para Discord. Consulta INTRO.md para más detalles sobre el propósito y objetivos del proyecto.

## Estructura del Proyecto:

- **config/**: Archivos de configuración y persistencia (config.yaml, devices.json, rules.json, events.json).
- **src/**: Código fuente principal (bridge.py, controller.py, rule_engine.py, utils.py).
- **src/devices/**: Simuladores de dispositivos (dummy_clock.py, dummy_switch.py, dummy_sensor.py).
- **test/**: Pruebas unitarias (test_device.py, test_controller.py).
- **requirements.txt**: Dependencias del proyecto.
- **simulator.sh**: Script para lanzar el sistema completo.

## PRE-REQUISITOS para ejecución

### Requisitos de Software

- **Python 3.6+**: Asegúrate de tener Python instalado.
- **Mosquitto**: Broker MQTT para la comunicación entre dispositivos. Instala con:
  ```
  sudo apt update
  sudo apt install mosquitto mosquitto-clients
  ```
- **pip**: Gestor de paquetes de Python (normalmente viene con Python).
- **Dependencias**: Instala las bibliotecas necesarias con:
  ```
  pip install -r requirements.txt
  ```
  (requirements.txt incluye paho-mqtt, flask, discord.py, requests, pyyaml).

### Configuración

Asegúrate de que el directorio `config/` contenga:
- **config.yaml**: Configuración del sistema con la siguiente estructura:
  ```yaml
  mqtt:
    host: localhost
    port: 1883
    group: 2321
    pair: 6
  discord:
    token: "your_discord_bot_token"
    channel_id: 1234567890
  ```
  Reemplaza "your_discord_bot_token" con el token de tu bot de Discord y "1234567890" con el ID del canal de Discord.
- **devices.json**: Inicialmente vacío (`[]`) para almacenar el estado de los dispositivos.
- **rules.json**: Inicialmente vacío (`[]`) para almacenar las reglas.
- **events.json**: Inicialmente vacío (`[]`) para almacenar los eventos.

### Configuración del Entorno

1. Clona el repositorio:
   ```
   git clone https://git.eps.uam.es/redes2/2425/2301-2321/p06/practica3
   cd practica3
   ```
2. Instala las dependencias:
   ```
   pip install -r requirements.txt
   ```
3. Inicia el broker Mosquitto:
   ```
   mosquitto
   ```

## Pruebas

### Tests Unitarios
El directorio `test/` contiene pruebas unitarias para verificar el correcto funcionamiento de los dispositivos y el controlador.

Ejecutar pruebas:
```
python3 -m unittest test/test_device.py
python3 -m unittest test/test_controller.py
```

Contenido de las pruebas:
- **test_device.py**:
  - Verifica la conexión al broker MQTT.
  - Comprueba errores si el broker no está disponible.
  - Valida la lectura de parámetros por línea de comandos.
  - Prueba cambios de estado en interruptor y sensor.
- **test_controller.py**:
  - Confirma la conexión al broker.
  - Detecta errores si el broker falla.
  - Prueba el desencadenamiento de reglas con mensajes de sensores.
  - Verifica la ejecución de acciones y la lectura de persistencia.

### Pruebas de Integración

- Usa el script `simulator.sh` para lanzar todos los componentes y simular cambios de estado:
  ```
  chmod +x simulator.sh
  ./simulator.sh
  ```
- Observa los logs de `src/controller.py` y `src/bridge.py`, y verifica los estados en `config/devices.json` o con `!list_devices` en Discord.

## Manual de reglas / documentación de comandos

### Comandos del Bot de Discord
Interactúa con el sistema a través del bot en el canal configurado. Usa el prefijo `!` seguido de los comandos:

- **!add_device <type> <id>**: Registra un dispositivo. Ejemplo: `!add_device clock 1`
  Tipos válidos: clock, switch, sensor.
- **!delete_device <id>**: Elimina un dispositivo registrado. Ejemplo: `!delete_device 1`
- **!set_switch <id> <state>**: Cambia el estado de un interruptor. Estados válidos: ON, OFF. Ejemplo: `!set_switch 2 ON`
- **!add_rule <condition> then <action>**: Añade una regla al motor de reglas.
  Formato de condición: `if <type> <id> <operator> <value>` (e.g., `if sensor 3 > 25`).
  Formato de acción: `switch <id> <state>` (e.g., `switch 2 ON`).
  Ejemplo: `!add_rule if sensor 3 > 25 then switch 2 ON`
- **!list_devices**: Lista todos los dispositivos registrados con sus estados. Ejemplo de salida: `1 (clock): 09:01:00`
- **!list_rules**: Lista todas las reglas definidas en `config/rules.json`. Ejemplo de salida: `if sensor 3 > 25 then switch 2 ON`
- **!list_events**: Muestra los últimos 10 eventos registrados en `config/events.json`. Ejemplo de salida: `3 (sensor): 26 at timestamp`

## Especificación de comunicación con cada tipo de sensor

### Comunicación MQTT
Todos los dispositivos publican y suscriben a tópicos bajo la estructura base `redes2/2321/6/<id>`, definida en `config.yaml`. El controlador se suscribe a `redes2/2321/6/+/set` y `redes2/2321/6/+` para recibir comandos y estados.

### Reloj (src/devices/dummy_clock.py)

- **Tópico de publicación**: `redes2/2321/6/<id>` (e.g., `redes2/2321/6/1`).
- **Formato de mensaje**: Hora en formato HH:MM:SS (e.g., `09:00:00`).
- **Frecuencia**: Configurable con `--rate` (mensajes por segundo) y `--increment` (segundos entre actualizaciones).
- **Comando de ejemplo**:
  ```
  python3 src/devices/dummy_clock.py 1 --host localhost --port 1883 --time 09:00:00 --increment 60 --rate 12
  ```

### Interruptor (src/devices/dummy_switch.py)

- **Tópico de publicación**: `redes2/2321/6/<id>` (e.g., `redes2/2321/6/2`).
- **Tópico de suscripción**: `redes2/2321/6/<id>/set` (e.g., `redes2/2321/6/2/set`) para recibir comandos.
- **Formato de mensaje**: ON o OFF.
- **Probabilidad de fallo**: Configurable con `--probability` (default 0.3).
- **Comando de ejemplo**:
  ```
  python3 src/devices/dummy_switch.py 2 --host localhost --port 1883 --probability 0.3
  ```

### Sensor (src/devices/dummy_sensor.py)

- **Tópico de publicación**: `redes2/2321/6/<id>` (e.g., `redes2/2321/6/3`).
- **Formato de mensaje**: Valor numérico entre `--min` y `--max` (e.g., 20 to 30).
- **Frecuencia**: Configurable con `--interval` (segundos entre cambios).
- **Comando de ejemplo**:
  ```
  python3 src/devices/dummy_sensor.py 3 --host localhost --port 1883 --interval 5 --min 20 --max 30 --increment 1
  ```

## Notas Adicionales

- Asegúrate de que los dispositivos estén registrados con `!add_device` antes de ejecutar los simuladores.
- El puente (`src/bridge.py`) envía notificaciones a Discord para cada evento, con un límite de una notificación cada 2 segundos para evitar restricciones de la API.
- El motor de reglas (`src/rule_engine.py`) gestiona las reglas definidas en `config/rules.json`.
- Consulta los logs de `src/controller.py` y `src/bridge.py` para depurar problemas.
- Cabe mencionar que tras la realización de una serie de test, hemos deducido que el mejor valor para calcular el tiempo de espera entre actualización y actualización del valor del `dummy_clock.py` deberia ser `>= 25/self.rate` ya que de esta manera no hay problemas con los limites de peticiones realizadas al servidor externo de discord. Cabe mencionar que las limitaciones del sistema con el cual se ejecute el software también influirá en esto.