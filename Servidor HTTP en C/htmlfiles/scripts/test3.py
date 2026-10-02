import sys
import signal
TIMEOUT = 1  # seconds
signal.signal(signal.SIGALRM, lambda signum, frame: print("¡Tiempo agotado!") or sys.exit(0))
signal.alarm(TIMEOUT)

def celsius_a_fahrenheit(celsius):
    return (celsius * 9/5) + 32

print("Inicio")
print("Script Python para peticiones POST\n")

print("Recibido por STDIN: ")
try:
            celsius = float(sys.stdin[1])
            fahrenheit = celsius_a_fahrenheit(celsius)
            print(f"\n{celsius} grados Celsius son {fahrenheit} grados Fahrenheit.")
except:
    print("No se recibió entrada por STDIN")

print("Fin de datos")

print("\n\nRecibido por ARGV:")
try:
    celsius = float(sys.argv[1])
    fahrenheit = celsius_a_fahrenheit(celsius)
    print(f"\n{celsius} grados Celsius son {fahrenheit} grados Fahrenheit.")
except:
    print("No se recibió entrada por ARGV")

print("Fin de datos")

print("\n\nFin del script")