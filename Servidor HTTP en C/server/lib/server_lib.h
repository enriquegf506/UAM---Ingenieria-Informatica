/**
 * @brief Define métodos, variables globales y librerias utilizadas para maneko de sockets
 * @file server_lib.h
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 20/02/2025
 */

#ifndef SERVER_LIB_H
#define SERVER_LIB_H

#include "http_manage.h"
#include "types.h"

// Definición de variables globales
#define SEM_NAME "/my_semaphore"    // Nombre del semáforo 
#define BUFFER_SIZE 1024            // Támaño máximo de cadena

/********
 * FUNCIÓN: int Tcp_listen(const char *host, const char *port, socklen_t *addrlen)  
 * ARGS_IN: const char *host - Dirección IP del servidor (por ejemplo, "127.0.0.1").
 *          const char *port - Puerto en el que el servidor intentará escuchar.
 *          socklen_t *addrlen - Puntero a la variable donde se almacenará el tamaño de la dirección.
 * DESCRIPCIÓN: Configura y pone en marcha el servidor para escuchar las conexiones entrantes. Si el puerto 
 *              especificado está en uso, intenta con otros puertos consecutivos hasta un máximo de 10 intentos.
 * ARGS_OUT: int - Descriptor de archivo del socket del servidor.
 * 
 ********/
int     Tcp_listen(const char *host, const char *port, socklen_t *addrlen);
/********
 * FUNCIÓN: sem_t *my_lock_init(void)  
 * ARGS_IN: Ninguno.
 * DESCRIPCIÓN: Inicializa un semáforo para el control de acceso en secciones críticas.
 * ARGS_OUT: sem_t * - Puntero al semáforo creado.
 * 
 ********/
sem_t   *my_lock_init(void);
/********
 * FUNCIÓN: int my_lock_wait(sem_t *sem)  
 * ARGS_IN: sem_t *sem - El semáforo que se desea bloquear.
 * DESCRIPCIÓN: Bloquea el semáforo, haciendo que el proceso espere hasta que esté disponible.
 * ARGS_OUT: int - devuelve 0 si la operación fue exitosa, o -1 en caso de error.
 * 
 ********/
int    my_lock_wait(sem_t *sem);
/********
 * FUNCIÓN: void my_lock_release(sem_t *sem)  
 * ARGS_IN: sem_t *sem - El semáforo que se desea liberar.
 * DESCRIPCIÓN: Libera el semáforo, permitiendo que otros procesos accedan a la sección crítica.
 * ARGS_OUT: void - No devuelve nada.
********/
void    my_lock_release(sem_t *sem);
/********
 * FUNCIÓN: void my_lock_destroy(sem_t *sem)  
 * ARGS_IN: sem_t *sem - El semáforo a destruir.
 * DESCRIPCIÓN: Cierra y elimina el semáforo, liberando los recursos asociados.
 * ARGS_OUT: void - No devuelve nada.
 ********/
void    my_lock_destroy(sem_t *sem);
/********
 * FUNCIÓN: void process_request(int connfd)  
 * ARGS_IN: int connfd - El descriptor de archivo de la conexión del cliente.
 * DESCRIPCIÓN: Recibe una solicitud del cliente, la muestra en consola y envía una respuesta.
 * ARGS_OUT: void - Nada.
 * 
 ********/
void    process_request(int connectionfd);

/********
 * FUNCIÓN: int Accept(int fd, struct sockaddr *cliaddr, socklen_t *addrlen)  
 * ARGS_IN: int fd - El descriptor de archivo del socket de escucha.
 *          struct sockaddr *cliaddr - Puntero a la estructura que almacena la dirección del cliente.
 *          socklen_t *addrlen - Puntero al tamaño de la estructura `cliaddr`.
 * DESCRIPCIÓN: Acepta una nueva conexión entrante y devuelve el descriptor de archivo de la conexión.
 * ARGS_OUT: int - El descriptor de archivo de la conexión.
********/
int     Accept(int listenfd, struct sockaddr *cliaddr ,socklen_t *addrlen);
/********
 * FUNCIÓN: void Close(int connectionfd)  
 * ARGS_IN: int connectionfd - El descriptor de archivo de la conexión.
 * DESCRIPCIÓN: Cierra la conexión con el cliente, liberando los recursos del socket.
 * ARGS_OUT: void - No devuelve nada.
 * 
 ********/
void    Close(int connectionfd);

#endif
