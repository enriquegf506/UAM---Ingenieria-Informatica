/**
 * @brief Funcionalidad de los métodos http soportados
 * @file http_methods.c
 * @author Daniel Aquino y Enrique Gómez
 * @version 1.0
 * @date 23/02/2025
 */

#include "../lib/http_methods.h"

#define MAX_FILE_SIZE 10485760      // Tamaño máximo en bytes para archivos procesados por el servidor.
#define MAX_PATH_LENGTH 4096        // Longitud máxima permitida para rutas de archivos
#define MAX_CMD_SIZE 8200           // Tamaño máximo para comandos del sistema que ejecutan scripts.
#define UPLOAD_DIR "./uploads/"     // Directorio donde se almacenan los archivos subidos por los usuarios.
#define MESSAGE_FILE "messages.txt" // Nombre del archivo donde se guardan los mensajes del sistema

/**
 * @brief Estructura que define un manejador de scripts.
 */
typedef struct {
    const char *extension;  /**< Extensión del archivo a manejar */
    const char *command;    /**< Comando para ejecutar el script */
    file_type_t type;       /**< Tipo de archivo enumerado */
} script_handler_t;


/**
 * @brief  Manejadores de scripts soportados por el servidor.
 * Cada elemento contiene la extensión, el comando para ejecutarlo y el tipo
 */
static const script_handler_t script_handlers[] = {
    {".py", "python3", FILE_TYPE_PY},     /**< Manejador para archivos Python */
    {".php", "php", FILE_TYPE_PHP},       /**< Manejador para archivos PHP */
    {NULL, NULL, FILE_TYPE_UNKNOWN}       /**< Terminador del array */
};

/********
 * FUNCIÓN: char *get_relative_path_to_src()   
 * ARGS_IN: Ninguno.
 * DESCRIPCIÓN: Obtiene la ruta relativa hasta el directorio 'src'.
 * ARGS_OUT: char* - Puntero a la cadena con la ruta relativa (debe liberarse después).
 ********/
char *get_relative_path_to_src() {
    char cwd[MAX_PATH_LENGTH];
    char *src_dir = "/server";  
    char *pos;
    char *relative_path = NULL;

    // Obtiene el directorio actual
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // Busca el directorio 'server' en la ruta actual
        pos = strstr(cwd, src_dir);
        if (pos != NULL) {
            // Si encuentra el directorio, trunca la cadena en ese punto
            *pos = '\0';
            relative_path = malloc(strlen(cwd) + 1);
            if (relative_path != NULL) {
                strcpy(relative_path, cwd);
            } else {
                writeLog(ERR, "Error al asignar memoria para ruta relativa");
                perror("Error al asignar memoria");
                free(relative_path);
                return NULL;
            }
        } else {
            writeLog(WARNING, "No se encontró el directorio 'server' en la ruta actual");
            printf("No se encontró el directorio 'src' en la ruta actual.\n");
            free(relative_path);
            return NULL;
        }
    } else {
        writeLog(ERR, "Error al obtener el directorio de trabajo actual");
        perror("getcwd() error");
        free(relative_path);
        return NULL;
    }
    // Devuelve la ruta relativa hasta el directorio padre de 'server'
    return relative_path;
}


/********
 * FUNCIÓN: char** split_query_string(char *query_string)
 * ARGS_IN: char *query_string - Cadena de consulta a dividir.
 * DESCRIPCIÓN: Divide la cadena de consulta en tokens separados por '+' después del '='.
 * ARGS_OUT: char** - Array de punteros a los tokens extraídos, o NULL si no hay '='.
 ********/
char** split_query_string(char *query_string) {
    
    char *query_start = strchr(query_string, '=');
    if (query_start == NULL) {
        return NULL;
    }
    query_start++;
    char **tokens = malloc(10 * sizeof(char*));
    char *token = strtok(query_start, "+");
    int i = 0;
    while (token != NULL) {
        tokens[i] = token;
        token = strtok(NULL, "+");
        i++;
    }
    tokens[i] = NULL;
    return tokens;

}

/********
 * FUNCIÓN: static int build_file_path(char *dest, size_t dest_size, const char *base_dir, const char *root_dir, const char *path)
 * ARGS_IN: char *dest - Buffer de destino para la ruta completa.
 *          size_t dest_size - Tamaño del buffer de destino.
 *          const char *base_dir - Directorio base del servidor.
 *          const char *root_dir - Directorio raíz para servir archivos.
 *          const char *path - Ruta del archivo solicitado.
 * DESCRIPCIÓN: Construye la ruta completa del archivo, verificando límites y añadiendo index.html si es necesario.
 * ARGS_OUT: int - 0 si la construcción fue exitosa, -1 en caso de error.
 ********/
static int build_file_path(char *dest, size_t dest_size, const char *base_dir, const char *root_dir, const char *path) {
    int has_trailing_slash = (root_dir[strlen(root_dir) - 1] == '/');
    // Si la ruta comienza con barra diagonal, la omite
    const char *effective_path = (path[0] == '/' ? path + 1 : path);
    // Construye la ruta completa
    int result = snprintf(dest, dest_size, "%s/%s%s%s", base_dir, root_dir, 
                          has_trailing_slash ? "" : "/", effective_path);

    if (result < 0 || (size_t)result >= dest_size) {
        writeLog(ERR, "Ruta demasiado larga o error en snprintf");
        return -1;
    }

    // Verifica si la ruta termina en '/' y no es solo "/"
    size_t path_len = strlen(dest);
    if (path_len > 1 && dest[path_len - 1] == '/') {
        // Añade "index.html" comprobando antes si se puede
        const char *index_file = "index.html";
        size_t index_len = strlen(index_file);
        if (path_len + index_len >= dest_size) {
            writeLog(ERR, "Ruta con index.html demasiado larga");
            return -1;
        }
        strncat(dest, index_file, dest_size - path_len - 1);
    }

    return 0;
}

/********
 * FUNCIÓN: int execute_script(int connfd, const char *filepath, char *query_string, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          const char *filepath - Ruta del archivo script a ejecutar.
 *          char *query_string - Cadena de consulta con parámetros para el script.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Ejecuta un script (.py o .php) y captura su salida para enviarla al cliente.
 * ARGS_OUT: int - METHOD_SUCCESS si la ejecución fue exitosa, METHOD_ERROR en caso contrario.
 ********/
static int execute_script(int connfd, const char *filepath, char *query_string, struct ServerConf conf) {
    char cmd[MAX_CMD_SIZE] = {0};
    const script_handler_t *handler = NULL;

    // Busca el manejador adecuado para la extensión
    for (int i = 0; script_handlers[i].extension != NULL; i++) {
        if (strstr(filepath, script_handlers[i].extension) != NULL) {
            handler = &script_handlers[i];
            break;
        }
    }

    // Si no hay un manejador disponible, devuelve un error 404
    if (!handler) {
        send_404(connfd, conf);
        return METHOD_ERROR;
    }

    // Construye el comando
    snprintf(cmd, sizeof(cmd), "%s %s", handler->command, filepath);
    if (query_string && strlen(query_string) > 0) {
        char **params = split_query_string(query_string);

        for (int i = 0; params[i] != NULL; i++) {
            strncat(cmd, " ", sizeof(cmd) - strlen(cmd) - 1);
            strncat(cmd, params[i], sizeof(cmd) - strlen(cmd) - 1);
        }

        free(params);
    }


    // Ejecutar script
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        writeLog(ERR, "Error al ejecutar el script");
        send_500(connfd, conf);
        return METHOD_ERROR;
    }

    // Lee la salida del script
    char output[MAX_BODY_SIZE];
    size_t bytes_read = fread(output, 1, sizeof(output) - 1, pipe);

    if (bytes_read == 0) {
        writeLog(ERR, "Error reading from pipe");
        send_404(connfd, conf);
        pclose(pipe);
        return METHOD_ERROR;
    }

    output[bytes_read] = '\0';  

    // Cerrar el pipe
    if (pclose(pipe) == -1) {
        send_500(connfd, conf);
        writeLog(ERR, "Error closing pipe");
        return METHOD_ERROR;
    }
    // Enviar respuesta
    char response[MAX_BODY_SIZE + 610];

    // Calcular tiempo
    char date_buf[MAX_LENGTH];
    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);
    strftime(date_buf, sizeof(date_buf), "%a, %d %b %Y %H:%M:%S GMT", tm_info);

    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Date: %s\r\n"
             "Server: %s\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s",
             date_buf, conf.signature, strlen(output), output);
    write(connfd, response, strlen(response));
    return METHOD_SUCCESS;
}


/********
 * FUNCIÓN: const char* get_content_type(const char *filename)   
 * ARGS_IN: const char *filename - Nombre del archivo.
 * DESCRIPCIÓN: Determina el tipo de contenido (MIME type) basado en la extensión del archivo.
 * ARGS_OUT: const char* - Cadena con el tipo de contenido MIME.
 ********/
const char* get_content_type(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) {
        return "application/octet-stream";
    }
    
    ext++; // Saltar el punto
    const char *contentType = "application/octet-stream";
    
    if (strcasecmp(ext, "html") == 0 || strcasecmp(ext, "htm") == 0) contentType = "text/html";
    else if (strcasecmp(ext, "txt") == 0) contentType = "text/plain";
    else if (strcasecmp(ext, "json") == 0) contentType = "application/json";
    else if (strcasecmp(ext, "jpg") == 0 || strcasecmp(ext, "jpeg") == 0) contentType = "image/jpeg";
    else if (strcasecmp(ext, "png") == 0) contentType = "image/png";
    else if (strcasecmp(ext, "gif") == 0) contentType = "image/gif";
    else if (strcasecmp(ext, "pdf") == 0) contentType = "application/pdf";
    else if (strcasecmp(ext, "doc") == 0 || strcasecmp(ext, "docx") == 0) contentType = "application/msword";
    else if (strcasecmp(ext, "mpeg") == 0 || strcasecmp(ext, "mpg") == 0) contentType = "video/mpeg";
        
    return contentType;
}


/********
 * FUNCIÓN: int handle_get_request(int connfd, http_request_t request)   
 * ARGS_IN: int connfd - Descriptor de socket.
 *          http_request_t request - Estructura con la solicitud HTTP.
 * DESCRIPCIÓN: Maneja las solicitudes GET, enviando archivos al cliente si existen.
 * ARGS_OUT: int - Código de éxito o error.
 ********/
int handle_get_request(int connfd, http_request_t request, struct ServerConf conf) {
    char filepath[MAX_PATH_LENGTH];

    // Obtiene la ruta base del servidor
    char *base_dir = get_relative_path_to_src();


    if (!base_dir) {
        // Si no se puede obtener la ruta devuelve un error 500
        writeLog(ERR, "Error al obtener la ruta base");
        perror("Error en get actual path");
        send_500(connfd, conf);
        return METHOD_ERROR;
    }

    // Verifica si la solicitud tiene un formato básico válido
    if (strlen(request.path) == 0) {
        writeLog(WARNING, "Solicitud GET con ruta vacía o nula");
        free(base_dir);
        send_400(connfd, conf);
        return METHOD_ERROR;
    }

    // Verifica si hay intento de directory traversal para prevenir ataques
    if (strstr(request.path, "../")) {
        writeLog(WARNING, "Intento de traversal de directorio detectado en GET");
        free(base_dir);
        send_403(connfd, conf);
        return METHOD_ERROR;
    }

    // Hace una copia de la ruta para manipularla sin modificar el original
    char path_copy[MAX_PATH_LENGTH];
    strncpy(path_copy, request.path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    char *query_start = strchr(path_copy, '?');
    char query_string[MAX_PATH_LENGTH] = "";
    if (query_start != NULL) {
        *query_start = '\0';  // Truncar la ruta en el '?'
        strncpy(query_string, query_start + 1, sizeof(query_string) - 1);
        query_string[sizeof(query_string) - 1] = '\0';
    
        // Verifica si la query string está vacía después del '?'
        if (strlen(query_string) == 0) {
            writeLog(WARNING, "Query string vacía en solicitud GET");
            free(base_dir);
            send_400(connfd, conf);
            return METHOD_ERROR;
        }
    
        // Validar que el query string tenga formato clave=valor
        char *equal_sign = strchr(query_string, '=');
        if (equal_sign == NULL) {
            writeLog(WARNING, "Query string sin valor (falta '=') en solicitud GET");
            free(base_dir);
            send_400(connfd, conf);
            return METHOD_ERROR;
        }
        // Opcional: Verificar que haya un valor después del '='
        if (*(equal_sign + 1) == '\0') {
            writeLog(WARNING, "Query string con clave pero sin valor en solicitud GET");
            free(base_dir);
            send_400(connfd, conf);
            return METHOD_ERROR;
        }
    }

    // Construye la ruta completa del archivo
    if (build_file_path(filepath, sizeof(filepath), base_dir, conf.root, path_copy) < 0) {
        writeLog(ERR, "Error al construir la ruta del archivo");
        free(base_dir);
        send_414(connfd, conf);
        return METHOD_ERROR;
    }

    // Si es un script, lo ejecuta
    file_type_t file_type = parse_http_file(filepath, request.headers);
    if (file_type == FILE_TYPE_PHP || file_type == FILE_TYPE_PY) {
        int ret = execute_script(connfd, filepath, query_string, conf);
        free(base_dir);
        return ret;
    }

    // Abre el archivo y si no existe envia 404
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        writeLog(WARNING, "Archivo no encontrado");
        free(base_dir);
        send_404(connfd, conf);
        return METHOD_ERROR;
    }

    // Obtiene el tamaño del archivo
    struct stat st;
    fstat(fd, &st);
    off_t filesize = st.st_size;

    // Obtene la fecha actual en formato HTTP
    char date_buf[MAX_LENGTH];
    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);
    strftime(date_buf, sizeof(date_buf), "%a, %d %b %Y %H:%M:%S GMT", tm_info);

    // Envía la cabecera HTTP de la respuesta
    const char *content_type = get_content_type(filepath);
    char header[MAX_HEADER_SIZE];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Date: %s\r\n"
             "Server: %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "\r\n",
             date_buf, conf.signature, content_type, filesize);
    write(connfd, header, strlen(header));

    // Envía el contenido del archivo en bloques
    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(connfd, buffer, bytes_read);
    }

    // Cierra el archivo y libera recursos
    close(fd);
    free(base_dir);
    writeLog(INFO, "Archivo enviado correctamente");
    return METHOD_SUCCESS;
}

/********
 * FUNCIÓN: int handle_post_request(int connfd, http_request_t request)   
 * ARGS_IN: int connfd - Descriptor de socket.
 *          http_request_t request - Estructura con la solicitud HTTP.
 * DESCRIPCIÓN: Maneja las solicitudes POST, guardando el contenido recibido en un archivo.
 * ARGS_OUT: int - Código de éxito o error.
 ********/
int handle_post_request(int connfd, http_request_t request, struct ServerConf conf) {
    char filepath[MAX_PATH_LENGTH];
    // Obtiene la ruta base del servidor
    char *base_dir = get_relative_path_to_src();

    if (!base_dir) {
        // Si no se puede obtener la ruta devuelve un error 500
        writeLog(ERR, "Error al obtener la ruta base");
        free(base_dir);
        send_500(connfd, conf);
        return METHOD_ERROR;
    }

    // Verifica si la solicitud tiene un formato básico válido
    if (strlen(request.path) == 0) {
        writeLog(WARNING, "Solicitud POST con ruta vacía o nula");
        free(base_dir);
        send_400(connfd, conf);
        return METHOD_ERROR;
    }
    if (strlen(request.body) == 0) {
        writeLog(WARNING, "Solicitud POST sin cuerpo");
        free(base_dir);
        send_400(connfd, conf);
        return METHOD_ERROR;
    }

    // Verifica si hay intento de directory traversal para prevenir ataques
    if (strstr(request.path, "../")) {
        writeLog(WARNING, "Intento de traversal de directorio detectado en POST");
        free(base_dir);
        send_403(connfd, conf);
        return METHOD_ERROR;
    }

    // Hace una copia de la ruta para manipularla sin modificar el original
    char path_copy[MAX_PATH_LENGTH];
    strncpy(path_copy, request.path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    // Separa la ruta de la query string
    char *query_start = strchr(path_copy, '?');
    char query_string[MAX_PATH_LENGTH] = "";
    if (query_start != NULL) {
        *query_start = '\0';  // Trunca la ruta en el '?'
        strncpy(query_string, query_start + 1, sizeof(query_string) - 1);
        query_string[sizeof(query_string) - 1] = '\0';
    }

    // Construye la ruta completa
    if (build_file_path(filepath, sizeof(filepath), base_dir, conf.root, path_copy) < 0) {
        writeLog(ERR, "Error al construir la ruta del archivo");
        free(base_dir);
        send_414(connfd, conf);
        return METHOD_ERROR;
    }

    // Verifica si es un script ejecutable y si lo es, lo ejecuta
    file_type_t file_type = parse_http_file(filepath, request.headers);
    if (file_type == FILE_TYPE_PY || file_type == FILE_TYPE_PHP) {
        char query_string[strlen(request.body)];
        strcpy(query_string, request.body);
        int ret = execute_script(connfd, filepath, query_string, conf);
        free(base_dir);
        return ret;
    }

    // Si el archivo ya existe, envía un error 409
    if (access(filepath, F_OK) == 0) {
        writeLog(WARNING, "El archivo ya existe");
        free(base_dir);
        send_409(connfd, conf);
        return METHOD_ERROR;
    }

    //Crea el archivo y si no puede envia error 500
    int fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
        writeLog(ERR, "Error al crear archivo");
        free(base_dir);
        send_500(connfd, conf);
        return METHOD_ERROR;
    }

    // Escribe el contenido en el archivo
    ssize_t bytes_written = (file_type == FILE_TYPE_JPEG || file_type == FILE_TYPE_PNG ||
                            file_type == FILE_TYPE_GIF || file_type == FILE_TYPE_BMP)
                            ? write(fd, request.body, sizeof(request.body) - 1)
                            : dprintf(fd, "%s", request.body);

    close(fd);

    // Verifica si se escribieron datos en el archivo
    if (bytes_written <= 0) {
        writeLog(ERR, "Error al escribir datos en el archivo");
        free(base_dir);
        send_500(connfd, conf);
        return METHOD_ERROR;
    }

    free(base_dir);
    writeLog(INFO, "Archivo guardado correctamente");
    send_201(connfd, conf);
    return METHOD_SUCCESS;
}


/********
 * FUNCIÓN: int handle_options_request(int connfd, http_request_t request)   
 * ARGS_IN: int connfd - Descriptor de socket.
 *          http_request_t request - Estructura con la solicitud HTTP.
 * DESCRIPCIÓN: Maneja las solicitudes OPTIONS, enviando los métodos soportados.
 * ARGS_OUT: int - Código de éxito o error.
 ********/
int handle_options_request(int connfd, struct ServerConf conf) {
    //Esta la podemos enviar directamente
    writeLog(INFO, "Procesando petición OPTIONS");

    char date_buf[MAX_LENGTH];
    char response[MAX_HEADER_SIZE];

    // Obtene la fecha actual en formato HTTP
    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);
    strftime(date_buf, sizeof(date_buf), "%a, %d %b %Y %H:%M:%S GMT", tm_info);

    // Crea y envia respuesta
    snprintf(response, sizeof(response),
        "HTTP/1.1 204 No Content\r\n"
        "Date: %s\r\n"
        "Server: %s\r\n"
        "Allow: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "\r\n",
        date_buf, conf.signature);

    writeLog(INFO, "Respuesta OPTIONS enviada correctamente");
    if(send(connfd, response, strlen(response), 0) == 0) {
        writeLog(ERR, "Error al enviar respuesta OPTIONS");
        perror("Error in server send response");
        return METHOD_ERROR;
    }

    return METHOD_SUCCESS;
}

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
void send_error_response(int connfd, const char *code, const char *message, const char *body, struct ServerConf conf) {
    char response[MAX_BODY_SIZE];
    char date_buf[MAX_LENGTH];
    
    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);
    strftime(date_buf, sizeof(date_buf), "%a, %d %b %Y %H:%M:%S GMT", tm_info);

    //Construye la respuesta
    snprintf(response, sizeof(response),
        "HTTP/1.1 %s %s\r\n"
        "Date: %s\r\n"
        "Server: %s\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %ld\r\n"
        "\r\n"
        "<html><body>"
        "<h1>%s</h1>"
        "<p>%s</p>"
        "</body></html>",
        code, message, date_buf, conf.signature, strlen(body) + 120, message, body);

    //Envía la respuesta
    if (send(connfd, response, strlen(response), 0) == -1) {
        writeLog(ERR, "Error al enviar respuesta con cuerpo HTML");
        perror("Error en servidor al enviar respuesta con error");
    }
}


/********
 * FUNCIÓN: void send_414(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 414 (URI Too Long) al cliente.
 * ARGS_OUT: void.
 ********/
void send_414(int connfd, struct ServerConf conf) {
    send_error_response(connfd, "414", "URI Too Long", "La URI solicitada es demasiado larga.", conf);
}

/********
 * FUNCIÓN: void send_500(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 500 (Internal Server Error) al cliente.
 * ARGS_OUT: void.
 ********/
void send_500(int connfd, struct ServerConf conf) {
    send_error_response(connfd, "500", "Internal Server Error", "Hubo un error interno en el servidor. Intente más tarde.", conf);
}

/********
 * FUNCIÓN: void send_403(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 403 (Forbidden) al cliente.
 * ARGS_OUT: void.
 ********/
void send_403(int connfd, struct ServerConf conf) {
    send_error_response(connfd, "403", "Forbidden", "Acceso denegado. No tiene permisos para acceder al recurso.", conf);
}

/********
 * FUNCIÓN: void send_404(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 404 (Not Found) al cliente.
 * ARGS_OUT: void.
 ********/
void send_404(int connfd, struct ServerConf conf) {
    send_error_response(connfd, "404", "Not Found", "The requested URL was not found on this server.", conf);
}

/********
 * FUNCIÓN: void send_405(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 405 (Method Not Allowed) al cliente.
 * ARGS_OUT: void.
 ********/
void send_405(int connfd, struct ServerConf conf) {
    send_error_response(connfd, "405", "Method Not Allowed", "El método HTTP solicitado no está permitido en este servidor.", conf);
}

/********
 * FUNCIÓN: void send_409(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 409 (Conflict) al cliente.
 * ARGS_OUT: void.
 ********/
void send_409(int connfd, struct ServerConf conf) {
    send_error_response(connfd, "409", "Conflict", "El recurso solicitado ya existe en el servidor.", conf);
}

/********
 * FUNCIÓN: void send_201(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *          struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 201 (Created) al cliente.
 * ARGS_OUT: void.
 ********/
void send_201(int connfd, struct ServerConf conf) {
    send_error_response(connfd, "201", "Created", "El recurso solicitado fue creado exitosamente.", conf);
}

/********
 * FUNCIÓN: void send_400(int connfd, struct ServerConf conf)
 * ARGS_IN: int connfd - Descriptor de socket para la conexión.
 *         struct ServerConf conf - Configuración del servidor.
 * DESCRIPCIÓN: Envía una respuesta HTTP 400 (Bad Request) al cliente.
 * ARGS_OUT: void.
 ********/


void send_400(int connfd, struct ServerConf conf) {
    send_error_response(connfd, "400", "Bad Request", "La solicitud HTTP es incorrecta o incompleta.", conf);
}