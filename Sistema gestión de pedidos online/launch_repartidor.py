import time
import uuid
from src.repartidor import Repartidor

def iniciar_repartidores(num_repartidores):
    """Inicia múltiples repartidores en hilos separados."""
    repartidores = []
    for i in range(num_repartidores):
        repartidor_id = f"Repartidor-{uuid.uuid4().hex[:8]}"
        repartidor = Repartidor(repartidor_id)
        repartidor.run()
        repartidores.append(repartidor)
        print(f"Repartidor {repartidor_id} iniciado.")
    return repartidores

if __name__ == "__main__":
    NUM_REPARTIDORES = 1  # Número de repartidores a iniciar
    try:
        iniciar_repartidores(NUM_REPARTIDORES)
        print(f"{NUM_REPARTIDORES} repartidores iniciados. Presiona Ctrl+C para detener.")
        while True:
            time.sleep(1)  # Mantener el programa corriendo
    except KeyboardInterrupt:
        print("Deteniendo repartidores...")