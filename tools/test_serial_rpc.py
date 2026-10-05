
import serial_rpc
import time

def button_0_notification_callback(notification_id, args, parameters):
    print(f"Button 0 pressed ! Press count : {parameters['button_press_count']}")

def button_1_notification_callback(notification_id, args, parameters):
    print(f"Button 1 pressed ! Press count : {parameters['button_press_count']}")

handle = serial_rpc.serial_rpc(port_name = 'COM8', baud_rate = 115200, parameter_file = 'test_parameter_file.yaml')

handle.set_request_parameter_value('test_u8', 'length', 4)
handle.set_request_parameter_value('test_u8', 'values', [1,2,3,4])
handle.send_request('test_u8')