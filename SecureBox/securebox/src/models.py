from datetime import datetime

class ContainerVersion:
    """
    Representa una versión de un contenedor
    """
    def __init__(self, content, timestamp=None):
        """
        Inicializa una nueva versión
        """
        self.content = content
        self.timestamp = timestamp or datetime.now()
        
    def to_dict(self):
        """
        Convierte la versión a diccionario para serialización
        """
        return {
            'content': self.content,
            'timestamp': self.timestamp.isoformat()
        }
        
    @classmethod
    def from_dict(cls, data):
        """
        Crea una versión desde un diccionario
        """
        return cls(
            content=data['content'],
            timestamp=datetime.fromisoformat(data['timestamp'])
        )

class Container:
    """
    Representa un contenedor de contenido con historial de versiones
    """
    def __init__(self, name, content=""):
        """
        Inicializa un nuevo contenedor
        """
        self.name = name
        self.content = content
        self.versions = [ContainerVersion(content)]
    
    def edit(self, new_content):
        """
        Edita el contenido y crea una nueva versión
        """
        self.content = new_content
        self.versions.append(ContainerVersion(new_content))
    
    def get_history(self):
        """
        Obtiene el historial de versiones
        """
        return [(v.timestamp, v.content) for v in self.versions]
    
    def restore_version(self, timestamp):
        """
        Restaura una versión específica
        """
        for version in self.versions:
            if version.timestamp == timestamp:
                self.content = version.content
                return True
        return False
    
    def to_dict(self):
        """
        Convierte el contenedor a diccionario para serialización
        """
        return {
            'name': self.name,
            'content': self.content,
            'versions': [v.to_dict() for v in self.versions]
        }
    
    @classmethod
    def from_dict(cls, data):
        """
        Crea un contenedor desde un diccionario
        """
        container = cls(data['name'], data['content'])
        container.versions = [ContainerVersion.from_dict(v) for v in data['versions']]
        return container
