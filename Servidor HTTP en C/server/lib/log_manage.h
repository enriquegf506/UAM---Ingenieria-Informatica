/**
 * @brief Define métodos, variables globales y librerias utilizadas para el sistema de logs
 * @file log_manage.h
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 23/02/2025
 */

#ifndef LOG_MANAGE_H
#define LOG_MANAGE_H

#include "types.h"


// Definición de variables globales
#define LOG_FILE "server.log"    // Nombre del archivo donde se guardarán los logs
#define MAX_LOG_LINE 1024        // Tamaño máximo de una línea de log

// Enumeración de los niveles de log disponibles
typedef enum {
    INFO,     // Mensajes informativos
    WARNING,  // Mensajes de advertencia
    ERR       // Mensajes de error
} LogType;

// Declaraciones de funciones
/*******
 * FUNCIÓN: STATUS initLog()  
 * ARGS_IN: Ninguno.  
 * DESCRIPCIÓN: Inicializa el sistema de logs, abriendo el archivo de log en modo "a+" para que los logs sean anexados.  
 * Si ocurre un error al abrir el archivo, devuelve un código de error.  
 * ARGS_OUT: STATUS - devuelve `OK` si la inicialización fue exitosa, o `ERROR` si hubo un fallo al abrir el archivo de log.
 ********/
STATUS initLog();
/**
 * FUNCIÓN: STATUS writeLog(LogType level, const char* log)  
 * ARGS_IN: LogType level - El nivel de severidad del log (INFO, WARNING, ERR).  
 *          const char* log - El mensaje de log que se desea escribir.  
 * DESCRIPCIÓN: Escribe un mensaje de log en el archivo de log.
 * La función usa un mutex debido a que puede haber un acceso concurrente y se trata de una sección crítica 
 * ARGS_OUT: STATUS - devuelve `OK` si el log fue escrito correctamente, o `ERROR` si hubo un problema al escribir.
 */
STATUS writeLog(LogType level, const char* log);

/********
 * FUNCIÓN: void closeLog()  
 * ARGS_IN: Ninguno.  
 * DESCRIPCIÓN: Cierra el archivo de log y destruye el mutex. Esta función asegura que se liberen los recursos del sistema de logs.  
 * ARGS_OUT: void 
 *******/
void closeLog();

#endif