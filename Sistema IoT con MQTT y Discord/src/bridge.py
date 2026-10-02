from flask import Flask, request
import discord
from discord.ext import commands
import yaml
import requests
import asyncio
import threading
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

app = Flask(__name__)
with open("config/config.yaml", "r") as f:
    config = yaml.safe_load(f)

intents = discord.Intents.default()
intents.message_content = True
bot = commands.Bot(command_prefix="!", intents=intents)

loop = asyncio.new_event_loop()
asyncio.set_event_loop(loop)


@bot.event
async def on_ready():
    """
    Evento que se dispara cuando el bot de Discord está listo y conectado.
    
    Imprime un mensaje de confirmación con el nombre del usuario del bot.
    """

    print(f"Logged in as {bot.user}")


async def send_discord_message(channel_id, message):
    """
    Envía un mensaje a un canal específico de Discord.
    
    Args:
        channel_id (str): ID del canal de Discord donde se enviará el mensaje.
        message (str): Contenido del mensaje a enviar.
        
    Returns:
        None
    """
    channel = bot.get_channel(int(channel_id))
    if channel:
        await channel.send(message)
    else:
        print("Channel not found. Check channel ID and bot permissions.")


@app.route('/')
def home():
    """
    Ruta raíz del servidor Flask.
    
    Returns:
        str: Mensaje de confirmación de que el servidor Flask está funcionando.
    """
    return "Servidor Flask funcionando correctamente"


@app.route("/event", methods=["POST"])
def handle_event():
    """
    Manejador de eventos recibidos vía HTTP POST.
    
    Procesa los eventos de cambio de estado de los dispositivos y envía
    notificaciones al canal de Discord configurado.
    
    Returns:
        tuple: Respuesta vacía con código HTTP 200 (OK).
    """
    data = request.json
    device_id = data['device_id']
    state = data['state']
    print(f"Received event for device: {device_id}, state: {state}")

    if state.startswith("Rule triggered"):
        msg = f"{state.replace('device', f'device {device_id.split('/')[-1]}')}"
    else:
        msg = f"Device {device_id.split('/')[-1]} updated to {state}"

    loop.create_task(send_discord_message(config["discord"]["channel_id"], msg))
    return "", 200



@bot.command()
async def add_device(ctx, device_type: str, device_id: str):
    """
    Comando de Discord para añadir un nuevo dispositivo.
    
    Args:
        ctx: Contexto del comando de Discord.
        device_type (str): Tipo de dispositivo a añadir ("switch", "sensor", "clock").
        device_id (str): Identificador único para el nuevo dispositivo.
        
    Returns:
        None: Envía un mensaje de respuesta al canal de Discord.
    """
    if device_type not in ["switch", "sensor", "clock"]:
        await ctx.send("Invalid device type (switch, sensor, clock)")
        return
    full_id = f"redes2/{config['mqtt']['group']}/{config['mqtt']['pair']}/{device_id}"
    response = requests.post("http://localhost:8081/add_device", json={
        "device_id": full_id,
        "device_type": device_type
    })
    if response.status_code == 200:
        await ctx.send(f"Device {device_id} added")
    else:
        try:
            error_msg = response.json().get("error", "Failed to add device.")
        except ValueError:
            error_msg = "Failed to add device."
        await ctx.send(error_msg)


@bot.command()
async def delete_device(ctx, device_id: str):
    """
    Comando de Discord para eliminar un dispositivo existente.
    
    Args:
        ctx: Contexto del comando de Discord.
        device_id (str): Identificador del dispositivo a eliminar.
        
    Returns:
        None: Envía un mensaje de respuesta al canal de Discord.
    """
    full_id = f"redes2/{config['mqtt']['group']}/{config['mqtt']['pair']}/{device_id}"
    response = requests.post("http://localhost:8081/delete_device", json={
        "device_id": full_id
    })
    if response.status_code == 200:
        await ctx.send(f"Device {device_id} deleted")
    else:
        try:
            error_msg = response.json().get("error", "Failed to delete device.")
        except ValueError:
            error_msg = "Failed to delete device."
        await ctx.send(error_msg)

@bot.command()
async def set_switch(ctx, device_id: str, state: str):
    """
    Comando de Discord para cambiar el estado de un interruptor.
    
    Args:
        ctx: Contexto del comando de Discord.
        device_id (str): Identificador del interruptor a controlar.
        state (str): Nuevo estado para el interruptor ("ON" u "OFF").
        
    Returns:
        None: Envía un mensaje de respuesta al canal de Discord.
    """
    if state not in ["ON", "OFF"]:
        await ctx.send("Invalid state (ON, OFF)")
        return
    full_id = f"redes2/{config['mqtt']['group']}/{config['mqtt']['pair']}/{device_id}"
    response = requests.post("http://localhost:8081/set_switch", json={
        "device_id": full_id,
        "state": state
    })
    if response.status_code == 200:
        await ctx.send(f"Switch {device_id} set to {state}")
    else:
        await ctx.send(f"Failed to set switch {device_id}")

@bot.command()
async def add_rule(ctx, *, rule: str):
    """
    Comando de Discord para añadir una nueva regla de automatización.
    
    Args:
        ctx: Contexto del comando de Discord.
        rule (str): Regla en formato "if <condition> then <action>".
        
    Returns:
        None: Envía un mensaje de respuesta al canal de Discord.
    """
    try:
        condition, action = rule.split("then")
        condition = condition.strip()
        action = action.strip()
        response = requests.post("http://localhost:8081/add_rule", json={
            "condition": condition,
            "action": action
        })
        if response.status_code == 200:
            await ctx.send("Rule added")
        else:
            await ctx.send("Failed to add rule")
    except ValueError:
        await ctx.send("Invalid rule format. Use: if <condition> then <action>")

@bot.command()
async def list_devices(ctx):
    """
    Comando de Discord para listar todos los dispositivos registrados.
    
    Args:
        ctx: Contexto del comando de Discord.
        
    Returns:
        None: Envía un mensaje con la lista de dispositivos al canal de Discord.
    """
    response = requests.get("http://localhost:8081/devices")
    if response.status_code == 200:
        devices = response.json()
        if devices:
            msg = "\n".join([f"{d['id'].split('/')[-1]} ({d['type']}): {d['state']}" for d in devices])
        else:
            msg = "No devices registered"
        await ctx.send(msg)
    else:
        await ctx.send("Failed to fetch devices")


@bot.command()
async def list_rules(ctx):
    """
    Comando de Discord para listar todas las reglas de automatización.
    
    Args:
        ctx: Contexto del comando de Discord.
        
    Returns:
        None: Envía un mensaje con la lista de reglas al canal de Discord.
    """
    response = requests.get("http://localhost:8081/rules")
    if response.status_code == 200:
        rules = response.json()
        if rules:
            msg = "\n".join([f"{r['condition']} then {r['action']}" for r in rules])
        else:
            msg = "No rules defined"
        await ctx.send(msg)
    else:
        await ctx.send("Failed to fetch rules")


@bot.command()
async def list_events(ctx):
    """
    Comando de Discord para listar los últimos eventos registrados.
    
    Args:
        ctx: Contexto del comando de Discord.
        
    Returns:
        None: Envía un mensaje con los últimos 10 eventos al canal de Discord.
    """
    response = requests.get("http://localhost:8081/events")
    if response.status_code == 200:
        events = response.json()
        if events:
            msg = "\n".join([f"{e['device_id'].split('/')[-1]}: {e['state']} at {e['timestamp']}" for e in events[-10:]])
        else:
            msg = "No events recorded"
        await ctx.send(msg)
    else:
        await ctx.send("Failed to fetch events")


def run_discord_bot():
    """
    Función para iniciar el bot de Discord en un bucle de eventos.
    
    Esta función se ejecuta en un hilo separado y mantiene el bot
    en funcionamiento hasta que se cierre la aplicación.
    
    Returns:
        None
    """
    loop.run_until_complete(bot.start(config["discord"]["token"]))

if __name__ == "__main__":
    threading.Thread(target=run_discord_bot, daemon=True).start()
    app.run(port=5000)
