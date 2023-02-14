Tareas llevadas a cabo en la entrega extraordinaria:
-GANTTS: Se ha realizado un diagrama de gant final.
-MAKEFILE: Se ha cambiado el makefile lo máximo que he podido intentando poner todas las flags posibles, añadiendo macros, utilizando el .PHONY y eliminando la opción -g de compilacion. 
-DOCUMENTACIÓN: A su vez he intentado arreglar toda la documentación que faltaba o que era errónea tanto en macros como EDs y campos como _Object y _Player. También he documentado las funciones privadas que faltaban.
-ATTACK: se ha cambiado el valor de MAX_RAND  a 10 para que el valor de ataque esté entre 0-9. También cambié srand(time(NULL)) para que solo se invoque una vez en el programa(main).
-TAKE: El comando take ha sido modificado para que los objetos solo se puedan coger escribiendo O delante del identificador, como se pide en el enunciado.
-SET: Se ha cambiado la funcion Set_findId(), para que en un set llegue a haber MAX_SETIDS. Se ha modificado set_delete para que se compacte el contenido del array de ids, que quedaran al principio del mismo, cada vez que se quiera eliminar una id.
-INTERFAZ: Se ha modificado el graphic engine para que se muestren las conexiones a la izquierda. Se indica el resultado de los comandos ejecutados.

