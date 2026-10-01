
import serial_rpc
import time

handle = serial_rpc.serial_rpc(port_name = 'COM8', baud_rate = 115200, parameter_file = 'test_parameter_file.yaml')

for i in range(0, 1000):
    handle.send_request('button_0_status')
    handle.send_request('button_1_status')
    
    button_0_state = handle.get_response_parameter_value('button_0_status', 'button_0_state')
    button_0_press_count = handle.get_response_parameter_value('button_0_status', 'button_0_press_count')
    button_1_state = handle.get_response_parameter_value('button_1_status', 'button_1_state')
    button_1_press_count = handle.get_response_parameter_value('button_1_status', 'button_1_press_count')
   
    print(f"Button 0 state: {button_0_state}, Button 0 press count: {button_0_press_count}, Button 1 state: {button_1_state}, Button 1 press count: {button_1_press_count}")
    
    time.sleep(0.01)