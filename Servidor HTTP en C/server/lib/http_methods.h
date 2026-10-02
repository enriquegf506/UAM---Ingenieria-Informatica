/**
 * @brief Define métodos, variables globales y librerias utilizadas para los métodos de soporte a verbos GET, Post y OPTIONS
 * @file http_methods.h
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 23/02/2025
 */

#ifndef HTTP_METHODS_H
#define HTTP_METHODS_H

// Bibliotecas necesarias para el manejo de HTTP
#include "types.h"             // Definiciones de tipos personalizados
#include "http_manage.h"       // Para manejar peticiones HTTP
#include "log_manage.h"        // Para el manejo de logs

// Códigos de resultado para las operaciones
#define METHOD_SUCCESS 0       // Operación exitosa
#define METHOD_ERROR -1        // Error genérico en la operación
#define METHOD_NOT_FOUND -2    // Recurso no encontrado


// Funciones principales para cada método HTTP

/********
 * FUNCIÓN: int handle_get_request(int connfd, http_request_t request)   
 * ARGS_IN: int connfd - Descriptor de socket.
 *          http_request_t request - Estructura con la solicitud HTTP.
 * DESCRIPCIÓN: Maneja las solicitudes GET, enviando archivos al cliente si existen.
 * ARGS_OUT: int - Código de éxito o error.
 ********/
int handle_get_request();
/********
 * FUNCIÓN: int handle_post_request(int connfd, http_request_t request)   
 * ARGS_IN: int connfd - Descriptor de socket.
 *          http_request_t request - Estructura con la solicitud HTTP.
 * DESCRIPCIÓN: Maneja las solicitudes POST, guardando el contenido recibido en un archivo.
 * ARGS_OUT: int - Código de éxito o error.
 ********/
int handle_post_request();
/********
 * FUNCIÓN: int handle_options_request(int connfd, http_request_t request)   
 * ARGS_IN: int connfd - Descriptor de socket.
 *          http_request_t request - Estructura con la solicitud HTTP.
 * DESCRIPCIÓN: Maneja las solicitudes OPTIONS, enviando los métodos soportados.
 * ARGS_OUT: int - Código de éxito o error.
 ********/
int handle_options_request();
/********
 * FUNCIÓN: void send_500(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 500 (Internal Server Error) al cliente.
 * ARGS_OUT: void.
 ********/
void send_500(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: void send_400(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *         struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 400 (Bad Request) al cliente.
 * ARGS_OUT: void.
 * *******/
void send_400(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: void send_414(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 414 (URI Too Long) al cliente.
 * ARGS_OUT: void.
 ********/
void send_414(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: void send_405(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 405 (Method Not Allowed) al cliente.
 * ARGS_OUT: void.
 ********/
void send_405(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: void send_404(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 404 (Not Found) al cliente.
 * ARGS_OUT: void.
 ********/
void send_404(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: void send_403(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 403 (Forbidden) al cliente.
 * ARGS_OUT: void.
 ********/
void send_403(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: void send_409(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 409 (Conflict) al cliente.
 * ARGS_OUT: void.
 ********/
void send_409(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: void send_201(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 201 (Created) al cliente.
 * ARGS_OUT: void.
 ********/
void send_201(int connfd, struct ServerConf conf);
/********
 * FUNCIÓN: void send_error_response(int connfd, const char *code, const char *message, const char *body, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          const char *code - Código de estado HTTP.
 *          const char *message - Mensaje de estado HTTP.
 *          const char *body - Cuerpo del mensaje de error.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta de error HTTP con el código, mensaje y cuerpo especificados.
 * ARGS_OUT: void.
 ********/
void send_error_response(int connfd, const char *code, const char *message, const char *body, struct ServerConf conf);


#endif 