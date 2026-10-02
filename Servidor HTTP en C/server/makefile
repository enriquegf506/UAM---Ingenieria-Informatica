CC = gcc
CFLAGS = -Wall -Wextra -Ilib
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
LIB = $(LIB_DIR)/libserver.a
TARGET = server

# Regla principal: crea tanto la librería como el ejecutable
all: $(LIB) $(TARGET)

# Crear el ejecutable vinculando con la librería
$(TARGET): server.c $(LIB)
	$(CC) $(CFLAGS) -o $(TARGET) server.c -L$(LIB_DIR) -lserver

# Crear la librería estática
$(LIB): $(OBJ_DIR) $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $(LIB) $(OBJS)

# Crear directorio de objetos
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilar archivos fuente a objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIB_DIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza de objetos
clean:
	rm -rf $(OBJ_DIR)

# Limpieza completa
fclean:
	rm -rf $(OBJ_DIR) $(LIB) $(TARGET)

# Reconstruir todo
re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re