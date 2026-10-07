
import serial_rpc
import time

def button_0_notification_callback(notification_id, args, parameters):
    print(f"Button 0 pressed ! Press count : {parameters['button_press_count']}")

def button_1_notification_callback(notification_id, args, parameters):
    print(f"Button 1 pressed ! Press count : {parameters['button_press_count']}")

def test_u8_notification_callback(notification_id, args, parameters):
    print(f"test_u8 parameters: {parameters}")

def test_i8_notification_callback(notification_id, args, parameters):
    print(f"test_i8 parameters: {parameters}")

def test_u16_notification_callback(notification_id, args, parameters):
    print(f"test_u16 parameters: {parameters}")

def test_i16_notification_callback(notification_id, args, parameters):
    print(f"test_i16 parameters: {parameters}")

def test_u32_notification_callback(notification_id, args, parameters):
    print(f"test_u32 parameters: {parameters}")

def test_i32_notification_callback(notification_id, args, parameters):
    print(f"test_i32 parameters: {parameters}")

def test_float_notification_callback(notification_id, args, parameters):
    print(f"test_float parameters: {parameters}")

handle = serial_rpc.serial_rpc(port_name = 'COM8', baud_rate = 115200, parameter_file = 'test_parameter_file.yaml')

handle.register_notification_callback('test_notification_u8', test_u8_notification_callback, None)
handle.register_notification_callback('test_notification_i8', test_i8_notification_callback, None)
handle.register_notification_callback('test_notification_u16', test_u16_notification_callback, None)
handle.register_notification_callback('test_notification_i16', test_i16_notification_callback, None)
handle.register_notification_callback('test_notification_u32', test_u32_notification_callback, None)
handle.register_notification_callback('test_notification_i32', test_i32_notification_callback, None)
handle.register_notification_callback('test_notification_float', test_float_notification_callback, None)

handle.set_request_parameter_value('test_u8', 'length', 4)
handle.set_request_parameter_value('test_u8', 'values', [1,2,3,4])
handle.send_request('test_u8')
print(handle.get_response_parameter_value('test_u8', 'length'))
print(handle.get_response_parameter_value('test_u8', 'values'))

handle.set_request_parameter_value('test_i8', 'length', 4)
handle.set_request_parameter_value('test_i8', 'values', [1,2,3,4])
handle.send_request('test_i8')
print(handle.get_response_parameter_value('test_i8', 'length'))
print(handle.get_response_parameter_value('test_i8', 'values'))

handle.set_request_parameter_value('test_u16', 'length', 4)
handle.set_request_parameter_value('test_u16', 'values', [1,2,3,4])
handle.send_request('test_u16')
print(handle.get_response_parameter_value('test_u16', 'length'))
print(handle.get_response_parameter_value('test_u16', 'values'))

handle.set_request_parameter_value('test_i16', 'length', 4)
handle.set_request_parameter_value('test_i16', 'values', [1,2,3,4])
handle.send_request('test_i16')
print(handle.get_response_parameter_value('test_i16', 'length'))
print(handle.get_response_parameter_value('test_i16', 'values'))

handle.set_request_parameter_value('test_u32', 'length', 4)
handle.set_request_parameter_value('test_u32', 'values', [1,2,3,4])
handle.send_request('test_u32')
print(handle.get_response_parameter_value('test_u32', 'length'))
print(handle.get_response_parameter_value('test_u32', 'values'))

handle.set_request_parameter_value('test_i32', 'length', 4)
handle.set_request_parameter_value('test_i32', 'values', [1,2,3,4])
handle.send_request('test_i32')
print(handle.get_response_parameter_value('test_i32', 'length'))
print(handle.get_response_parameter_value('test_i32', 'values'))

handle.set_request_parameter_value('test_float', 'length', 4)
handle.set_request_parameter_value('test_float', 'values', [1,2,3,4])
handle.send_request('test_float')
print(handle.get_response_parameter_value('test_float', 'length'))
print(handle.get_response_parameter_value('test_float', 'values'))

handle.enable_notifications()

handle.set_request_parameter_value('test_notification_trigger', 'mask', 0xFF)
handle.set_request_parameter_value('test_notification_trigger', 'reset', 1)
handle.set_request_parameter_value('test_notification_trigger', 'length', 4)

handle.send_request('test_notification_trigger')

time.sleep(0.1)

print(handle.get_notification_parameter_value('test_notification_u8', 'length'))
print(handle.get_notification_parameter_value('test_notification_u8', 'values'))
print(handle.get_notification_parameter_value('test_notification_i8', 'length'))
print(handle.get_notification_parameter_value('test_notification_i8', 'values'))
print(handle.get_notification_parameter_value('test_notification_u16', 'length'))
print(handle.get_notification_parameter_value('test_notification_u16', 'values'))
print(handle.get_notification_parameter_value('test_notification_i16', 'length'))
print(handle.get_notification_parameter_value('test_notification_i16', 'values'))
print(handle.get_notification_parameter_value('test_notification_u32', 'length'))
print(handle.get_notification_parameter_value('test_notification_u32', 'values'))
print(handle.get_notification_parameter_value('test_notification_i32', 'length'))
print(handle.get_notification_parameter_value('test_notification_i32', 'values'))
print(handle.get_notification_parameter_value('test_notification_float', 'length'))
print(handle.get_notification_parameter_value('test_notification_float', 'values'))

handle.close()