
#include "serial_rpc.h"

extern uint8_t crc8ccitt(uint8_t * data, size_t size);

/* Test packets. */
serial_rpc_packet_t test_packets[] = 
{
    [0] = {
            .address = 0x00,
            .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
            .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(0),
            .index_low = SERIAL_RPC_PACKET_INDEX_LOW(0),
            .payload_length = 4,
            .payload = {1, 2, 3, 4},
          },
};

/* Define callback functions. */
SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_0)
{
    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH(handle, 4);
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT32(handle, 0, 0xE4A68FB0);
    
    SERIAL_RPC_LOG("Callback function for request index 0 called !");
}

/* Function to emulate transmission of bytes along system bus. */
static void send_to_printf(uint8_t* data, size_t length)
{
    size_t index;
    
    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_LOG("Sending byte[%d] = 0x%.2x", (int)index, data[index]);
    }
}

/* Response callback table. */
SERIAL_RPC_RESPONSE_CALLBACK_TABLE_DEFINE(test_response_callback_table) = 
{
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0, test_response_callback_0, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_END,
};

/* Definition of serial RPC handle instance. */
SERIAL_RPC_HANDLE_DEFINE(test_rpc_handle, 0x00, test_response_callback_table, send_to_printf);

/*************************** Helper functions **********************/

static void test_packets_preprocess()
{
    int index;
    
    for( index = 0 ; index < sizeof(test_packets)/sizeof(test_packets[0]) ; index++ )
    {
        test_packets[index].crc = crc8ccitt( (uint8_t*)&test_packets[index], sizeof(serial_rpc_packet_t) - 1 );
    }
}

static void test_consume_test_packet(int index)
{
    int byte_index;
    
    for( byte_index = 0 ; byte_index < sizeof(serial_rpc_packet_t) ; byte_index++ )
    {
        serial_rpc_handle_rx_byte(&test_rpc_handle, ((uint8_t*)&test_packets[index])[byte_index] );
    }
}

/*************************** Main function *************************/

int main(void) {
    
    test_packets_preprocess();
    serial_rpc_init(&test_rpc_handle);
    
    test_consume_test_packet(0);
    
    serial_rpc_process(&test_rpc_handle);
    
    return 0;
}