import os
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from googleapiclient.discovery import build
from googleapiclient.http import MediaFileUpload, MediaIoBaseDownload

class GoogleDriveHandler:
    """
    Clase para manejar la subida y descarga de archivos en Google Drive
    """
    SCOPES = ['https://www.googleapis.com/auth/drive.file']
    
    def __init__(self, credentials_path='credentials.json'):
        """
        Inicializa el manejador de Google Drive
        """
        self.credentials_path = credentials_path
        self.creds = None
        self.service = None
        self.file_id = {}
        
    def authenticate(self):
        """
        Autentica el usuario con Google Drive
        """
        if os.path.exists('token.json'):
            self.creds = Credentials.from_authorized_user_file('token.json', self.SCOPES)
            
        if not self.creds or not self.creds.valid:
            if self.creds and self.creds.expired and self.creds.refresh_token:
                self.creds.refresh(Request())
            else:
                flow = InstalledAppFlow.from_client_secrets_file(self.credentials_path, self.SCOPES)
                self.creds = flow.run_local_server(port=0)
                
            with open('token.json', 'w') as token:
                token.write(self.creds.to_json())
                
        self.service = build('drive', 'v3', credentials=self.creds)
        
    def _get_file_id(self, filename):
        """
        Busca el ID del archivo por nombre en Google Drive
        """
        try:
            results = self.service.files().list(
                q=f"name='{filename}'",
                spaces='drive',
                fields='files(id, name)'
            ).execute()
            files = results.get('files', [])
            
            if files:
                return files[0]['id']
            return None
        except Exception as e:
            print(f"Error buscando archivo en Drive: {e}")
            return None

    def upload(self, file_path):
        """
        Sube o actualiza un archivo en Google Drive
        """
        try:
            filename = os.path.basename(file_path)
            print(f"Preparando para subir {filename} a Google Drive...")
            
            file_metadata = {'name': filename}
            media = MediaFileUpload(file_path, resumable=True)
            
            file_id = self._get_file_id(filename)
            
            if file_id:
                print(f"Actualizando archivo existente en Drive...")
                file = self.service.files().update(
                    fileId=file_id,
                    body=file_metadata,
                    media_body=media,
                    fields='id'
                ).execute()
            else:
                print(f"Creando nuevo archivo en Drive...")
                file = self.service.files().create(
                    body=file_metadata,
                    media_body=media,
                    fields='id'
                ).execute()
            
            file_id = file.get('id')
            self.file_id[filename] = file_id
            print(f"Archivo subido exitosamente. ID: {file_id}")
            return True
            
        except Exception as e:
            print(f"Error subiendo archivo a Drive: {e}")
            return False

    def download(self, filename, destination_path):
        """
        Descarga un archivo de Google Drive
        """
        try:
            print(f"Buscando {filename} en Google Drive...")
            
            file_id = self._get_file_id(filename)
            
            if not file_id:
                print(f"No se encontró el archivo {filename} en Drive")
                return False
            
            print(f"Descargando archivo con ID: {file_id}")
            request = self.service.files().get_media(fileId=file_id)
            
            with open(destination_path, 'wb') as f:
                downloader = MediaIoBaseDownload(f, request)
                done = False
                while done is False:
                    status, done = downloader.next_chunk()
                    print(f"Descarga: {int(status.progress() * 100)}%")
            
            print("Descarga completada exitosamente")
            return True
            
        except Exception as e:
            print(f"Error descargando archivo de Drive: {e}")
            return False
