#include "unity.h"
#include "../serial_rpc.h"

#define TEST_BUS_TARGET_ADDRESS     (0x01)

#define TEST_REQUEST_INDEX_0        (0x00F)
#define TEST_REQUEST_INDEX_1        (0x07F)
#define TEST_REQUEST_INDEX_2        (0x1F0)
#define TEST_REQUEST_INDEX_3        (0x3F7)

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_0);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_1);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_2);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_3);

static uint32_t test_response_callback_counter_0;
static uint32_t test_response_callback_counter_1;
static uint32_t test_response_callback_counter_2;
static uint32_t test_response_callback_counter_3;

SERIAL_RPC_RESPONSE_CALLBACK_TABLE_DEFINE(test_response_callback_table) =
{
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(TEST_REQUEST_INDEX_0, test_response_callback_0, (&test_response_callback_counter_0)),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(TEST_REQUEST_INDEX_1, test_response_callback_1, (&test_response_callback_counter_1)),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(TEST_REQUEST_INDEX_2, test_response_callback_2, (&test_response_callback_counter_2)),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(TEST_REQUEST_INDEX_3, test_response_callback_3, (&test_response_callback_counter_3)),
};

extern uint8_t crc8ccitt(uint8_t * data, size_t size);

static void send_to_console(uint8_t* data, size_t length)
{
    int index;
    
    for(index = 0 ; index < (int)length ; index++)
    {
        SERIAL_RPC_LOG("Sending byte[%d] = 0x%.2x\n", index, data[index]);
    }
}

SERIAL_RPC_HANDLE_DEFINE(test_rpc_handle, TEST_BUS_TARGET_ADDRESS, test_response_callback_table, send_to_console);

/*********************** Helper functions ********************/

static void test_consume_rpc_packet(serial_rpc_packet_t* packet)
{
    int index;
    
    for(index = 0 ; index < sizeof(serial_rpc_packet_t) ; index++)
    {
        serial_rpc_handle_rx_byte(&test_rpc_handle, ((uint8_t*)packet)[index] );
    }
}

static void test_update_packet_crc(serial_rpc_packet_t* packet)
{
    packet->crc = crc8ccitt((uint8_t*)packet, sizeof(serial_rpc_packet_t) - 1);
}

void setUp(void)
{
    serial_rpc_init(&test_rpc_handle);
}

void tearDown(void)
{
}

void test_serial_rpc_packet_size(void)
{
    TEST_ASSERT_EQUAL_UINT32(32, sizeof(serial_rpc_packet_t));
}

void test_serial_rpc_control_packet_size(void)
{
    TEST_ASSERT_EQUAL_UINT32(32, sizeof(serial_rpc_control_and_status_packet_t));
}

void test_serial_rpc_request_callback(void)
{
    test_response_callback_counter_0 = 0;
    test_response_callback_counter_1 = 0;
    test_response_callback_counter_2 = 0;
    test_response_callback_counter_3 = 0;
    
    serial_rpc_packet_t test_packet_0 = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_0),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_0),
        .payload_length = (size_t)4,
        .payload = {1, 2, 3, 4},
    };

    serial_rpc_packet_t test_packet_1 = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_1),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_1),
        .payload_length = (size_t)8,
        .payload = {1, 2, 3, 4, 5, 6, 7, 8},
    };

    serial_rpc_packet_t test_packet_2 = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_2),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_2),
        .payload_length = (size_t)16,
        .payload = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
    };

    serial_rpc_packet_t test_packet_3 = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_3),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_3),
        .payload_length = (size_t)24,
        .payload = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, },
    };

    test_update_packet_crc(&test_packet_0);
    test_update_packet_crc(&test_packet_1);
    test_update_packet_crc(&test_packet_2);
    test_update_packet_crc(&test_packet_3);
    
    test_consume_rpc_packet(&test_packet_0);
    serial_rpc_process(&test_rpc_handle);
    test_consume_rpc_packet(&test_packet_1);
    serial_rpc_process(&test_rpc_handle);
    test_consume_rpc_packet(&test_packet_2);
    serial_rpc_process(&test_rpc_handle);
    test_consume_rpc_packet(&test_packet_3);
    serial_rpc_process(&test_rpc_handle);
    
    TEST_ASSERT_EQUAL_UINT32(1, test_response_callback_counter_0);
    TEST_ASSERT_EQUAL_UINT32(1, test_response_callback_counter_1);
    TEST_ASSERT_EQUAL_UINT32(1, test_response_callback_counter_2);
    TEST_ASSERT_EQUAL_UINT32(1, test_response_callback_counter_3);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_serial_rpc_packet_size);
    RUN_TEST(test_serial_rpc_control_packet_size);
    RUN_TEST(test_serial_rpc_request_callback);

    return UNITY_END();
}

/* Callback functions. */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_0)
{
    test_response_callback_counter_0++;
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_1)
{
    test_response_callback_counter_1++;
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_2)
{
    test_response_callback_counter_2++;
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_3)
{
    test_response_callback_counter_3++;
}
