/**
 * @brief Define estructuras y datos del servidor
 * @file types.h
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 20/02/2025
 */

# ifndef TYPES_H
# define TYPES_H
  
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <stdarg.h>
#include <signal.h>

 
 #define MAX_LENGTH 256


// Estructura de la configuración del servidor
struct ServerConf {
    char signature[MAX_LENGTH];  // Nombre del servidor
    char root[MAX_LENGTH];       // Ruta raíz del servidor para archivos
    int port;                    // Puerto en el que el servidor escucha
    int maxClients;              // Número máximo de clientes permitidos simultáneamente
};

// Enumeración que representa el estado de una operación
typedef enum Status {
    OK,    // Exito
    ERROR  // Fallo
} STATUS;

 #endif
