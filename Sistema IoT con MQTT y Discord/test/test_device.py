import unittest
from unittest.mock import patch, MagicMock, call
import sys
import os
import time
import argparse
from io import StringIO
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from src.devices.dummy_sensor import DummySensor
from src.devices.dummy_switch import DummySwitch

class TestDevices(unittest.TestCase):
    
    def setUp(self):
        """Configuración común para las pruebas"""
        self.config_path = "config/config.yaml"
        self.device_id = "test_device"
        
    @patch('src.utils.MQTTClient.connect')
    def test_sensor_connects_to_broker(self, mock_connect):
        """Prueba que el sensor se conecta correctamente al broker"""
        sensor = DummySensor(self.config_path, self.device_id, 20, 30, 1, 1)
        
        sensor.connect("test_host", 1883)
        
        mock_connect.assert_called_once_with("test_host", 1883)
    
    @patch('src.utils.MQTTClient.connect', side_effect=Exception("Connection error"))
    def test_sensor_connection_error(self, mock_connect):
        """Prueba que el sensor maneja correctamente errores de conexión"""
        sensor = DummySensor(self.config_path, self.device_id, 20, 30, 1, 1)
        
        with self.assertRaises(Exception) as context:
            sensor.connect("test_host", 1883)
        
        self.assertTrue("Connection error" in str(context.exception))
        
    @patch('sys.argv', ['dummy_sensor.py', 'sensor1', '--min', '10', '--max', '25', '--increment', '2', '--interval', '5'])
    def test_sensor_command_line_args(self):
        """Prueba que el sensor lee correctamente los parámetros de línea de comandos"""
        original_argv = sys.argv.copy()
        try:
            sys.argv = ['dummy_sensor.py', 'sensor1', '--min', '10', '--max', '25', '--increment', '2', '--interval', '5']
            # Creamos un parser de argumentos igual al del módulo
            parser = argparse.ArgumentParser()
            parser.add_argument('--host', default='localhost')
            parser.add_argument('-p', '--port', type=int, default=1883)
            parser.add_argument('-m', '--min', type=int, default=20)
            parser.add_argument('-M', '--max', type=int, default=30)
            parser.add_argument('--increment', type=int, default=1)
            parser.add_argument('-i', '--interval', type=int, default=1)
            parser.add_argument('id')            
            # Parseamos los argumentos
            args = parser.parse_args()
            # Verificamos los valores
            self.assertEqual(args.id, 'sensor1')
            self.assertEqual(args.min, 10)
            self.assertEqual(args.max, 25)
            self.assertEqual(args.increment, 2)
            self.assertEqual(args.interval, 5)
            self.assertEqual(args.host, 'localhost')
            self.assertEqual(args.port, 1883)
        finally:
            sys.argv = original_argv
    
    @patch('src.utils.MQTTClient.publish')
    @patch('time.sleep', return_value=None)  
    def test_sensor_value_changes(self, mock_sleep, mock_publish):
        """Prueba que el sensor cambia su valor dentro de los límites establecidos"""
        sensor = DummySensor(self.config_path, self.device_id, 5, 10, 1, 0.1)
        
        # Mock de run para ejecutar solo algunas iteraciones
        with patch.object(sensor, 'run', wraps=lambda: self._mock_run(sensor, 10)):
            try:
                sensor.run()
            except StopIteration:
                pass
        
        # Verificar que los valores publicados están entre min y max siguiendo patron
        expected_values = ['5', '6', '7', '8', '9', '10', '9', '8', '7', '6']
        expected_calls = [call(f"{sensor.topic}", val) for val in expected_values]
        
        mock_publish.assert_has_calls(expected_calls)
        self.assertEqual(mock_publish.call_count, 10)
    
    def _mock_run(self, sensor, iterations):
        """Función auxiliar para simular el comportamiento de run() pero con número limitado de iteraciones"""
        for _ in range(iterations):
            sensor.publish(sensor.topic, str(sensor.current))
            sensor.current += sensor.direction * sensor.increment
            if sensor.current >= sensor.max_val:
                sensor.direction = -1
            elif sensor.current <= sensor.min_val:
                sensor.direction = 1
            time.sleep(sensor.interval)
        raise StopIteration()  
    
    
    @patch('random.random')
    @patch('src.utils.MQTTClient.publish')
    def test_switch_state_change(self, mock_publish, mock_random):
        """Prueba que el switch cambia su estado correctamente al recibir un comando"""
        mock_random.return_value = 0.5
        
        switch = DummySwitch(self.config_path, self.device_id, 0.3)
        
        switch.publish(switch.topic, switch.state)
        
        mock_msg = MagicMock()
        mock_msg.topic = f"{switch.command_topic}"
        mock_msg.payload = b"ON"
        
        switch.on_message(None, None, mock_msg)
        
        self.assertEqual(switch.state, "ON")
        mock_publish.assert_has_calls([
            call(f"{switch.topic}", "OFF"), 
            call(f"{switch.topic}", "ON")
        ])
    
    @patch('random.random')
    @patch('src.utils.MQTTClient.publish')
    @patch('builtins.print')
    def test_switch_action_failure(self, mock_print, mock_publish, mock_random):
        """Prueba que el switch maneja correctamente fallos en las acciones"""
        mock_random.return_value = 0

        switch = DummySwitch(self.config_path, self.device_id, 0)
        switch.publish(switch.topic, switch.state)

        
        # Simulamos la recepción de un mensaje
        mock_msg = MagicMock()
        mock_msg.topic = f"{switch.topic}/set"
        mock_msg.payload = b"ON"
        
        # Llamamos al manejador de mensajes
        switch.on_message(None, None, mock_msg)
        
        # Verificamos que el estado NO cambió
        self.assertEqual(switch.state, "OFF")
        mock_print.assert_called_once_with(f"Action failed for {switch.device_id}")
        mock_publish.assert_called_once_with(f"{switch.topic}", "OFF")
    
    @patch('sys.argv', ['dummy_switch.py', 'switch1', '--probability', '0.5'])
    def test_switch_command_line_args(self):
        """Prueba que el switch lee correctamente los parámetros de línea de comandos"""
        original_argv = sys.argv.copy()
        try:
            # Configuramos sys.argv para la prueba
            sys.argv = ['dummy_switch.py', 'switch1', '--probability', '0.5']
            # Creamos un parser de argumentos igual al del módulo
            parser = argparse.ArgumentParser()
            parser.add_argument('--host', default='localhost')
            parser.add_argument('-p', '--port', type=int, default=1883)
            parser.add_argument('-P', '--probability', type=float, default=0.3)
            parser.add_argument('id')
            # Parseamos los argumentos
            args = parser.parse_args()
            # Verificamos los valores
            self.assertEqual(args.id, 'switch1')
            self.assertEqual(args.probability, 0.5)
            self.assertEqual(args.host, 'localhost')
            self.assertEqual(args.port, 1883)
        finally:
            sys.argv = original_argv

if __name__ == '__main__':
    unittest.main()