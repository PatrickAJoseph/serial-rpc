import serial
import time

def crc8_ccitt(data: bytes) -> int:
    if not isinstance(data, (bytes, bytearray)):
        raise TypeError("Input must be bytes or bytearray.")

    crc = 0x00  # Initial value
    poly = 0x07  # Polynomial

    for byte in data:
        crc ^= byte  # XOR byte into CRC
        for _ in range(8):  # Process each bit
            if crc & 0x80:  # If MSB is set
                crc = ((crc << 1) & 0xFF) ^ poly
            else:
                crc = (crc << 1) & 0xFF

    return crc

#  LED0/1 control RPC request packet payload has the following structure.
#
#  BYTE0       : set_led_0_state
#  BYTE1       : led_0_state
#  BYTE2       : set_led_0_blink_count
#  BYTE3       : led_0_blink_count
#  BYTE4       : set_led_0_blink_interval_ms
#  BYTE5 & 6   : led_0_blink_interval_ms
#  BYTE7       : led_0_blink   

def led_control_payload(set_led_state: int = 0, led_state: int = 0, set_led_blink_count: int = 0, led_blink_count: int = 0, set_led_blink_interval: int = 0, led_blink_interval_ms: int = 0, blink_led: int = 0):

    payload = []
    payload.append(set_led_state)
    payload.append(led_state)
    payload.append(set_led_blink_count)
    payload.append(led_blink_count)
    payload.append(set_led_blink_interval)
    payload.append(led_blink_interval_ms >> 8)
    payload.append(led_blink_interval_ms & 255)
    payload.append(blink_led)

    payload = payload + [0 for i in range(0, 20)]

    return payload

def led0_on(s: serial.Serial):
    payload = led_control_payload(set_led_state = 1, led_state = 1)
    packet = []
    packet.append(0)                  # Bus target address, packet type = request(0), request index = (0).
    packet.append(0)
    packet.append(8)                  # Payload length (8)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led0_off(s: serial.Serial):

    payload = led_control_payload(set_led_state = 1, led_state = 0)

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (0).
    packet.append(0)
    packet.append(8)                  # Payload length (8)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led0_set_blink_interval_ms(s: serial.Serial, interval_ms: int):

    payload = led_control_payload(set_led_blink_interval = 1, led_blink_interval_ms = interval_ms)

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (0).
    packet.append(0)
    packet.append(8)                  # Payload length (8)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led0_set_blink_count(s: serial.Serial, count: int):

    payload = led_control_payload(set_led_blink_count = 1, led_blink_count = count)

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (0).
    packet.append(0)
    packet.append(8)                  # Payload length (8)

    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led0_blink(s: serial.Serial):

    payload = led_control_payload(blink_led = 1)

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (0).
    packet.append(0)
    packet.append(8)                  # Payload length (8)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led0_status(s: serial.Serial):

    payload = [0 for i in range(0, 28)]

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (1).
    packet.append(1)
    packet.append(0)                  # Payload length (0)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))
    
    packet.append(crc)

    s.write(bytes(packet))
    response = s.read(32)
    
    response_payload = response[3:]
        
    led_state = int(response_payload[0])
    led_blink_count = (int(response_payload[1]) << 8) + int(response_payload[2])
    
    return (led_state, led_blink_count)

def led1_on(s: serial.Serial):
    payload = led_control_payload(set_led_state = 1, led_state = 1)
    packet = []
    packet.append(0)                  # Bus target address, packet type = request(0), request index = (2).
    packet.append(2)
    packet.append(8)                  # Payload length (8)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led1_off(s: serial.Serial):

    payload = led_control_payload(set_led_state = 1, led_state = 0)

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (2).
    packet.append(2)
    packet.append(8)                  # Payload length (8)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led1_set_blink_interval_ms(s: serial.Serial, interval_ms: int):

    payload = led_control_payload(set_led_blink_interval = 1, led_blink_interval_ms = interval_ms)

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (2).
    packet.append(2)
    packet.append(8)                  # Payload length (8)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led1_set_blink_count(s: serial.Serial, count: int):

    payload = led_control_payload(set_led_blink_count = 1, led_blink_count = count)

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (2).
    packet.append(2)
    packet.append(8)                  # Payload length (8)

    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led1_blink(s: serial.Serial):

    payload = led_control_payload(blink_led = 1)

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (2).
    packet.append(2)
    packet.append(8)                  # Payload length (8)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))

    packet.append(int(crc))

    s.write(bytes(packet))
    s.read(32)

def led1_status(s: serial.Serial):

    payload = [0 for i in range(0, 28)]

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (1).
    packet.append(3)
    packet.append(0)                  # Payload length (0)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))
    
    packet.append(crc)

    s.write(bytes(packet))
    response = s.read(32)
    
    response_payload = response[3:]
        
    led_state = int(response_payload[0])
    led_blink_count = (int(response_payload[1]) << 8) + int(response_payload[2])
    
    return (led_state, led_blink_count)

def button0_reset_counter(s: serial.Serial):

    payload = [0 for i in range(0, 28)]

    payload[0] = 1

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (4).
    packet.append(4)
    packet.append(0)                  # Payload length (0)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))
    
    packet.append(crc)

    s.write(bytes(packet))
    
    s.read(32)

def button0_status(s: serial.Serial):

    payload = [0 for i in range(0, 28)]

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (5).
    packet.append(5)
    packet.append(0)                  # Payload length (0)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))
    
    packet.append(crc)

    s.write(bytes(packet))
    
    response = s.read(32)  
    
    response = response[3:]
    
    state = int(response[0])
    count = int(response[1])
    
    return (state, count)

def button1_reset_counter(s: serial.Serial):

    payload = [0 for i in range(0, 28)]

    payload[0] = 1

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (6).
    packet.append(6)
    packet.append(0)                  # Payload length (0)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))
    
    packet.append(crc)

    s.write(bytes(packet))
    
    s.read(32)

def button1_status(s: serial.Serial):

    payload = [0 for i in range(0, 28)]

    packet = []

    packet.append(0)                  # Bus target address, packet type = request(0), request index = (7).
    packet.append(7)
    packet.append(0)                  # Payload length (0)
    packet = packet + payload

    crc = crc8_ccitt(bytes(packet))
    
    packet.append(crc)

    s.write(bytes(packet))
    
    response = s.read(32)  
    
    response = response[3:]
    
    state = int(response[0])
    count = int(response[1])
    
    return (state, count)
        
 
############################################ Main function #################################### 
 
s = serial.Serial(port = 'COM8', baudrate = 115200, timeout = 5.0)

s.close()
s.open()

led0_off(s)
led1_off(s)

led0_set_blink_count(s, 10)
led0_set_blink_interval_ms(s, 250)
led0_blink(s)

led1_set_blink_count(s, 20)
led1_set_blink_interval_ms(s, 125)
led1_blink(s)

button0_reset_counter(s)
button1_reset_counter(s)

for i in range(0, 1000):
    (state0, count0) = button0_status(s)
    (state1, count1) = button1_status(s)
    print(f"Button 0 state: {state0}, Button 0 press count: {count0}, Button 1 state: {state1}, Button 1 press count: {count1}")
    time.sleep(0.01)

s.close()