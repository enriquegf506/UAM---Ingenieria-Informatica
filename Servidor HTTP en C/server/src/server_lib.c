/**
 * @brief Funcionalidad de todo lo que necesita el servidor
 * @file server_lib.c
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 23/02/2025
 */

#include "../lib/server_lib.h"

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
int Tcp_listen(const char *host, const char *port, socklen_t *addrlen)
{
    int server_fd = 0;
    struct sockaddr_in address;
    int opt = 1;  // Opción para activar SO_REUSEADDR (permitir reutilizar la dirección)
    int base_port = atoi(port);  // Convertir el puerto recibido a un entero
    int current_port = base_port;  // Puerto que vamos a usar actualmente
    int max_retries = 10;  // Número máximo de intentos de conexión con puertos consecutivos
    int retries = 0;

    // Verificación de los argumentos
    if (!host || !port || !addrlen) exit(EXIT_FAILURE);

    // Inicializa la estructura para la dirección del socket
    *addrlen = sizeof(address);
    memset(&address, 0, sizeof(address));

    // Intenta varias veces con puertos diferentes si el puerto actual está en uso
    while (retries < max_retries)
    {
        // Crear el socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // opción SO_REUSEADDR para reutilizar direcciones
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            perror("setsockopt(SO_REUSEADDR) failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // dirección IP y el puerto
        address.sin_family = AF_INET;
        if (inet_pton(AF_INET, host, &address.sin_addr) <= 0)
        {
            perror("Dirección IP no válida");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        address.sin_port = htons(current_port);

        // Intentar vincular (bind) el socket al puerto
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            // Si el puerto está en uso, intenta con otro puerto
            if (errno == EADDRINUSE)
            {
                printf("Puerto %d en uso. Intentando con otro puerto...\n", current_port);
                close(server_fd);
                current_port++;  // aumentamos el puerto y el intento
                retries++;
                continue;
            }
            else
            {
                // Si ocurre un error distinto, salir
                writeLog(ERR, "Bind fallido");
                perror("bind failed");
                close(server_fd);
                exit(EXIT_FAILURE);
            }
        }

        // Si bind es correcto, salir del bucle
        break;
    }

    // Si no se pudo encontrar un puerto disponible después de varios intentos finaliza
    if (retries == max_retries)
    {
        writeLog(ERR,  "No se pudo encontrar un puerto disponible después de 10 intentos.\n");
        perror("No se pudo encontrar un puerto disponible después de 10 intentos.\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        writeLog(ERR, "listen failure");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    writeLog(INFO, "Servidor escuchando...");
    return server_fd;
}

/********
 * FUNCIÓN: sem_t *my_lock_init(void)  
 * ARGS_IN: Ninguno.
 * DESCRIPCIÓN: Inicializa un semáforo para el control de acceso en secciones críticas.
 * ARGS_OUT: sem_t * - Puntero al semáforo creado.
 * 
 ********/
sem_t *my_lock_init(void) {
    // Eliminar cualquier semáforo existente con el mismo nombre
    sem_unlink(SEM_NAME);
    // Crear el semáforo con un valor inicial de 1
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        sem_unlink(SEM_NAME);  // Limpiar si falla la creación
        return NULL;
    }
    return sem;
}

/********
 * FUNCIÓN: int my_lock_wait(sem_t *sem)  
 * ARGS_IN: sem_t *sem - El semáforo que se desea bloquear.
 * DESCRIPCIÓN: Bloquea el semáforo, haciendo que el proceso espere hasta que esté disponible.
 * ARGS_OUT: int - devuelve 0 si la operación fue exitosa, o -1 en caso de error.
 * 
 ********/
int my_lock_wait(sem_t *sem) {
    if (!sem) return -1;  // Verifica si el semáforo es válido
    return sem_wait(sem);  // Espera a que el semáforo esté disponible
}

/********
 * FUNCIÓN: void my_lock_release(sem_t *sem)  
 * ARGS_IN: sem_t *sem - El semáforo que se desea liberar.
 * DESCRIPCIÓN: Libera el semáforo, permitiendo que otros procesos accedan a la sección crítica.
 * ARGS_OUT: void - No devuelve nada.
********/
void my_lock_release(sem_t *sem) {
    if (sem) sem_post(sem);  // Libera el semáforo 
}

/********
 * FUNCIÓN: void process_request(int connfd)  
 * ARGS_IN: int connfd - El descriptor de archivo de la conexión del cliente.
 * DESCRIPCIÓN: Recibe una solicitud del cliente, la muestra en consola y envía una respuesta.
 * ARGS_OUT: void - Nada.
 * 
 ********/
void process_request(int connfd)
{
    char buffer[BUFFER_SIZE];
    const char *response = "message received";  // Respuesta estándar para el cliente

    ssize_t bytes;
    
    memset(buffer, 0, BUFFER_SIZE);  // Inicializar el buffer
    bytes = recv(connfd, buffer, sizeof(buffer) - 1, 0);  // Recibe datos del cliente

    if (bytes <= 0) {
        perror("Error en recv");
        return;
    } else if(bytes == 0)
    {
        return;
    }

    buffer[bytes] = '\0'; 

    // Envia la respuesta
    if (send(connfd, response, strlen(response), 0) <= 0) {
        perror("Error en send"); 
        return;
    }

    return;
}

/********
 * FUNCIÓN: int Accept(int fd, struct sockaddr *cliaddr, socklen_t *addrlen)  
 * ARGS_IN: int fd - El descriptor de archivo del socket de escucha.
 *          struct sockaddr *cliaddr - Puntero a la estructura que almacena la dirección del cliente.
 *          socklen_t *addrlen - Puntero al tamaño de la estructura `cliaddr`.
 * DESCRIPCIÓN: Acepta una nueva conexión entrante y devuelve el descriptor de archivo de la conexión.
 * ARGS_OUT: int - El descriptor de archivo de la conexión.
********/
int Accept(int fd, struct sockaddr *cliaddr, socklen_t *addrlen) {
    int ret = accept(fd, cliaddr, addrlen);  // Aceptar la conexión
    if (ret < 0) {
        perror("accept error"); 
    }
    return ret;
}

/********
 * FUNCIÓN: void Close(int connectionfd)  
 * ARGS_IN: int connectionfd - El descriptor de archivo de la conexión.
 * DESCRIPCIÓN: Cierra la conexión con el cliente, liberando los recursos del socket.
 * ARGS_OUT: void - No devuelve nada.
 * 
 ********/
void Close(int connectionfd) {
    if (connectionfd >= 0) {
        close(connectionfd);
    }
}

/********
 * FUNCIÓN: void my_lock_destroy(sem_t *sem)  
 * ARGS_IN: sem_t *sem - El semáforo a destruir.
 * DESCRIPCIÓN: Cierra y elimina el semáforo, liberando los recursos asociados.
 * ARGS_OUT: void - No devuelve nada.
 ********/
void my_lock_destroy(sem_t *sem) {
    if (sem) {
        sem_close(sem);  // Cerrar
        sem_unlink(SEM_NAME);  // Eliminar
    }
}
