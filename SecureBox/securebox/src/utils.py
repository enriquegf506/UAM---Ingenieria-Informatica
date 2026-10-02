import re

def validar_contraseña(password):
    """
    Valida una contraseña según los siguientes criterios:
    - Debe tener al menos 10 caracteres.
    - Debe contener al menos una letra mayúscula.
    - Debe contener al menos una letra minúscula.
    - Debe contener al menos un número.
    - Debe contener al menos un carácter especial (por ejemplo, un punto).
    """
    errores = []

    if len(password) < 10:
        errores.append("La contraseña debe tener más de 10 caracteres.")
    if not re.search(r"[A-Z]", password):
        errores.append("La contraseña debe contener al menos una letra mayúscula.")
    if not re.search(r"[a-z]", password):
        errores.append("La contraseña debe contener al menos una letra minúscula.")
    if not re.search(r"\d", password):
        errores.append("La contraseña debe contener al menos un número.")
    if not re.search(r"[\.]", password):
        errores.append("La contraseña debe contener al menos un carácter especial (por ejemplo, un punto).")

    return errores