/**
 * @brief Define métodos, variables globales y librerias utilizadas para los métodos para manejar y parsear las solicitudes http
 * @file http_manage.h
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 23/02/2025
 */

#ifndef HTTP_MANAGE_H
#define HTTP_MANAGE_H

// Bibliotecas necesarias
#include "http_methods.h"   //Para manejar métodos HTTP
#include "picohttpparser.h" // Para el análisis de peticiones HTTP
#include "types.h"          // Definiciones de tipos personalizados
#include "log_manage.h"     // Para el manejo de logs

// Códigos de estado HTTP
#define HTTP_OK 200            // Respuesta exitosa
#define HTTP_NOT_FOUND 404     // Recurso no encontrado
#define HTTP_SERVER_ERROR 500  // Error interno del servidor

// Definiciones de tamaños máximos para buffers y estructuras
#define MAX_REQUEST_SIZE 4096  // Tamaño máximo de una petición HTTP
#define MAX_HEADERS 100        // Número máximo de cabeceras en una petición
#define MAX_HEADER_SIZE 8192   // Tamaño máximo para las cabeceras HTTP
#define MAX_PATH_SIZE 1024     // Tamaño máximo para la ruta de un recurso
#define MAX_BODY_SIZE 65536    // Tamaño máximo para el cuerpo de una petición

// Enumeración de los métodos HTTP soportados
typedef enum {
    HTTP_METHOD_GET,           // Método GET
    HTTP_METHOD_POST,          // Método POST
    HTTP_METHOD_OPTIONS,       // Método OPTIONS
    HTTP_METHOD_UNKNOWN        // Método desconocido
} http_method_t;

// Enumeración de los tipos de archivos soportados
typedef enum {
    FILE_TYPE_HTML,            // Archivo HTML
    FILE_TYPE_CSS,             // Archivo CSS
    FILE_TYPE_JS,              // Archivo JavaScript
    FILE_TYPE_JPEG,            // Archivo JPEG
    FILE_TYPE_PNG,             // Archivo PNG
    FILE_TYPE_GIF,             // Archivo GIF
    FILE_TYPE_BMP,             // Archivo BMP
    FILE_TYPE_PY,              // Archivo Python
    FILE_TYPE_PHP,             // Archivo PHP
    FILE_TYPE_UNKNOWN          // Tipo de archivo desconocido
} file_type_t;

// Estructura para almacenar una petición HTTP parseada
typedef struct {
    http_method_t method;           // Método HTTP utilizado
    char path[MAX_PATH_SIZE];       // Ruta del recurso solicitado
    char version[16];               // Versión del protocolo HTTP
    char headers[MAX_HEADER_SIZE];  // Cabeceras de la petición
    char body[MAX_BODY_SIZE];       // Cuerpo de la petición
} http_request_t;

// Funciones principales
/********
 * FUNCIÓN: int process_http_request(int connfd, struct ServerConf conf)  
 * ARGS_IN: int connfd - El descriptor de archivo de la conexión del cliente.
 *          struct ServerConf conf - Estructura de configuración del servidor.
 * DESCRIPCIÓN: Procesa una solicitud HTTP entrante, parseando el método, headers, y el cuerpo de la solicitud.
 * ARGS_OUT: int - devuelve 0 si la solicitud fue procesada correctamente, o un valor negativo en caso de error.
 ********/
int process_http_request(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: file_type_t parse_http_file(const char *path, const char *headers)  
 * ARGS_IN: const char *path - La ruta del archivo solicitada.
 *          const char *headers - Los headers de la solicitud HTTP que pueden incluir un Content-Type.
 * DESCRIPCIÓN: Determina el tipo de archivo de la solicitud basándose en la ruta del archivo o los headers.
 * ARGS_OUT: file_type_t - El tipo de archivo detectado como un valor del enum `file_type_t`.
 ********/
file_type_t parse_http_file(const char *path, const char *headers);
/********
 * FUNCIÓN: http_method_t parse_http_method(const char *method_str, size_t method_len)  
 * ARGS_IN: const char *method_str - Cadena que contiene el método HTTP.
 *          size_t method_len - Longitud de la cadena `method_str`.
 * DESCRIPCIÓN: Analiza el método HTTP de la solicitud y lo convierte en un valor del tipo `http_method_t`.
 * ARGS_OUT: http_method_t - El método HTTP correspondiente como un valor del enum `http_method_t`.
 ********/
http_method_t parse_http_method(const char *method_str, size_t method_len);

#endif // HTTP_HANDLER_H
