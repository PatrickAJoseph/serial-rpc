
import serial_rpc
import time
import pytest

PORT_NAME = 'COM8'
BAUD_RATE = 115200

UINT8_MIN = 0
UINT8_MAX = 255
INT8_MIN = -128
INT8_MAX = 127

UINT16_MIN = 0
UINT16_MAX = 65535
INT16_MIN = -32768
INT16_MAX = 32767

UINT32_MIN = 0
UINT32_MAX = 4294967295
INT32_MIN = -2147483648
INT32_MAX = 2147483647

FLOAT_MIN = -1000.0
FLOAT_MAX = 1000.0

def open_handle():
    handle = serial_rpc.serial_rpc(port_name = PORT_NAME, baud_rate = BAUD_RATE, parameter_file = 'test_parameter_file.yaml')
    return handle

def close_handle(handle):
    handle.close()

def test_u8():
    handle = open_handle()
    
    for length in range(1, 7):
        
        handle.set_request_parameter_value('test_u8', 'length', length)
        
        values = []
        expected_values = []

        step = (UINT8_MAX - UINT8_MIN) // length
        
        for index in range(0, length):
            values.append(UINT8_MIN + (step * index))
            expected_values.append( UINT8_MIN + (step * index) + 1 )
        
        handle.set_request_parameter_value('test_u8', 'values', values)
        
        handle.send_request('test_u8')        
        
        response_length = handle.get_response_parameter_value('test_u8', 'length')
        response_values = handle.get_response_parameter_value('test_u8', 'values')
        
        assert response_length == length
        assert response_values == expected_values
    
    close_handle(handle)
    
def test_i8():
    handle = open_handle()
    
    for length in range(1, 7):
        
        handle.set_request_parameter_value('test_i8', 'length', length)
        
        values = []
        expected_values = []

        step = (INT8_MAX - INT8_MIN) // length
        
        for index in range(0, length):
            values.append(INT8_MIN + (step * index))
            expected_values.append( INT8_MIN + (step * index) + 1 )
        
        handle.set_request_parameter_value('test_i8', 'values', values)
        
        handle.send_request('test_i8')        
        
        response_length = handle.get_response_parameter_value('test_i8', 'length')
        response_values = handle.get_response_parameter_value('test_i8', 'values')
        
        assert response_length == length
        assert response_values == expected_values
    
    close_handle(handle)

def test_u16():
    handle = open_handle()
    
    for length in range(1, 7):
        
        handle.set_request_parameter_value('test_u16', 'length', length)
        
        values = []
        expected_values = []

        step = (UINT16_MAX - UINT16_MIN) // length
        
        for index in range(0, length):
            values.append(UINT16_MIN + (step * index))
            expected_values.append( UINT16_MIN + (step * index) + 1 )
        
        handle.set_request_parameter_value('test_u16', 'values', values)
        
        handle.send_request('test_u16')        
        
        response_length = handle.get_response_parameter_value('test_u16', 'length')
        response_values = handle.get_response_parameter_value('test_u16', 'values')
        
        assert response_length == length
        assert response_values == expected_values
    
    close_handle(handle)
    
def test_i16():
    handle = open_handle()
    
    for length in range(1, 7):
        
        handle.set_request_parameter_value('test_i16', 'length', length)
        
        values = []
        expected_values = []

        step = (INT16_MAX - INT16_MIN) // length
        
        for index in range(0, length):
            values.append(INT16_MIN + (step * index))
            expected_values.append( INT16_MIN + (step * index) + 1 )
        
        handle.set_request_parameter_value('test_i16', 'values', values)
        
        handle.send_request('test_i16')        
        
        response_length = handle.get_response_parameter_value('test_i16', 'length')
        response_values = handle.get_response_parameter_value('test_i16', 'values')
        
        assert response_length == length
        assert response_values == expected_values
    
    close_handle(handle)

def test_u32():
    handle = open_handle()
    
    for length in range(1, 7):
        
        handle.set_request_parameter_value('test_u32', 'length', length)
        
        values = []
        expected_values = []

        step = (UINT32_MAX - UINT32_MIN) // length
        
        for index in range(0, length):
            values.append(UINT32_MIN + (step * index))
            expected_values.append( UINT32_MIN + (step * index) + 1 )
        
        handle.set_request_parameter_value('test_u32', 'values', values)
        
        handle.send_request('test_u32')        
        
        response_length = handle.get_response_parameter_value('test_u32', 'length')
        response_values = handle.get_response_parameter_value('test_u32', 'values')
        
        assert response_length == length
        assert response_values == expected_values
    
    close_handle(handle)
    
def test_i32():
    handle = open_handle()
    
    for length in range(1, 7):
        
        handle.set_request_parameter_value('test_i32', 'length', length)
        
        values = []
        expected_values = []

        step = (INT32_MAX - INT32_MIN) // length
        
        for index in range(0, length):
            values.append(INT32_MIN + (step * index))
            expected_values.append( INT32_MIN + (step * index) + 1 )
        
        handle.set_request_parameter_value('test_i32', 'values', values)
        
        handle.send_request('test_i32')        
        
        response_length = handle.get_response_parameter_value('test_i32', 'length')
        response_values = handle.get_response_parameter_value('test_i32', 'values')
        
        assert response_length == length
        assert response_values == expected_values
    
    close_handle(handle)

def test_float():
    handle = open_handle()
    
    for length in range(1, 7):
        
        handle.set_request_parameter_value('test_float', 'length', length)
        
        values = []
        expected_values = []

        step = (FLOAT_MAX - FLOAT_MIN) // length
        
        for index in range(0, length):
            values.append(FLOAT_MIN + (step * index))
            expected_values.append( FLOAT_MIN + (step * index) + 1.0 )
        
        handle.set_request_parameter_value('test_float', 'values', values)
        
        handle.send_request('test_float')        
        
        response_length = handle.get_response_parameter_value('test_float', 'length')
        response_values = handle.get_response_parameter_value('test_float', 'values')
        
        assert response_length == length
        assert response_values == expected_values
    
    close_handle(handle)

class test_notification_counter:
    def __init__(self):
        self.notification_count_u8: int = 0
        self.notification_count_i8: int = 0
        self.notification_count_u16: int = 0
        self.notification_count_i16: int = 0
        self.notification_count_u32: int = 0
        self.notification_count_i32: int = 0
        self.notification_count_float: int = 0

def notification_callback_u8(notification_id, args, parameters):
    args.notification_count_u8 = args.notification_count_u8 + 1

def notification_callback_i8(notification_id, args, parameters):
    args.notification_count_i8 = args.notification_count_i8 + 1

def notification_callback_u16(notification_id, args, parameters):
    args.notification_count_u16 = args.notification_count_u16 + 1

def notification_callback_i16(notification_id, args, parameters):
    args.notification_count_i16 = args.notification_count_i16 + 1

def notification_callback_u32(notification_id, args, parameters):
    args.notification_count_u32 = args.notification_count_u32 + 1

def notification_callback_i32(notification_id, args, parameters):
    args.notification_count_i32 = args.notification_count_i32 + 1

def notification_callback_float(notification_id, args, parameters):
    args.notification_count_float = args.notification_count_float + 1

def test_notification_u8():
    
    handle = open_handle()
    
    counter = test_notification_counter()
    
    handle.register_notification_callback('test_notification_u8', notification_callback_u8, counter)    
    
    handle.enable_notifications()
    
    expected_notification_count = 0
    
    for length in range(1, 7):

        expected_values = [0, 0, 0, 0, 0, 0]
    
        for i in range(0, 32):
    
            handle.set_request_parameter_value('test_notification_trigger', 'mask', ( 1 << 0 ))
            handle.set_request_parameter_value('test_notification_trigger', 'reset', int(i == 0))
            handle.set_request_parameter_value('test_notification_trigger', 'length', length)

            handle.send_request('test_notification_trigger')

            time.sleep(0.01)

            response_values = handle.get_notification_parameter_value('test_notification_u8', 'values')
            response_length = handle.get_notification_parameter_value('test_notification_u8', 'length')
    
            expected_values[0] = expected_values[0] + 1
            expected_values[1] = expected_values[1] + 2
            expected_values[2] = expected_values[2] + 3
            expected_values[3] = expected_values[3] + 4
            expected_values[4] = expected_values[4] + 5
            expected_values[5] = expected_values[5] + 6
    
            for j in range(0, length):
                assert response_values[j] == expected_values[j]
    
            expected_notification_count = expected_notification_count + 1
    
            assert response_length == length
            assert counter.notification_count_u8 == expected_notification_count
    
    handle.disable_notifications()
    
    close_handle(handle)
    
def test_notification_i8():
    
    handle = open_handle()
    
    counter = test_notification_counter()
    
    handle.register_notification_callback('test_notification_i8', notification_callback_i8, counter)    
    
    handle.enable_notifications()
    
    expected_notification_count = 0
    
    for length in range(1, 7):

        expected_values = [INT8_MIN, INT8_MIN, INT8_MIN, INT8_MIN, INT8_MIN, INT8_MIN]
    
        for i in range(0, 16):
    
            handle.set_request_parameter_value('test_notification_trigger', 'mask', ( 1 << 1 ))
            handle.set_request_parameter_value('test_notification_trigger', 'reset', int(i == 0))
            handle.set_request_parameter_value('test_notification_trigger', 'length', length)

            handle.send_request('test_notification_trigger')

            time.sleep(0.01)

            response_values = handle.get_notification_parameter_value('test_notification_i8', 'values')
            response_length = handle.get_notification_parameter_value('test_notification_i8', 'length')
    
            expected_values[0] = expected_values[0] + 1
            expected_values[1] = expected_values[1] + 2
            expected_values[2] = expected_values[2] + 3
            expected_values[3] = expected_values[3] + 4
            expected_values[4] = expected_values[4] + 5
            expected_values[5] = expected_values[5] + 6
    
            for j in range(0, length):
                assert response_values[j] == expected_values[j]
    
            expected_notification_count = expected_notification_count + 1
    
            assert response_length == length
            assert counter.notification_count_i8 == expected_notification_count
    
    handle.disable_notifications()
    
    close_handle(handle)
    
def test_notification_u16():
    
    handle = open_handle()
    
    counter = test_notification_counter()
    
    handle.register_notification_callback('test_notification_u16', notification_callback_u16, counter)    
    
    handle.enable_notifications()
    
    expected_notification_count = 0
    
    for length in range(1, 7):

        expected_values = [0, 0, 0, 0, 0, 0]
    
        for i in range(0, 32):
    
            handle.set_request_parameter_value('test_notification_trigger', 'mask', ( 1 << 2 ))
            handle.set_request_parameter_value('test_notification_trigger', 'reset', int(i == 0))
            handle.set_request_parameter_value('test_notification_trigger', 'length', length)

            handle.send_request('test_notification_trigger')

            time.sleep(0.01)

            response_values = handle.get_notification_parameter_value('test_notification_u16', 'values')
            response_length = handle.get_notification_parameter_value('test_notification_u16', 'length')
    
            expected_values[0] = expected_values[0] + 1
            expected_values[1] = expected_values[1] + 2
            expected_values[2] = expected_values[2] + 3
            expected_values[3] = expected_values[3] + 4
            expected_values[4] = expected_values[4] + 5
            expected_values[5] = expected_values[5] + 6
    
            for j in range(0, length):
                assert response_values[j] == expected_values[j]
    
            expected_notification_count = expected_notification_count + 1
    
            assert response_length == length
            assert counter.notification_count_u16 == expected_notification_count
    
    handle.disable_notifications()
    
    close_handle(handle)
    
def test_notification_i16():
    
    handle = open_handle()
    
    counter = test_notification_counter()
    
    handle.register_notification_callback('test_notification_i16', notification_callback_i16, counter)    
    
    handle.enable_notifications()
    
    expected_notification_count = 0
    
    for length in range(1, 7):

        expected_values = [INT16_MIN, INT16_MIN, INT16_MIN, INT16_MIN, INT16_MIN, INT16_MIN]
    
        for i in range(0, 16):
    
            handle.set_request_parameter_value('test_notification_trigger', 'mask', ( 1 << 3 ))
            handle.set_request_parameter_value('test_notification_trigger', 'reset', int(i == 0))
            handle.set_request_parameter_value('test_notification_trigger', 'length', length)

            handle.send_request('test_notification_trigger')

            time.sleep(0.01)

            response_values = handle.get_notification_parameter_value('test_notification_i16', 'values')
            response_length = handle.get_notification_parameter_value('test_notification_i16', 'length')
    
            expected_values[0] = expected_values[0] + 1
            expected_values[1] = expected_values[1] + 2
            expected_values[2] = expected_values[2] + 3
            expected_values[3] = expected_values[3] + 4
            expected_values[4] = expected_values[4] + 5
            expected_values[5] = expected_values[5] + 6
    
            for j in range(0, length):
                assert response_values[j] == expected_values[j]
    
            expected_notification_count = expected_notification_count + 1
    
            assert response_length == length
            assert counter.notification_count_i16 == expected_notification_count
    
    handle.disable_notifications()
    
    close_handle(handle)
    
def test_notification_u32():
    
    handle = open_handle()
    
    counter = test_notification_counter()
    
    handle.register_notification_callback('test_notification_u32', notification_callback_u32, counter)    
    
    handle.enable_notifications()
    
    expected_notification_count = 0
    
    for length in range(1, 7):

        expected_values = [0, 0, 0, 0, 0, 0]
    
        for i in range(0, 32):
    
            handle.set_request_parameter_value('test_notification_trigger', 'mask', ( 1 << 4 ))
            handle.set_request_parameter_value('test_notification_trigger', 'reset', int(i == 0))
            handle.set_request_parameter_value('test_notification_trigger', 'length', length)

            handle.send_request('test_notification_trigger')

            time.sleep(0.01)

            response_values = handle.get_notification_parameter_value('test_notification_u32', 'values')
            response_length = handle.get_notification_parameter_value('test_notification_u32', 'length')
    
            expected_values[0] = expected_values[0] + 1
            expected_values[1] = expected_values[1] + 2
            expected_values[2] = expected_values[2] + 3
            expected_values[3] = expected_values[3] + 4
            expected_values[4] = expected_values[4] + 5
            expected_values[5] = expected_values[5] + 6
    
            for j in range(0, length):
                assert response_values[j] == expected_values[j]
    
            expected_notification_count = expected_notification_count + 1
    
            assert response_length == length
            assert counter.notification_count_u32 == expected_notification_count
    
    handle.disable_notifications()
    
    close_handle(handle)
    
def test_notification_i32():
    
    handle = open_handle()
    
    counter = test_notification_counter()
    
    handle.register_notification_callback('test_notification_i32', notification_callback_i32, counter)    
    
    handle.enable_notifications()
    
    expected_notification_count = 0
    
    for length in range(1, 7):

        expected_values = [INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN]
    
        for i in range(0, 16):
    
            handle.set_request_parameter_value('test_notification_trigger', 'mask', ( 1 << 5 ))
            handle.set_request_parameter_value('test_notification_trigger', 'reset', int(i == 0))
            handle.set_request_parameter_value('test_notification_trigger', 'length', length)

            handle.send_request('test_notification_trigger')

            time.sleep(0.01)

            response_values = handle.get_notification_parameter_value('test_notification_i32', 'values')
            response_length = handle.get_notification_parameter_value('test_notification_i32', 'length')
    
            expected_values[0] = expected_values[0] + 1
            expected_values[1] = expected_values[1] + 2
            expected_values[2] = expected_values[2] + 3
            expected_values[3] = expected_values[3] + 4
            expected_values[4] = expected_values[4] + 5
            expected_values[5] = expected_values[5] + 6
    
            for j in range(0, length):
                assert response_values[j] == expected_values[j]
    
            expected_notification_count = expected_notification_count + 1
    
            assert response_length == length
            assert counter.notification_count_i32 == expected_notification_count
    
    handle.disable_notifications()
    
    close_handle(handle)
    
def test_notification_float():
    
    handle = open_handle()
    
    counter = test_notification_counter()
    
    handle.register_notification_callback('test_notification_float', notification_callback_float, counter)    
    
    handle.enable_notifications()
    
    expected_notification_count = 0
    
    for length in range(1, 7):

        expected_values = [FLOAT_MIN, FLOAT_MIN, FLOAT_MIN, FLOAT_MIN, FLOAT_MIN, FLOAT_MIN]
    
        for i in range(0, 16):
    
            handle.set_request_parameter_value('test_notification_trigger', 'mask', ( 1 << 6 ))
            handle.set_request_parameter_value('test_notification_trigger', 'reset', int(i == 0))
            handle.set_request_parameter_value('test_notification_trigger', 'length', length)

            handle.send_request('test_notification_trigger')

            time.sleep(0.01)

            response_values = handle.get_notification_parameter_value('test_notification_float', 'values')
            response_length = handle.get_notification_parameter_value('test_notification_float', 'length')
    
            expected_values[0] = expected_values[0] + 0.01
            expected_values[1] = expected_values[1] + 0.02
            expected_values[2] = expected_values[2] + 0.03
            expected_values[3] = expected_values[3] + 0.04
            expected_values[4] = expected_values[4] + 0.05
            expected_values[5] = expected_values[5] + 0.06
    
            for j in range(0, length):
                assert pytest.approx(response_values[j], rel = 0.01) == expected_values[j]
    
            expected_notification_count = expected_notification_count + 1
    
            assert response_length == length
            assert counter.notification_count_float == expected_notification_count
    
    handle.disable_notifications()
    
    close_handle(handle)