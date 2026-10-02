import sys
import signal

# Configuración de la alarma
TIMEOUT = 1  # seconds
signal.signal(signal.SIGALRM, lambda signum, frame: print("¡Tiempo agotado!") or sys.exit(0))
signal.alarm(TIMEOUT)

print("Inicio")
print("Script Python para peticiones GET\n")


print("\n\nRecibido por STDIN: ")
try:
    if(sys.stdin[1]):
        print(f"\nHola {sys.stdin[1]}")
    else:
        print("No se recibió entrada por STDIN")
except:
    ignorar = True

print("Fin de datos")

print("\n\nRecibido por ARGV: ")
try:
    if(sys.argv[1]):
        print(f"\nHola {sys.argv[1]}")
    else:
        print("No se recibió entrada por ARGV")
except:
    ignorar = True

print("Fin de datos")

print("\n\nFin del script")