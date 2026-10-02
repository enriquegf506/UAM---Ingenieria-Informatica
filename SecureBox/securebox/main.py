import getpass
import os
import sys
from src.securebox import SecureBox
from src.utils import validar_contraseña


def main():
    """
    Función principal que implementa la interfaz de línea de comandos.
    """
    max_attempts = 3
    attempts = 0

    while True:
        username = input("Nombre de usuario: ")
        if username:
            break
        print("El nombre de usuario no puede estar vacío.")

    vault = SecureBox(username)
    
    while attempts < max_attempts:
        password = getpass.getpass("Contraseña maestra: ")
        
        if not SecureBox.user_exists(username):
            print("\nNo se encontró un vault existente. Creando uno nuevo...")
            confirm_password = getpass.getpass("Confirme la contraseña maestra: ")
            
            if password != confirm_password:
                print("Las contraseñas no coinciden. Por favor, intente de nuevo.")
                continue
            
            errores = validar_contraseña(password)
            if errores:
                print("La contraseña no cumple con los siguientes requisitos:")
                for error in errores:
                    print(f"- {error}")
                continue 

            vault.initialize(password)
            print("Nuevo vault creado exitosamente.")
            break
            
        success, error_msg = vault.unlock(password)
        if success:
            print("Vault desbloqueado correctamente.")
            break
            
        print(f"Error: {error_msg}")
        attempts += 1
        
        if attempts < max_attempts:
            print(f"Intentos restantes: {max_attempts - attempts}")
        
    if attempts >= max_attempts:
        print("Número máximo de intentos alcanzado. Por favor, inténtelo más tarde.")
        return
    
    while True:
        print("\nSecureBox - Menú Principal")
        print("1. Listar contenedores")
        print("2. Crear contenedor")
        print("3. Editar contenedor")
        print("4. Ver contenedor")
        print("5. Eliminar contenedor")
        print("6. Ver historial de versiones")
        print("7. Restaurar versión")
        print("8. Backup manual a Google Drive")
        print("9. Restaurar desde Google Drive")
        print("10. Salir")
        
        choice = input("\nElija una opción: ")
        
        if choice == "1":
            containers = vault.list_containers()
            if containers:
                print("\nContenedores disponibles:")
                for i, name in enumerate(containers, 1):
                    print(f"{i}. {name}")
            else:
                print("\nNo hay contenedores.")
                
        elif choice == "2":
            name = input("Nombre del nuevo contenedor: ")
            if vault.create_container(name):
                print("Contenedor creado.")
            else:
                print("El contenedor ya existe.")
                
        elif choice == "3":
            name = input("Nombre del contenedor a editar: ")
            if not vault.edit_container(name):
                print("El contenedor no existe.")
                
        elif choice == "4":
            name = input("Nombre del contenedor a ver: ")
            content = vault.view_container(name)
            if content is not None:
                print(f"\nContenido de {name}:")
                print(content)
            else:
                print("El contenedor no existe.")
                
        elif choice == "5":
            name = input("Nombre del contenedor a eliminar: ")
            if vault.delete_container(name):
                print("Contenedor eliminado.")
            else:
                print("El contenedor no existe.")
                
        elif choice == "6":
            name = input("Nombre del contenedor: ")
            history = vault.get_container_history(name)
            if history:
                print(f"\nHistorial de versiones para {name}:")
                for i, (timestamp, content) in enumerate(history, 1):
                    print(f"\nVersión {i} - {timestamp}:")
                    print(f"Contenido: {content[:50]}..." if len(content) > 50 else f"Contenido: {content}")
            else:
                print("No se encontró historial para ese contenedor.")
                
        elif choice == "7":
            name = input("Nombre del contenedor: ")
            history = vault.get_container_history(name)
            if history:
                print(f"\nVersiones disponibles para {name}:")
                for i, (timestamp, _) in enumerate(history, 1):
                    print(f"{i}. {timestamp}")
                try:
                    version_idx = int(input("\nSeleccione el número de versión a restaurar: ")) - 1
                    if 0 <= version_idx < len(history):
                        timestamp = history[version_idx][0]
                        if vault.restore_version(name, timestamp):
                            print("Versión restaurada exitosamente.")
                        else:
                            print("Error al restaurar la versión.")
                    else:
                        print("Número de versión inválido.")
                except ValueError:
                    print("Por favor, introduzca un número válido.")
            else:
                print("No se encontró historial para ese contenedor.")
                
        elif choice == "8":
            print("Realizando backup manual a Google Drive...")
            if vault._backup_to_drive():
                print("Backup completado exitosamente.")
            else:
                print("Error al realizar el backup.")
                
        elif choice == "9":
            print("Restaurando desde Google Drive...")
            if vault.restore_from_drive():
                print("Restauración completada exitosamente.")

                attempts = 0
                while attempts < max_attempts:
                    password = getpass.getpass("Ingrese la contraseña del vault restaurado: ")
                    vault = SecureBox(username)

                    success, error_msg = vault.unlock(password)
                    if success:
                        print("Vault restaurado y desbloqueado correctamente.")
                        break

                    print(f"Error: {error_msg}")
                    attempts += 1

                    if attempts < max_attempts:
                        print(f"Intentos restantes: {max_attempts - attempts}")

                if attempts >= max_attempts:
                    print("Número máximo de intentos alcanzado. No se puede acceder al vault restaurado.")
                    return
            else:
                print("Error al restaurar desde Google Drive.")

                
        elif choice == "10":
            print("Gracias por usar SecureBox. ¡Hasta pronto!")
            break
            
        else:
            print("Opción no válida. Por favor, intente de nuevo.")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nGracias por usar SecureBox. ¡Hasta pronto!")
        sys.exit(0)