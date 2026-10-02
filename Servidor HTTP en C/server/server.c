/**
 * @brief Funcionalidad del servidor
 * @file server.c
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 16/02/2025
 */

#include "lib/server_lib.h"
#include "lib/types.h"
#include "lib/http_manage.h"
#include "lib/log_manage.h"

#define PORT_LEN 6      // Tamaño de buffer del puerto de escucha


int listenfd = -1;                  // Descriptor de archivo para el socket de escucha
socklen_t addrlen;                  // Tamaño de la estructura de dirección
sem_t *sem = NULL;                  // Semáforo para sincronización de hilos
struct ServerConf conf;             // Estructura para la configuración del servidor
volatile sig_atomic_t sigint = 0;   // Flag para indicar si se recibió señal SIGINT
pthread_t *threads = NULL;          // Array de hilos
int num_threads_created = 0;        // Contador de hilos creados exitosamente


/********
 * FUNCIÓN: void cleanup_resources()
 * DESCRIPCIÓN: Libera todos los recursos del servidor de manera segura
 * ARGS_OUT: Ninguno (void)
 ********/
void cleanup_resources(void) {
    // Libera la memoria asignada para los hilos
    if (threads) {
        free(threads);
        threads = NULL;
    }

    // Destruye el semáforo 
    if (sem) {
        my_lock_destroy(sem);
        sem = NULL;
    }
    
    // Cierra el socket
    if (listenfd >= 0) {
        Close(listenfd);
        listenfd = -1;
    }
    
    //Cierra el log
    closeLog();
}

/********
 * FUNCIÓN: void handle_sigint()
 * DESCRIPCIÓN: Maneja la señal SIGINT de manera segura
 ********/
void handle_sigint() {
    sigint = 1;
    writeLog(INFO, "Recibido SIGINT (Ctrl + C). Cerrando servidor...");
    // Cleanup será manejado en main
}

/********
 * FUNCIÓN: STATUS read_config()   
 * ARGS_IN: struct ServerConf *config - Guarda la configuración del servidor.
 * DESCRIPCIÓN: Se enncarga de leer la configuración por fichero y guardar
 *              los parametros en la estructura ServerConf
 * ARGS_OUT: ERROR - en caso de fallo, OK - en caso de que salga bien.
 ********/
STATUS read_config(struct ServerConf *config) {
    // Abre el archivo de configuración
    FILE *file = fopen("server.conf", "r");
    if (!file) {
        writeLog(ERR, "Error al abrir server.conf");
        return ERROR;
    }

    char line[MAX_LENGTH];
    char *key, *value;

    // Lee el archivo línea por línea y guarda los valores pertinetes en la estructura de configuración del servidor
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        key = strtok(line, "=");
        if (key == NULL) continue;
        
        int len = strlen(key);
        while (len > 0 && (key[len-1] == ' ' || key[len-1] == '\t')) {
            key[--len] = '\0';
        }

        value = strtok(NULL, "\n");
        if (value == NULL) continue;
        
        while (*value == ' ' || *value == '\t') value++;

        if (strcmp(key, "server_root") == 0) {
            strcpy(config->root, value);
            if (strcmp(config->root, "") == 0){
                writeLog(ERR, "Error: debe asignarle una ruta al servidor");
                return ERROR;
            }
        } 
        else if (strcmp(key, "max_clients") == 0) {
            config->maxClients = atoi(value);

            if (config->maxClients <= 0 || config->maxClients > 1000) {
                writeLog(ERR, "Error: max_clients debe estar entre 1 y 1000");
                return ERROR;
            }
        } 
        else if (strcmp(key, "listen_port") == 0) {
            config->port = atoi(value);
            
            if (config->port <= 0 || config->port > 65535) {
                writeLog(ERR, "Error: listen_port debe estar entre 1 y 65535");
                return ERROR;
            }
            
        } 
        else if (strcmp(key, "server_signature") == 0) {
            strcpy(config->signature, value);

            if(strcmp(config->signature, "") == 0) {
                writeLog(ERR, "Error: debe asignarle un nombre al servidor");
                return ERROR;
            }
        }
    }

    fclose(file);
    return OK;
}





/********
 * FUNCIÓN: void *thread_main()   
 * ARGS_IN: struct ServerConf *config - Guarda la configuración del servidor.
 * DESCRIPCIÓN: Funcion que ejecuta cada hilo del pool, se encarga de hacer el accept de la conexión
 *              La entrada a este accept está controlada por un semáforo para controlar la sección crítica,
 *              Una vez aceptada, se procesa mediante la llamada a la función process_http_request y
 * ARGS_OUT: NULL - Retorna un puntero nulo
 ********/
void* thread_main() {
    
    struct sockaddr_storage cliaddr;
    socklen_t clilen;
    int connfd = -1;
    
    // Bucle principal del hilo mientras no se reciba señal de interrupción
    while (!sigint) {
        clilen = addrlen;
        
        if (sem == NULL) {
            break;
        }
        
        // Espera en el semáforo para entrar a la sección crítica
        if (my_lock_wait(sem) == -1) {
            if (!sigint) {
                writeLog(ERR, "Error en sem_wait");
            }
            break;
        }
        
         // Verifica si hay que finalizar
        if (sigint) {
            my_lock_release(sem);
            break;
        }
        
        // Acepta conexión
        connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        
        // Libera el semáforo antes y procesa la solicitud
        if (connfd >= 0) {
            my_lock_release(sem);
            process_http_request(connfd, conf);
            Close(connfd);
        } else {
            my_lock_release(sem);
            if (!sigint) {
                writeLog(ERR, "Error al aceptar conexión");
            }
        }
    }

    return NULL;
}

/********
 * FUNCIÓN: int main()  
 * ARGS_IN: -
 * DESCRIPCIÓN:  Esta función configura y ejecuta el servidor de manera concurrente, creando hilos para manejar múltiples clientes.
 *               Espera a que todos los hilos terminen su ejecución y posteriormente libera los recursos utilizados 
 *               y cierra el servidor correctamente.
 * ARGS_OUT: NULL - Retorna un puntero nulo
 ********/
int main() {
    char port_str[PORT_LEN];
    int i;

    // Configura el manejador de señal para SIGINT
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        writeLog(ERR, "Error al configurar manejador SIGINT");
        exit(EXIT_FAILURE);
    }

    // Lee la configuración del servidor
    if (read_config(&conf) == ERROR) {
        writeLog(ERR, "Error al leer configuración");
        exit(EXIT_FAILURE);
    }

    // Inicializa el sistema de logging
    if (initLog() == ERROR) {
        exit(EXIT_FAILURE);
    }



    // Convierte el puerto a string y configura el socket de escucha
    snprintf(port_str, sizeof(port_str), "%d", conf.port);
    listenfd = Tcp_listen("0.0.0.0", port_str, &addrlen);
    if (listenfd < 0) {
        cleanup_resources();
        exit(EXIT_FAILURE);
    }

    // Inicializa el semáforo
    sem = my_lock_init();
    if (!sem) {
        cleanup_resources();
        exit(EXIT_FAILURE);
    }

    // Memoria para hilos
    threads = calloc(conf.maxClients, sizeof(pthread_t));
    if (!threads) {
        writeLog(ERR, "Error al asignar memoria para hilos");
        cleanup_resources();
        exit(EXIT_FAILURE);
    }

    // Crear hilos y rastrear argumentos
    for (i = 0; i < conf.maxClients && !sigint; i++) {
        
        if (pthread_create(&threads[i], NULL, thread_main,NULL) != 0) {
            writeLog(ERR, "Error al crear hilo");
            break;
        }
        num_threads_created++;
    }

    while (!sigint) {
        pause();
    }

    // Cancelar hilos activos y esperar su terminación
    for (i = 0; i < num_threads_created; i++) {
        pthread_cancel(threads[i]);
        pthread_join(threads[i], NULL);
    }

    /* esto cuando se quita, da problemas, cuando se prueba cualquier peticion que se complete, pero cuando se interrumpe alguna peticion funciona mejor */

    cleanup_resources();
    return EXIT_SUCCESS;
}