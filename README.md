# serial-rpc
A simple serial remote procedure call protocol based on C for embedded systems. The transport layer protocol can
be either UART or CAN protocol. This protocol is a hybrid request-response/streaming protocol designed for single bus
systems where multiple devices/systems are connected to a single bus. In the bus, there is exactly one master which 
sends out commands to a specific device along the bus and the device responds to the request. The bus master also has 
control over the streaming of data by individual devices/systems on the bus. The bus master can enable/disable streaming 
of data from a certain bus target in order to avoid collision of bytes on the system bus.

## Packet format
Each protocol packet consists of the following fields:

**address**:        This is the address of the bus target which is being addressed by the master or the address of the bus target
                    responding to a command or sending out a notification.

**packet_type**:    This indicates tha type of the packet. This protocol supports four types of packet: request(0), response(1),
                    notification(2) and control and status(3).
                
                    *Request*               :   This type of packet is sent by the bus master to a bus target of target *address*.
                    *Response*              :   This type of packet is sent by the bus target of target *address* to the bus master
                                                in response to the request packet sent by the bus master.
                    *Notification*          :   This type of packet is sent by the bus target of target *address* to the bus master.
                    *Control and status*    :   This is a special packet which is used to control the streaming of notifications by
                                                the target device and also get status of the bus target.

**index**:          This indicates the index of the request/response and the notification payload. In the bus master & target, the 
                    request/response packets are stored in a list and require the index to get the required request/response packet
                    from the list.

**payload_length**: The payload length indicates the number of bytes in the packet payload. The value of the payload length
                    ranges from 0 to 28 bytes.
                    
**crc**:            Each packet is protected by a CRC field which CRC8-CCITT [text](https://www.3dbrew.org/wiki/CRC-8-CCITT). The CRC
                    field protects the entire packet.

The request/response/notification packets have the following format:

|                                BYTE 0                                   |     BYTE 1      |      BYTE 2              |
|-------------------------------------------------------------------------|-----------------|--------------------------|
| Address(BIT7-BIT4) : Packet Type (BIT3 - BIT2) : INDEX_H (BIT1 - BIT0)  |     INDEX_L     |   PAYLOAD_LENGTH (**L**) |

|       BYTE 3 to BYTE L + 2        |    BYTE L + 3   |
|-----------------------------------|-----------------|
|    Payload bytes (length **L**)   |       CRC       |

The control and status packets have the following format:

|                                BYTE 0                             |          BYTE 1      |      BYTE 2              |
|-------------------------------------------------------------------|----------------------|--------------------------|
| Address(BIT7-BIT4) : Packet Type (BIT3 - BIT2) : 0 (BIT1 - BIT0)  | ENABLE_NOTIFICATIONS |  TRANSMIT BUFFER SIZE    |


|       BYTE 3           |          BYTE 4                 |            BYTE 5              |       BYTE 6         |
|------------------------|---------------------------------|--------------------------------|----------------------|
|   RECEIVE BUFFER SIZE  |  TRANSMIT BUFFER ELEMENT COUNT  |   RECEIVE BUFFER ELEMENT COUNT |       CRC            |

## Detailed software architecture

The bus master sends over a request packet over the bus to all devices. The devices on the bus receives the incoming byte and
calculates the target address. If the target address of the packet does not equal to the device's own address, the bus target device
rejects the incoming packet by not storing it in the request buffer. If the address of the incoming packet matches with the target
device, the incoming packet is stored in the request queue (if the packet type is not control/status). If the incoming packet
is a control/status packet, the device immediately sends out the response to the bus master. Once the request packet is loaded into
the request buffer, the application then gets the request packet from the request buffer, gets the payload and then uses the request packet
index to call the required callback function in the application code to process the request sent by the bus master. Once the request is
processed, the response packet is loaded into the response buffer which is then sent to the bus master. For sending over notifications,
the target device checks if the bus master has enabled notifications or not. If enabled, the notification packet is loaded into
the notification buffer which is then sent to the bus master.