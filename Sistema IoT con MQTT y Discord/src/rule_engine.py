import re
import logging
from utils import Persistence
import requests
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

logger = logging.getLogger('controller.RuleEngine')

"""
    Motor de reglas para evaluar condiciones y ejecutar acciones en dispositivos IoT.
    
    Esta clase permite definir reglas que se activan cuando se cumplen ciertas condiciones
    en los dispositivos. Las acciones pueden incluir cambiar el estado de un dispositivo
    o enviar notificaciones a través de una API.
    
    Attributes:
        base_topic (str): Prefijo de topic para los dispositivos.
        persistence (Persistence): Objeto de persistencia para almacenar estados y eventos.
        api_url (str): URL de la API de notificaciones para enviar eventos.
"""
class RuleEngine:
     
    def __init__(self, base_topic, persistence, api_url="http://localhost:5000"):
        self.persistence = persistence
        self.base_topic = base_topic
        self.api_url = api_url
        self.logger = logging.getLogger('controller.RuleEngine')
    

    def evaluate_rules(self, device_id, state, publish_callback):
        """
        Evalúa todas las reglas para un dispositivo y estado dados.
        
        Args:
            device_id: ID del dispositivo que ha cambiado de estado
            state: El nuevo estado del dispositivo
            publish_callback: Función para publicar mensajes MQTT
        """
        self.logger.info(f"Evaluating rules for device {device_id} with state {state}")
        for rule in self.persistence.get_rules():
            self.logger.debug(f"Checking rule: {rule}")
            if self.evaluate_rule(rule, device_id, state):
                self.logger.info(f"Rule matched: {rule['condition']} -> {rule['action']}")
                self.execute_action(rule['action'], publish_callback)
    
    def evaluate_rule(self, rule, device_id, state):
        """
        Evalúa si una regla debe ser ejecutada basada en la condición y el estado actual.
        
        Args:
            rule: Regla a evaluar
            device_id: ID del dispositivo
            state: Estado actual del dispositivo
            
        Returns:
            bool: True si la regla debe ejecutarse, False en caso contrario
        """
        match = re.match(r"if (\w+) (\d+) ([><=]) (\d+|\d{2}:\d{2}:\d{2})", rule['condition'])
        if not match:
            self.logger.warning(f"Invalid rule condition format: {rule['condition']}")
            return False
        
        dev_type, dev_id, op, value = match.groups()
        full_dev_id = f"{self.base_topic}/{dev_id}"
        
        if full_dev_id != device_id:
            self.logger.debug(f"Device ID mismatch: {full_dev_id} != {device_id}")
            return False
            
        try:
            if dev_type == "sensor":
                state_val = float(state)
                value = float(value)
            elif dev_type == "clock":
                state_val = state
                value = value
            else:
                self.logger.warning(f"Unsupported device type in rule: {dev_type}")
                return False
                
            if op == ">":
                result = state_val > value
                self.logger.debug(f"Evaluating {state_val} > {value}: {result}")
                return result
            elif op == "<":
                result = state_val < value
                self.logger.debug(f"Evaluating {state_val} < {value}: {result}")
                return result
            elif op == "=":
                result = state_val == value
                self.logger.debug(f"Evaluating {state_val} = {value}: {result}")
                return result
        except ValueError as e:
            self.logger.error(f"Error evaluating rule: {e}")
            return False
            
        return False
    
    def execute_action(self, action, publish_callback):
        """
        Ejecuta la acción definida en una regla.
        
        Args:
            action: Acción a ejecutar
            publish_callback: Función para publicar mensajes MQTT
        """
        match = re.match(r"switch (\d+) (\w+)", action)
        if match:
            dev_id, target_state = match.groups()
            full_device_id = f"{self.base_topic}/{dev_id}"

            current_state = self.get_device_state(full_device_id)
            if current_state == target_state:
                self.logger.info(f"Rule triggered, but device {dev_id} already in state {target_state}")
                requests.post(f"{self.api_url}/event", json={
                    "device_id": full_device_id,
                    "state": f"Rule triggered, but device already in state {target_state}"
                })
            else:
                if self.persistence.update_device_state(full_device_id, target_state):
                    self.persistence.log_event(full_device_id, target_state)
                    self.logger.info(f"Device {dev_id} updated to {target_state}")
                    publish_callback(f"{full_device_id}/set", target_state)
                else:
                    self.logger.error(f"Failed to update state for device {dev_id}")
    
    def get_device_state(self, device_id):
        """
        Obtiene el estado actual de un dispositivo.
        
        Args:
            device_id: ID del dispositivo
            
        Returns:
            str: Estado actual del dispositivo o None si no existe
        """
        devices = self.persistence.get_devices()
        for d in devices:
            if d["id"] == device_id:
                return d.get("state")
        self.logger.warning(f"Device not found: {device_id}")
        return None
    
    def add_rule(self, condition, action):
        """
        Añade una nueva regla.
        
        Args:
            condition: Condición de la regla
            action: Acción a ejecutar cuando se cumpla la condición
        """
        self.persistence.add_rule(condition, action)
        self.logger.info(f"Added rule: {condition} -> {action}")
    
    def get_rules(self):
        """
        Obtiene todas las reglas.
        
        Returns:
            list: Lista de reglas
        """
        rules = self.persistence.get_rules()
        self.logger.debug(f"Retrieved {len(rules)} rules")
        return rules