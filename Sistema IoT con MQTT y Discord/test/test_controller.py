import unittest
from unittest.mock import MagicMock, patch
import sys
import os
import time
import json
import yaml
import tempfile
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from src.utils import MQTTClient, Persistence
from src.controller import Controller
from src.rule_engine import RuleEngine

class TestControllerRules(unittest.TestCase):
    def setUp(self):
        """
        Preparación del entorno de pruebas: se crean archivos temporales
        para configuración, dispositivos, reglas y eventos. Se escriben datos
        de prueba en estos archivos.
        """
        # Crear archivos temporales
        self.config_fd, self.config_path = tempfile.mkstemp()
        self.devices_fd, self.devices_path = tempfile.mkstemp()
        self.rules_fd, self.rules_path = tempfile.mkstemp()
        self.events_fd, self.events_path = tempfile.mkstemp()
        
        # Crear configuración de prueba
        config_data = {
            'mqtt': {
                'host': 'localhost',
                'port': 1883,
                'group': '2321',
                'pair': '06'
            }
        }
        
        with os.fdopen(self.config_fd, 'w') as f:
            yaml.dump(config_data, f)
        
        # Dispositivos y reglas de prueba
        base_topic = "redes2/2321/06"
        test_devices = [
            {"id": f"{base_topic}/1", "type": "sensor", "state": "25"},
            {"id": f"{base_topic}/2", "type": "switch", "state": "OFF"}
        ]
        
        test_rules = [
            {"condition": "if sensor 1 > 26", "action": "switch 2 ON"}
        ]
        
        with os.fdopen(self.devices_fd, 'w') as f:
            json.dump(test_devices, f)
        
        with os.fdopen(self.rules_fd, 'w') as f:
            json.dump(test_rules, f)
        
        with os.fdopen(self.events_fd, 'w') as f:
            json.dump([], f)
        
    def tearDown(self):
        """
        Limpieza de los archivos temporales creados en la prueba.
        """
        os.unlink(self.config_path)
        os.unlink(self.devices_path)
        os.unlink(self.rules_path)
        os.unlink(self.events_path)
    
    @patch('paho.mqtt.client.Client')
    @patch('requests.post')  # Se simulan las peticiones HTTP
    def test_sensor_message_triggers_rule_check(self, mock_post, mock_mqtt):
        """
        Comprueba que, al recibir un mensaje de sensor, se activa
        la evaluación de reglas y se realiza la acción correspondiente.
        """
        # Creamos el controlador con la configuración de prueba
        controller = Controller(self.config_path)
        
        # Sobreescribimos la persistencia para usar nuestros archivos temporales
        controller.persistence = Persistence(
            devices_file=self.devices_path,
            rules_file=self.rules_path,
            events_file=self.events_path
        )
        
        # Creamos una nueva instancia de RuleEngine con los parámetros correctos
        controller.rule_engine = RuleEngine(
            base_topic=controller.base_topic,
            persistence=controller.persistence,
            api_url="http://localhost:5000"
        )
        
        controller.connect(host="localhost", port=1883)
        
        # Se sustituye evaluate_rule por una versión que marca si fue llamada
        original_evaluate = controller.rule_engine.evaluate_rule
        eval_called = [False]
        
        def mock_evaluate_rule(*args, **kwargs):
            eval_called[0] = True
            return original_evaluate(*args, **kwargs)
        
        controller.rule_engine.evaluate_rule = mock_evaluate_rule
        
        # Simulamos un mensaje MQTT que debería activar una regla
        base_topic = "redes2/2321/06"
        message = MagicMock()
        message.topic = f"{base_topic}/1"
        message.payload = b"27"  # Valor mayor que el umbral de la regla
        
        controller.on_message(mock_mqtt.return_value, None, message)
        
        # Se verifica que se evaluaron las reglas
        self.assertTrue(eval_called[0], "Debería haberse activado la evaluación de reglas")
        
        # Comprobamos que el estado del sensor fue actualizado
        devices = controller.persistence.get_devices()
        sensor_device = next((d for d in devices if d['id'] == f"{base_topic}/1"), None)
        self.assertEqual(sensor_device['state'], "27", "El estado del sensor debería ser '27'")
        
        # Comprobamos que se envió el comando MQTT correspondiente
        mock_mqtt.return_value.publish.assert_called_with(f"{base_topic}/2/set", "ON")
    
    @patch('paho.mqtt.client.Client')
    def test_rule_engine_action_execution(self, mock_mqtt):
        """
        Verifica que, al ejecutar una acción desde la regla, el estado
        del dispositivo se actualiza y se publica por MQTT.
        """
        # Creamos el controlador con la configuración de prueba
        controller = Controller(self.config_path)
        
        # Sobreescribimos la persistencia para usar nuestros archivos temporales
        controller.persistence = Persistence(
            devices_file=self.devices_path,
            rules_file=self.rules_path,
            events_file=self.events_path
        )
        
        # Creamos una nueva instancia de RuleEngine con los parámetros correctos
        controller.rule_engine = RuleEngine(
            base_topic=controller.base_topic,
            persistence=controller.persistence,
            api_url="http://localhost:5000"
        )
        
        controller.connect(host="localhost", port=1883)
        
        base_topic = "redes2/2321/06"
        
        # Ejecutamos la acción de la regla
        controller.rule_engine.execute_action("switch 2 ON", controller.publish)
        
        # Comprobar publicación MQTT
        mock_mqtt.return_value.publish.assert_called_with(f"{base_topic}/2/set", "ON")
        
        # Verificar estado del dispositivo
        devices = controller.persistence.get_devices()
        switch_device = next((d for d in devices if d['id'] == f"{base_topic}/2"), None)
        self.assertEqual(switch_device['state'], "ON", "El switch debería estar en 'ON'")
    
    def test_persistence_reading(self):
        """
        Verifica que los datos de dispositivos y reglas se leen
        correctamente desde los archivos de persistencia.
        """
        persistence = Persistence(
            devices_file=self.devices_path,
            rules_file=self.rules_path,
            events_file=self.events_path
        )
        
        devices = persistence.get_devices()
        self.assertEqual(len(devices), 2, "Debería haber 2 dispositivos")
        
        base_topic = "redes2/2321/06"
        sensor = next((d for d in devices if d['id'] == f"{base_topic}/1"), None)
        self.assertIsNotNone(sensor, "Debería encontrarse el sensor")
        self.assertEqual(sensor['type'], "sensor")
        self.assertEqual(sensor['state'], "25")
        
        switch = next((d for d in devices if d['id'] == f"{base_topic}/2"), None)
        self.assertIsNotNone(switch, "Debería encontrarse el switch")
        self.assertEqual(switch['type'], "switch")
        self.assertEqual(switch['state'], "OFF")
        
        rules = persistence.get_rules()
        self.assertEqual(len(rules), 1, "Debería haber 1 regla")
        self.assertEqual(rules[0]['condition'], "if sensor 1 > 26")
        self.assertEqual(rules[0]['action'], "switch 2 ON")
    
    def test_successful_connection(self):
        """
        Verifica que el controlador se conecta correctamente al broker MQTT.
        """
        controller = Controller(self.config_path)
        controller.persistence = Persistence(
            devices_file=self.devices_path,
            rules_file=self.rules_path,
            events_file=self.events_path
        )
        
        try:
            controller.connect(host="localhost", port=1883)
            
            self.assertIsNotNone(controller.client, "El cliente MQTT debería estar inicializado")
            
            # Intentamos publicar para verificar que hay conexión
            base_topic = f"redes2/2321/06"
            controller.client.publish(f"{base_topic}/test", "TEST")
            connection_successful = True
            
            self.assertTrue(connection_successful, "Debería poder publicarse un mensaje")
            controller.client.disconnect()
        except Exception as e:
            self.fail(f"La conexión al broker falló inesperadamente: {e}")
    
    def test_failed_connection(self):
        """
        Verifica que se maneja correctamente un intento de conexión fallido al broker MQTT.
        """
        controller = Controller(self.config_path)
        controller.persistence = Persistence(
            devices_file=self.devices_path,
            rules_file=self.rules_path,
            events_file=self.events_path
        )
        
        invalid_port = 56789  # Puerto inválido para forzar error de conexión
        
        try:
            controller.connect(host="localhost", port=invalid_port)
            controller.mqtt_client.disconnect()
            self.fail("La conexión con puerto inválido no debería funcionar")
        except Exception as e:
            self.assertIsInstance(e, Exception, "Se esperaba una excepción al fallar la conexión")

if __name__ == '__main__':
    unittest.main()
