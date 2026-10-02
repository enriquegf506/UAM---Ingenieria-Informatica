# **1. Resumen - Opción A: Django**

El objetivo es desarrollar un **sistema domótico** para el hogar, utilizando dispositivos IoT (interruptores, sensores, relojes) que se comunican mediante el protocolo MQTT, con el broker Mosquitto.

La **Opción A** propone crear un **proyecto web en Django** para gestionar el sistema. Este incluirá:

- **Gestión de dispositivos**: Alta, baja y modificación de dispositivos IoT.
- **Gestión de reglas**: Definir comportamientos automatizados (ej. encender luces al anochecer).
- **Consulta de eventos generados** por los dispositivos.

## Componentes Clave:
- **Dispositivos IoT**: Publican su estado o reciben comandos a través de MQTT.
- **Broker MQTT (Mosquitto)**: Intermediario de comunicación entre dispositivos y controlador.
- **Controlador**: Escucha al broker, gestiona eventos y activa reglas.
- **Rule Engine**: Evalúa reglas ante eventos y decide acciones.
- **Persistencia**: Se usará la base de datos de Django (por defecto, SQLite) para almacenar dispositivos, reglas y eventos.
- **Interfaz web (Django)**: Permite a los usuarios administrar dispositivos, reglas y visualizar eventos.

## Consideraciones técnicas:
- No es necesario implementar autenticación ni diseño CSS avanzado.
- Se deben usar topics MQTT con la estructura: `redes2/GRUPO/PAREJA/ID`.
- Se recomienda una estructura modular, reutilizando código y encapsulando funciones comunes (conexión al broker, mensajes, etc.).
- La implementación debe seguir buenas prácticas de prueba y documentación.

---

Aquí tienes un resumen claro y estructurado de la **sección 3: Funcionalidad**, adaptado a la **opción A (Django)**, que puede ayudarte también a completar tus secciones de requisitos, casos de uso y decisiones de diseño:

---

# **2. Funcionalidad (Resumen - Opción A: Django)**

## **1. Gestión de Dispositivos IoT**
- Se pueden **añadir, editar y borrar** dispositivos desde la aplicación web.
- Cada dispositivo tendrá un **ID único** y un **tipo** (sensor, interruptor, reloj).
- Es posible **registrar un dispositivo externamente** mediante un fichero, que luego debe ser notificado al sistema para integrarlo.
- Solo los dispositivos **registrados** serán procesados.

## **2. Gestión de Reglas Básicas**
- Las reglas siguen el formato **Dado-Cuando-Entonces**, usando operadores: `==`, `>`, `<`.
  - Ejemplo textual: `"si sala > 25 entonces enciende caldera"`.
- Estas reglas se crean, editan y eliminan desde el panel Django.
- Las reglas se almacenan como texto estructurado y el sistema debe ser capaz de interpretarlas.

## **3. Registro y funcionamiento del dispositivo**
Un dispositivo puede:
- **Publicar su estado** en un topic MQTT.
- **Recibir comandos** si el tipo lo permite (como un interruptor).
- **Conectarse al broker MQTT**.
- **Generar eventos internos** que son procesados por el sistema.
- **Disparar acciones** si alguna regla se cumple.
- **Modificar el estado** de otros dispositivos (ej. encender otro interruptor).

## **4. Interfaz Django**
- Consultar, añadir, editar y eliminar:
  - **Dispositivos**
  - **Reglas**
  - **Eventos generados**

## **Limitaciones del sistema**
- Los dispositivos **no almacenan su estado**: al iniciar, eligen uno y lo publican.
- Los **eventos son efímeros**: si el sistema cae, no se recuperan los eventos anteriores al reinicio.


## **Requisitos (a completar por el alumno)**

Podrías definirlos así:
- El sistema debe permitir registrar y gestionar dispositivos IoT desde Django.
- Las reglas deben poder definirse en texto estructurado y soportar comparaciones simples.
- El sistema debe rechazar mensajes de dispositivos no registrados.
- Se debe mantener persistencia de reglas, dispositivos y eventos usando modelos Django.
- El controlador debe suscribirse y publicar en el broker MQTT.
- El Rule Engine debe comprobar las reglas ante eventos y disparar acciones si corresponden.

---

## **Casos de Uso (a completar por el alumno)**

1. **Registrar un dispositivo**
2. **Editar el tipo o identificador de un dispositivo**
3. **Eliminar un dispositivo**
4. **Registrar una nueva regla**
5. **Editar una regla existente**
6. **Eliminar una regla**
7. **Consultar historial de eventos**
8. **Procesar evento entrante desde MQTT y ejecutar regla**
9. **Ejecutar acción sobre dispositivo desde una regla**

---

## **Decisiones de diseño (ideas para completar)**

- **¿Controller y Rule engine deben estar separados?**
  - **Sí.** Facilita la modularidad y el testing. Permite escalar cada componente por separado.
  - **No.** Si el proyecto es pequeño y no se requiere concurrencia avanzada, se pueden unir para reducir complejidad.

- **¿Cómo se comunican Controller y Rule engine?**
  - Pueden comunicarse internamente como funciones dentro del mismo servicio o mediante colas/eventos si están separados.

- **¿Qué componentes podrían compartir funcionalidad?**
  - Controller y Rule engine pueden compartir acceso a los modelos de Django para consultar reglas, dispositivos y registrar eventos.

- **¿Cuántas instancias hay de cada componente?**
  - En local: probablemente una sola. En producción: el Controller podría escalar en múltiples instancias.

- **¿Controller y Bridge deben ser separados?**
  - En opción A no hay Bridge, así que no aplica.

---
Perfecto. Aquí tienes un **borrador organizado y ampliado de los entregables**, especialmente útil si estás optando por la **opción A (proyecto Django)**, y quieres tenerlo todo bien documentado y justificado para la entrega:

---

# **4. Entregables**

---

## **Documentación**

### **A. Partes completadas en este documento**

- **Decisiones de diseño** (ver sección anterior): se han razonado las ventajas de separar `controller.py`, `rule-engine.py` y `bridge.py`, y se definió cómo se comunican entre sí (ej. mediante llamadas internas o colas de mensajes).
  
### **B. Especificación de comunicación con cada tipo de sensor**

| Tipo de dispositivo | Formato de mensaje MQTT | Topic | Notas |
|---------------------|--------------------------|-------|-------|
| Sensor de temperatura | `{ "id": "sensor1", "value": 28.3 }` | `devices/sensor1/state` | Publica cada intervalo |
| Interruptor (switch) | `{ "id": "switch1", "status": "on" }` | `devices/switch1/action` | Recibe acciones del Controller |
| Reloj (clock) | `{ "id": "clock1", "time": "09:30:15" }` | `devices/clock1/state` | Publica tiempo cada segundo |

### **C. Esquema de datos (modelo Django)**

```python
class Device(models.Model):
    id = models.CharField(primary_key=True, max_length=50)
    device_type = models.CharField(max_length=20)  # sensor, switch, clock

class Rule(models.Model):
    name = models.CharField(max_length=100)
    condition = models.TextField()  # Ej: "sala > 25"
    action = models.TextField()     # Ej: "enciende caldera"

class Event(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    device_id = models.CharField(max_length=50)
    value = models.TextField()
```

### **D. Clases principales y relaciones**

- `Device`: clase base, se pueden derivar `Sensor`, `Switch`, `Clock` si se quiere tipado explícito.
- `Rule`: almacena condiciones tipo `si X > Y entonces Z`.
- `Event`: registra cualquier mensaje procesado del broker.

Relaciones:
- Un `Event` se asocia a un `Device` por ID.
- Las reglas se evalúan usando los valores de `Event`.

### **E. Limitaciones de la solución**

- **Persistencia parcial**: los dispositivos no almacenan estado propio, sólo el último evento es persistido.
- **Eventos efímeros**: si el rule engine está caído al llegar un evento, se pierde.
- **Dependencia del broker MQTT**: si se cae, todo el sistema se ve afectado.
- **Discord (opcional)**: si no hay conectividad con Discord, no se publican acciones ni notificaciones externas.

---

## **Código**

### **dummy-switch.py**
- Instancia un interruptor, simula errores con una probabilidad.
- Espera comandos por MQTT y responde cambiando su estado (si no falla).

### **dummy-sensor.py**
- Publica valores numéricos periódicamente.
- Simula comportamiento de un sensor con valores aleatorios entre min y max.

### **dummy-clock.py**
- Publica marcas de tiempo en intervalos regulares desde una hora inicial.

---

## **controller.py**
- Conexión con el broker MQTT.
- Suscripción a todos los topics de dispositivos registrados.
- Procesamiento de eventos → envío a Rule Engine.
- Acciones generadas → publicación MQTT al dispositivo correspondiente.

---

## **rule-engine.py**
- Evalúa reglas cargadas desde la base de datos.
- Si se cumple una condición, genera una acción textual.
- Puede residir en un script separado o estar embebido en el controller.

---

## **tests**

### `test_device.py`
- ✅ Conecta correctamente con el broker MQTT.
- ✅ Lanza error si no puede conectar.
- ✅ Cambia de estado al recibir acción (interruptor).
- ✅ Envía valores dentro del rango (sensor).
- ✅ Recoge bien los argumentos desde CLI.

### `test_controller.py`
- ✅ Conecta y suscribe al broker.
- ✅ Lanza error si el broker no responde.
- ✅ Procesa mensajes entrantes, delega al Rule Engine.
- ✅ Aplica acciones generadas por Rule Engine.
- ✅ Carga la configuración desde la base de datos (devices, reglas).

---

## **simulator.sh**
Ejemplo de script:

```bash
#!/bin/bash
python dummy-sensor.py --min 22 --max 28 --increment 1 --interval 2 1 &
python dummy-switch.py --probability 0.2 2 &
python controller.py --host redes2.ii.uam.es --port 1883 --database iot.db
```

---

## **Configuración**

- `.env` para tokens (Discord), contraseña API, configuración de SQLite.
- `settings.py` con conexión MQTT por defecto.
- SQLite con el esquema creado (opcional: sin datos).
