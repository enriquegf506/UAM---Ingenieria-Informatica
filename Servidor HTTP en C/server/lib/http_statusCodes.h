/**
 * @brief Define métodos, variables globales y librerias utilizadas para los métodos para manejar los codigos http
 * @file http_statusCodes.h
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 23/02/2025
 */

#ifndef HTTP_STATUSCODES_H
#define HTTP_STATUSCODES_H

#include <stdio.h>  

// Enumeración de códigos de estado HTTP comunes
typedef enum {
    HTTP_OK = 200,                      // Respuesta exitosa
    HTTP_BAD_REQUEST = 400,             // Solicitud incorrecta
    HTTP_UNAUTHORIZED = 401,            // No autorizado
    HTTP_FORBIDDEN = 403,               // Prohibido
    HTTP_NOT_FOUND = 404,               // Recurso no encontrado
    HTTP_INTERNAL_SERVER_ERROR = 500,   // Error interno del servidor
    HTTP_NOT_IMPLEMENTED = 501          // Método no implementado
} HttpStatusCode;

// Estructura para asociar un código de estado HTTP con su mensaje correspondiente
typedef struct {
    HttpStatusCode code;       // Código de estado HTTP
    const char *message;       // Mensaje del código de estado
} HttpStatus;

// Mapea códigos de estado HTTP a sus mensajes correspondientes
HttpStatus httpStatuses[] = {
    {HTTP_OK, "OK"},
    {HTTP_BAD_REQUEST, "Bad Request"},
    {HTTP_UNAUTHORIZED, "Unauthorized"},
    {HTTP_FORBIDDEN, "Forbidden"},
    {HTTP_NOT_FOUND, "Not Found"},
    {HTTP_INTERNAL_SERVER_ERROR, "Internal Server Error"},
    {HTTP_NOT_IMPLEMENTED, "Not Implemented"}
};

/**
 * Obtiene el mensaje descriptivo asociado a un código de estado HTTP.
 * 
 * @param code Código de estado HTTP.
 * @return Mensaje descriptivo del código de estado, o "Unknown Status" si no se encuentra.
 */
const char* getHttpStatusMessage(HttpStatusCode code) {
    // Recorre el arreglo httpStatuses para encontrar el mensaje correspondiente al código
    for (size_t i = 0; i < sizeof(httpStatuses) / sizeof(httpStatuses[0]); i++) {
        if (httpStatuses[i].code == code) {
            return httpStatuses[i].message;
        }
    }
    // Mensaje si no se encuentra
    return "Unknown Status";
}

#endif
