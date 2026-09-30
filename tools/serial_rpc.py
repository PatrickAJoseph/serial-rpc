
import serial
import time
import yaml
import ast
from enum import Enum
import logging

class serial_rpc:

    class serial_rpc_parameter:
    
        def __init__(self, parameter_group: str, parameter_set_name: str, id: int, parameter_name: str, parameter_type: str, is_a_list: bool):
            
            self.value = 0
            self.parameter_name: str = parameter_name
            self.parameter_group: str = parameter_group
            self.parameter_set_name: str = parameter_set_name
            self.parameter_full_name: str = parameter_set_name + ":" + parameter_name
            self.parameter_type: str = parameter_type
            self.is_a_list:bool = is_a_list
            self.id = id
        
        def get(self):
            return self.value
        
        def set(self, value):
            self.value = value

    def log_parameter_info(self, parameter):
        self.logger.info("-----------------------------------------------")
        self.logger.info(f"Parameter set ID: {parameter.id}")
        self.logger.info(f"Parameter group: {parameter.parameter_group}")
        self.logger.info(f"Parameter set name: {parameter.parameter_set_name}")
        self.logger.info(f"Parameter name: {parameter.parameter_name}")
        self.logger.info(f"Parameter value: {parameter.value}")
        self.logger.info("-----------------------------------------------")


    def load_parameters(self):
        
        file_handle = open(self.parameter_file, 'r')
        yaml_file_data = yaml.safe_load(file_handle)
        
        for command in yaml_file_data['commands']:
        
            command_name = command['name']
            command_id = command['id']
        
            request_parameters = command['request_parameters']
            response_parameters = command['response_parameters']
        
            for request_parameter in request_parameters:
                request_parameter_name = request_parameter['name']
                request_parameter_is_a_list = request_parameter['is_a_list']
                request_parameter_type = request_parameter['type']
                
                self.request_parameters.append(self.serial_rpc_parameter(parameter_group = 'request', parameter_set_name = command_name, parameter_name = request_parameter_name, parameter_type = request_parameter_type, id = command_id, is_a_list = request_parameter_is_a_list))

            for response_parameter in response_parameters:
                response_parameter_name = response_parameter['name']
                response_parameter_is_a_list = response_parameter['is_a_list']
                response_parameter_type = response_parameter['type']
                
                self.response_parameters.append(self.serial_rpc_parameter(parameter_group = 'response', parameter_set_name = command_name, parameter_name = response_parameter_name, parameter_type = response_parameter_type, is_a_list= response_parameter_is_a_list, id = command_id))

        for notifications in yaml_file_data['notifications']:
            
            notification_name = notifications['name']
            notification_id = notifications['id']
            
            notification_parameters = notifications['notification_parameters']
            
            for notification_parameter in notification_parameters:
                
                notification_parameter_name = notification_parameter['name']
                notification_parameter_type = notification_parameter['type']
                notification_parameter_is_a_list = notification_parameter['is_a_list']
                
                self.notification_parameters.append(self.serial_rpc_parameter(parameter_group = 'notification', parameter_set_name = notification_name, parameter_name = notification_parameter_name, parameter_type = notification_parameter_type, is_a_list= notification_parameter_is_a_list, id = notification_id))                

        for parameter in self.request_parameters:
            self.log_parameter_info(parameter)

        for parameter in self.response_parameters:
            self.log_parameter_info(parameter)

        for parameter in self.notification_parameters:
            self.log_parameter_info(parameter)

    def __init__(self, port_name: str, baud_rate: int, parameter_file: str, target_address: int = 0, timeout_ms:int = 1000):

        logging.basicConfig( level = logging.DEBUG, format="%(asctime)s [%(levelname)s] %(name)s: %(message)s", filename = "serial_rpc.log", filemode = "w" )

        self.logger = logging.getLogger('serial_rpc_logger')
        
        self.serial_port: serial.Serial = serial.Serial(port = port_name, baudrate = baud_rate, timeout = float(timeout_ms) * float(0.001))
        self.request_parameters = []
        self.response_parameters = []
        self.notification_parameters = []
        self.parameter_file: str = parameter_file
        self.target_address = target_address
        
        self.load_parameters()
        
        self.logger.debug("RPC initialized")
    