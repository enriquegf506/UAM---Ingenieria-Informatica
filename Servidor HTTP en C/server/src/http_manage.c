/**
 * @brief Funcionalidad para manejar y parsear las solicitudes http
 * @file http_manage.c
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 23/02/2025
 */

#include "../lib/http_manage.h"

/********
 * FUNCIÓN: http_method_t parse_http_method(const char *method_str, size_t method_len)  
 * ARGS_IN: const char *method_str - Cadena que contiene el método HTTP.
 *          size_t method_len - Longitud de la cadena `method_str`.
 * DESCRIPCIÓN: Analiza el método HTTP de la solicitud y lo convierte en un valor del tipo `http_method_t`.
 * ARGS_OUT: http_method_t - El método HTTP correspondiente como un valor del enum `http_method_t`.
 ********/
http_method_t parse_http_method(const char *method_str, size_t method_len) {
    if (strncmp(method_str, "GET", method_len) == 0) return HTTP_METHOD_GET;
    if (strncmp(method_str, "POST", method_len) == 0) return HTTP_METHOD_POST;
    if (strncmp(method_str, "OPTIONS", method_len) == 0) return HTTP_METHOD_OPTIONS;
    
    writeLog(WARNING, "Método HTTP desconocido o no soportado");
    return HTTP_METHOD_UNKNOWN;
}

/********
 * FUNCIÓN: file_type_t parse_http_file(const char *path, const char *headers)  
 * ARGS_IN: const char *path - La ruta del archivo solicitada.
 *          const char *headers - Los headers de la solicitud HTTP que pueden incluir un Content-Type.
 * DESCRIPCIÓN: Determina el tipo de archivo de la solicitud basándose en la ruta del archivo o los headers.
 * ARGS_OUT: file_type_t - El tipo de archivo detectado como un valor del enum `file_type_t`.
 ********/
file_type_t parse_http_file(const char *path, const char *headers) {
    if (strstr(headers, "Content-Type: image/png") || strstr(path, ".png")) {
        writeLog(INFO, "Archivo detectado: PNG");
        return FILE_TYPE_PNG;
    }
    if (strstr(headers, "Content-Type: image/jpeg") || strstr(path, ".jpg") || strstr(path, ".jpeg")) {
        writeLog(INFO, "Archivo detectado: JPEG");
        return FILE_TYPE_JPEG;
    }
    if (strstr(headers, "Content-Type: image/gif") || strstr(path, ".gif")) {
        writeLog(INFO, "Archivo detectado: GIF");
        return FILE_TYPE_GIF;
    }
    if (strstr(headers, "Content-Type: image/bmp") || strstr(path, ".bmp")) {
        writeLog(INFO, "Archivo detectado: BMP");
        return FILE_TYPE_BMP;
    }
    if (strstr(headers, "Content-Type: text/html") || strstr(path, ".html")) {
        writeLog(INFO, "Archivo detectado: HTML");
        return FILE_TYPE_HTML;
    }
    if (strstr(headers, "Content-Type: text/css") || strstr(path, ".css")) {
        writeLog(INFO, "Archivo detectado: CSS");
        return FILE_TYPE_CSS;
    }
    if (strstr(headers, "Content-Type: application/javascript") || strstr(path, ".js")) {
        writeLog(INFO, "Archivo detectado: JavaScript");
        return FILE_TYPE_JS;
    }
    if (strstr(headers, "Content-Type: application/x-python-code") || strstr(path, ".py")) {
        writeLog(INFO, "Archivo detectado: Python");
        return FILE_TYPE_PY;
    }
    if (strstr(headers, "Content-Type: application/x-php") || strstr(path, ".php")) {
        writeLog(INFO, "Archivo detectado: PHP");
        return FILE_TYPE_PHP;
    }

    writeLog(WARNING, "Tipo de archivo desconocido");
    return FILE_TYPE_UNKNOWN;
}

/********
 * FUNCIÓN: int process_http_request(int connfd, struct ServerConf conf)  
 * ARGS_IN: int connfd - El descriptor de archivo de la conexión del cliente.
 *          struct ServerConf conf - Estructura de configuración del servidor.
 * DESCRIPCIÓN: Procesa una solicitud HTTP entrante, parseando el método, headers, y el cuerpo de la solicitud.
 * ARGS_OUT: int - devuelve 0 si la solicitud fue procesada correctamente, o un valor negativo en caso de error.
 ********/
int process_http_request(int connfd, struct ServerConf conf) {
    char buffer[MAX_REQUEST_SIZE + 1]; // +1 para el \0
    
    // Leer datos de la solicitud HTTP
    ssize_t buflen = read(connfd, buffer, MAX_REQUEST_SIZE);
    if (buflen <= 0) {
        writeLog(ERR, "Error al leer datos del socket");
        perror("Error reading from socket");
        return -1;
    }
    
    // \0 al final del buffer
    buffer[buflen] = '\0';
    
    const char *method, *path;
    int minor_version;
    struct phr_header headers[MAX_HEADERS];
    size_t method_len, path_len, num_headers = sizeof(headers) / sizeof(headers[0]);
    size_t prevbuflen = 0;
    
    // Parsear la solicitud HTTP usando la función `phr_parse_request`
    int pret = phr_parse_request(buffer, buflen, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, prevbuflen);
    
    // Comprueba si hubo error al parsear la solicitud
    if (pret <= 0) {
        if (pret == -1) {
            writeLog(ERR, "Error al parsear solicitud HTTP");
            perror("Error al parsear la solicitud");
        } else if (pret == -2) {
            writeLog(WARNING, "Solicitud HTTP incompleta");
            perror("Solicitud HTTP incompleta");
        }
        return pret;
    }
    
    // Prepara la estructura http_request_t y la inicializa
    http_request_t request;
    memset(&request, 0, sizeof(request));
    
    // Convierte el método HTTP a un valor del enum `http_method_t`
    request.method = parse_http_method(method, method_len);
    if (request.method == HTTP_METHOD_UNKNOWN) {
        writeLog(ERR, "Método HTTP no soportado");
        send_405(connfd, conf);
        perror("Método HTTP no soportado");
        return -1;
    }
    
    // Copia el path de la solicitud en la estructura
    if (path_len >= MAX_PATH_SIZE) {
        writeLog(ERR, "Path demasiado largo");
        perror("Path demasiado largo");
        return -1;
    }
    memcpy(request.path, path, path_len);
    request.path[path_len] = '\0';
    
    // Formatea la versión HTTP
    if (snprintf(request.version, sizeof(request.version), "HTTP/1.%d", minor_version) < 0) {
        writeLog(ERR, "Error al formatear versión HTTP");
        perror("Error formatting HTTP version");
        return -1;
    }
    
    // Procesa los headers de la solicitud
    char *header_ptr = request.headers;
    size_t headers_remaining = MAX_HEADER_SIZE;
    *header_ptr = '\0'; // Inicializar como cadena vacía
    
    for (size_t i = 0; i < num_headers; i++) {
        size_t total_len = headers[i].name_len + headers[i].value_len + 4; // ": " + "\r\n"
        
        if (total_len >= headers_remaining) {
            writeLog(ERR, "Headers demasiado grandes");
            perror("Headers demasiado grandes");
            return -1;
        }
        
        // Copia el nombre del header
        memcpy(header_ptr, headers[i].name, headers[i].name_len);
        header_ptr += headers[i].name_len;
        
        // Añade separador ": "
        memcpy(header_ptr, ": ", 2);
        header_ptr += 2;
        
        // Copia el valor del header
        memcpy(header_ptr, headers[i].value, headers[i].value_len);
        header_ptr += headers[i].value_len;
        
        // Añade fin de línea "\r\n"
        memcpy(header_ptr, "\r\n", 2);
        header_ptr += 2;
        
        headers_remaining -= total_len;
    }
    *header_ptr = '\0'; // Terminar headers
    
    // Extraer el cuerpo de la solicitud, si existe
    size_t headers_part_len = pret;
    if ((size_t)buflen > headers_part_len) {
        size_t body_len = buflen - headers_part_len;
        if (body_len >= MAX_BODY_SIZE) {
            writeLog(ERR, "Body demasiado grande");
            perror("Body demasiado grande");
            return -1;
        }
        memcpy(request.body, buffer + headers_part_len, body_len);
        request.body[body_len] = '\0';
    } else {
        request.body[0] = '\0';
    }

    // Llamae a la función adecuada según el método HTTP
    switch (request.method) {
        case HTTP_METHOD_GET:
            writeLog(INFO, "Procesando petición GET");
            handle_get_request(connfd, request, conf);
            break;
        case HTTP_METHOD_POST:
            writeLog(INFO, "Procesando petición POST");
            handle_post_request(connfd, request, conf);
            break;
        case HTTP_METHOD_OPTIONS:
            writeLog(INFO, "Procesando petición OPTIONS");
            handle_options_request(connfd, request, conf);
            break;
        default:
            writeLog(WARNING, "Método HTTP no soportado - 405 Method Not Allowed");
            send_405(connfd, conf);
            break;
    }
    
    return 0;
}
