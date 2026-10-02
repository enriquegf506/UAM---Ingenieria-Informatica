# Servidor HTTP con Pool de Hilos

**Grupo 2301-2321**  
**Pareja 06**  
**Integrantes:**  
- Daniel Aquino Santiago  
- Enrique Gómez Fernández  

## Descripción

Este proyecto implementa un servidor HTTP en C con soporte para múltiples conexiones mediante un pool de hilos. Su objetivo es manejar solicitudes concurrentes de manera eficiente, evitando la sobrecarga de creación y destrucción de hilos en cada petición.

El servidor es capaz de:  
- Escuchar en un puerto específico y aceptar conexiones entrantes.  
- Delegar el procesamiento de solicitudes a un conjunto predefinido de hilos trabajadores.  
- Servir archivos estáticos desde un directorio especificado.  
- Manejar respuestas adecuadas según el estado de la solicitud.  

El diseño del servidor sigue un enfoque modular para facilitar su mantenimiento y expansión.  

