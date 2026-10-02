import time
import uuid
from src.robot import Robot

def iniciar_robots(num_robots):
    """Inicia múltiples robots en hilos separados."""
    robots = []
    for i in range(num_robots):
        robot_id = f"Robot-{uuid.uuid4().hex[:8]}"
        robot = Robot(robot_id)
        robot.run()
        robots.append(robot)
        print(f"Robot {robot_id} iniciado.")
    return robots

if __name__ == "__main__":
    NUM_ROBOTS = 1  
    try:
        iniciar_robots(NUM_ROBOTS)
        print(f"{NUM_ROBOTS} robots iniciados. Presiona Ctrl+C para detener.")
        while True:
            time.sleep(1)  
    except KeyboardInterrupt:
        print("Deteniendo robots...")