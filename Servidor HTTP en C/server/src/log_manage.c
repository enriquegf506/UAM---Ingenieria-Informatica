/**
 * @brief Funcionalidad del sistema de logs
 * @file log_manage.c
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 23/02/2025
 */

 #include "../lib/log_manage.h"

static FILE* logFile = NULL;                                    // Puntero al archivo de logs
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;   // Mutex para la sincronización de accesos al archivo de logs

/**
 * FUNCIÓN: STATUS initLog()  
 * ARGS_IN: Ninguno.  
 * DESCRIPCIÓN: Inicializa el sistema de logs, abriendo el archivo de log en modo "a+" para que los logs sean anexados.  
 * Si ocurre un error al abrir el archivo, devuelve un código de error.  
 * ARGS_OUT: STATUS - devuelve `OK` si la inicialización fue exitosa, o `ERROR` si hubo un fallo al abrir el archivo de log.
 */
STATUS initLog() {
    // Intentar abrir el archivo de log
    logFile = fopen(LOG_FILE, "a+"); 
    if (logFile == NULL) {
        writeLog(ERR, "Error abriendo el archivo de log");
        return ERROR;  // devuelve ERROR si no se pudo abrir el archivo
    }
    return OK;  
}

/**
 * FUNCIÓN: STATUS writeLog(LogType level, const char* log)  
 * ARGS_IN: LogType level - El nivel de severidad del log (INFO, WARNING, ERR).  
 *          const char* log - El mensaje de log que se desea escribir.  
 * DESCRIPCIÓN: Escribe un mensaje de log en el archivo de log.
 * La función usa un mutex debido a que puede haber un acceso concurrente y se trata de una sección crítica 
 * ARGS_OUT: STATUS - devuelve `OK` si el log fue escrito correctamente, o `ERROR` si hubo un problema al escribir.
 */
STATUS writeLog(LogType level, const char* log) {
    if (logFile == NULL || log == NULL){ 
        return ERROR;  // devuelve ERROR si el archivo o el mensaje son nulos
    } 

    // Entra en sección crítica
    pthread_mutex_lock(&log_mutex);  
    
    // Obtiene el tiempo
    time_t now = time(NULL); 
    char timestamp[MAX_LENGTH]; 
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    const char* level_str;
    switch(level) {
        case INFO:    level_str = "INFO";    break;  // Nivel INFO
        case WARNING: level_str = "WARNING"; break;  // Nivel WARNING
        case ERR:   level_str = "ERROR";   break;  // Nivel ERROR
        default:          level_str = "UNKNOWN"; break;  // Nivel desconocido
    }
    
    // Escribir el mensaje en el archivo de log
    fprintf(logFile, "[%s] %s: %s\n", timestamp, level_str, log); 
    fflush(logFile);
    
    //Sale de sección crítica
    pthread_mutex_unlock(&log_mutex);  
    return OK;
}

/**
 * FUNCIÓN: void closeLog()  
 * ARGS_IN: Ninguno.  
 * DESCRIPCIÓN: Cierra el archivo de log y destruye el mutex. Esta función asegura que se liberen los recursos del sistema de logs.  
 * ARGS_OUT: void 
 */
void closeLog() {
    pthread_mutex_lock(&log_mutex);  // Bloquear el mutex para acceso seguro
    fclose(logFile);  //Cerrar el archivo de log
    logFile = NULL;  // Establecer el puntero a NULL
    pthread_mutex_unlock(&log_mutex);  // Desbloquear el mutex
    pthread_mutex_destroy(&log_mutex);  // Destruir el mutex
}
