#include "unity.h"
#include "../../src/serial_rpc.h"

 

#define TEST_BUS_TARGET_ADDRESS     (0x01)

#define TEST_REQUEST_INDEX_0        (0x00F)
#define TEST_REQUEST_INDEX_1        (0x07F)
#define TEST_REQUEST_INDEX_2        (0x1F0)
#define TEST_REQUEST_INDEX_3        (0x3F7)

#define UINT16_TO_BYTEARRAY(x)      (((uint16_t)(x) >> 8) & 0xFFU), (((uint16_t)(x)) & 0xFFU)
#define UINT32_TO_BYTEARRAY(x)      ((((uint32_t)(x)) >> 24) & 0xFFU), ((((uint32_t)(x)) >> 16) & 0xFFU), ((((uint32_t)(x)) >> 8) & 0xFFU), (((uint32_t)(x)) & 0xFFU)
#define INT16_TO_BYTEARRAY(x)      UINT16_TO_BYTEARRAY( ((uint16_t)(x)) )
#define INT32_TO_BYTEARRAY(x)      UINT32_TO_BYTEARRAY( ((uint32_t)(x)) )
#define FLOAT_TO_BYTEARRAY(x) \
    ((uint8_t)(float_to_uint32((float)(x)) >> 24)), \
    ((uint8_t)(float_to_uint32((float)(x)) >> 16)), \
    ((uint8_t)(float_to_uint32((float)(x)) >> 8)),  \
    ((uint8_t)(float_to_uint32((float)(x))))

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

static serial_rpc_packet_t sent_packet;

static void send_to_console(uint8_t* data, size_t length)
{
    memcpy(&sent_packet, data, sizeof(serial_rpc_packet_t));

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

static inline uint32_t float_to_uint32(float value)
{
    uint32_t result;
    memcpy(&result, &value, sizeof(result));
    return result;
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


struct test_packet_0_values
{
    uint8_t u8[4];
    uint8_t i8[4];
};

struct test_packet_1_values
{
    uint16_t u16[4];
    int16_t i16[4];
};

struct test_packet_2_values
{
    uint32_t u32[4];
    int32_t i32[4];
};

struct test_packet_3_values
{
    float flt[7];
};

struct test_packet_0_values test_packet_0_request_values;
struct test_packet_1_values test_packet_1_request_values;
struct test_packet_2_values test_packet_2_request_values;
struct test_packet_3_values test_packet_3_request_values;

struct test_packet_0_values test_packet_0_response_values;
struct test_packet_1_values test_packet_1_response_values;
struct test_packet_2_values test_packet_2_response_values;
struct test_packet_3_values test_packet_3_response_values;

void test_serial_rpc_request_callback_params(void)
{
    serial_rpc_packet_t test_packet_0 = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_0),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_0),
        .payload_length = (size_t)8 * sizeof(uint8_t),
        .payload = {1, 2, 3, 4, (uint8_t)-1, (uint8_t)-2, (uint8_t)-3, (uint8_t)-4},
    };

    serial_rpc_packet_t test_packet_1 = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_1),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_1),
        .payload_length = (size_t)8 * sizeof(uint16_t),
        .payload = { UINT16_TO_BYTEARRAY(1), UINT16_TO_BYTEARRAY(2), UINT16_TO_BYTEARRAY(3), UINT16_TO_BYTEARRAY(4),    \
                     INT16_TO_BYTEARRAY(-1), INT16_TO_BYTEARRAY(-2), INT16_TO_BYTEARRAY(-3), INT16_TO_BYTEARRAY(-4), },
    };

    serial_rpc_packet_t test_packet_2 = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_2),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_2),
        .payload_length = (size_t)7 * sizeof(uint32_t),
        .payload = { UINT32_TO_BYTEARRAY(1), UINT32_TO_BYTEARRAY(2), UINT32_TO_BYTEARRAY(3), UINT32_TO_BYTEARRAY(4),
                     INT32_TO_BYTEARRAY(-1), INT32_TO_BYTEARRAY(-2), INT32_TO_BYTEARRAY(-3), },
    };

    serial_rpc_packet_t test_packet_3 = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_3),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_3),
        .payload_length = (size_t)7 * sizeof(float),
        .payload = { FLOAT_TO_BYTEARRAY(1.23f), FLOAT_TO_BYTEARRAY(-2.34f), FLOAT_TO_BYTEARRAY(4.56f),
                     FLOAT_TO_BYTEARRAY(4.67f), FLOAT_TO_BYTEARRAY(-0.67f), FLOAT_TO_BYTEARRAY(1.02f),
                     FLOAT_TO_BYTEARRAY(7.89f), },
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

    TEST_ASSERT_EQUAL_UINT8( test_packet_0_request_values.u8[0], 1 );
    TEST_ASSERT_EQUAL_UINT8( test_packet_0_request_values.u8[1], 2 );
    TEST_ASSERT_EQUAL_UINT8( test_packet_0_request_values.u8[2], 3 );
    TEST_ASSERT_EQUAL_UINT8( test_packet_0_request_values.u8[3], 4 );
    TEST_ASSERT_EQUAL_INT8( test_packet_0_request_values.i8[0], -1 );
    TEST_ASSERT_EQUAL_INT8( test_packet_0_request_values.i8[1], -2 );
    TEST_ASSERT_EQUAL_INT8( test_packet_0_request_values.i8[2], -3 );
    TEST_ASSERT_EQUAL_INT8( test_packet_0_request_values.i8[3], -4 );
 

    TEST_ASSERT_EQUAL_UINT8( test_packet_0_response_values.u8[0], 2 );
    TEST_ASSERT_EQUAL_UINT8( test_packet_0_response_values.u8[1], 3 );
    TEST_ASSERT_EQUAL_UINT8( test_packet_0_response_values.u8[2], 4 );
    TEST_ASSERT_EQUAL_UINT8( test_packet_0_response_values.u8[3], 5 );
    TEST_ASSERT_EQUAL_INT8( test_packet_0_response_values.i8[0], 0 );
    TEST_ASSERT_EQUAL_INT8( test_packet_0_response_values.i8[1], -1 );
    TEST_ASSERT_EQUAL_INT8( test_packet_0_response_values.i8[2], -2 );
    TEST_ASSERT_EQUAL_INT8( test_packet_0_response_values.i8[3], -3 );

    TEST_ASSERT_EQUAL_UINT16( test_packet_1_request_values.u16[0], 1 );
    TEST_ASSERT_EQUAL_UINT16( test_packet_1_request_values.u16[1], 2 );
    TEST_ASSERT_EQUAL_UINT16( test_packet_1_request_values.u16[2], 3 );
    TEST_ASSERT_EQUAL_UINT16( test_packet_1_request_values.u16[3], 4 );
    TEST_ASSERT_EQUAL_INT16( test_packet_1_request_values.i16[0], -1 );
    TEST_ASSERT_EQUAL_INT16( test_packet_1_request_values.i16[1], -2 );
    TEST_ASSERT_EQUAL_INT16( test_packet_1_request_values.i16[2], -3 );
    TEST_ASSERT_EQUAL_INT16( test_packet_1_request_values.i16[3], -4 );

    TEST_ASSERT_EQUAL_UINT16( test_packet_1_response_values.u16[0], 2 );
    TEST_ASSERT_EQUAL_UINT16( test_packet_1_response_values.u16[1], 3 );
    TEST_ASSERT_EQUAL_UINT16( test_packet_1_response_values.u16[2], 4 );
    TEST_ASSERT_EQUAL_UINT16( test_packet_1_response_values.u16[3], 5 );
    TEST_ASSERT_EQUAL_INT16( test_packet_1_response_values.i16[0], 0 );
    TEST_ASSERT_EQUAL_INT16( test_packet_1_response_values.i16[1], -1 );
    TEST_ASSERT_EQUAL_INT16( test_packet_1_response_values.i16[2], -2 );
    TEST_ASSERT_EQUAL_INT16( test_packet_1_response_values.i16[3], -3 );

    TEST_ASSERT_EQUAL_UINT32( test_packet_2_request_values.u32[0], 1 );
    TEST_ASSERT_EQUAL_UINT32( test_packet_2_request_values.u32[1], 2 );
    TEST_ASSERT_EQUAL_UINT32( test_packet_2_request_values.u32[2], 3 );
    TEST_ASSERT_EQUAL_INT32( test_packet_2_request_values.i32[0], -1 );
    TEST_ASSERT_EQUAL_INT32( test_packet_2_request_values.i32[1], -2 );
    TEST_ASSERT_EQUAL_INT32( test_packet_2_request_values.i32[2], -3 );

    TEST_ASSERT_EQUAL_UINT32( test_packet_2_response_values.u32[0], 2 );
    TEST_ASSERT_EQUAL_UINT32( test_packet_2_response_values.u32[1], 3 );
    TEST_ASSERT_EQUAL_UINT32( test_packet_2_response_values.u32[2], 4 );
    TEST_ASSERT_EQUAL_INT32( test_packet_2_response_values.i32[0], 0 );
    TEST_ASSERT_EQUAL_INT32( test_packet_2_response_values.i32[1], -1 );
    TEST_ASSERT_EQUAL_INT32( test_packet_2_response_values.i32[2], -2 );
    
    TEST_ASSERT_EQUAL_FLOAT( test_packet_3_response_values.flt[0], 2.48f );
    TEST_ASSERT_EQUAL_FLOAT( test_packet_3_response_values.flt[1], -1.09f );
    TEST_ASSERT_EQUAL_FLOAT( test_packet_3_response_values.flt[2], 5.81f );
    TEST_ASSERT_EQUAL_FLOAT( test_packet_3_response_values.flt[3], 5.92f );
    TEST_ASSERT_EQUAL_FLOAT( test_packet_3_response_values.flt[4], 0.58f );
    TEST_ASSERT_EQUAL_FLOAT( test_packet_3_response_values.flt[5], 2.27f );
    TEST_ASSERT_EQUAL_FLOAT( test_packet_3_response_values.flt[6], 9.14f );
}

void test_invalid_request_packet(void)
{
    serial_rpc_packet_t test_packet = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_REQUEST,
        .index_low = SERIAL_RPC_PACKET_INDEX_LOW(TEST_REQUEST_INDEX_1),
        .index_high = SERIAL_RPC_PACKET_INDEX_HIGH(TEST_REQUEST_INDEX_1),
        .payload_length = (size_t)8 * sizeof(uint16_t),
        .payload = { UINT16_TO_BYTEARRAY(1), UINT16_TO_BYTEARRAY(2), UINT16_TO_BYTEARRAY(3), UINT16_TO_BYTEARRAY(4),    \
                     INT16_TO_BYTEARRAY(-1), INT16_TO_BYTEARRAY(-2), INT16_TO_BYTEARRAY(-3), INT16_TO_BYTEARRAY(-4), },
    };

    test_update_packet_crc(&test_packet);

    test_consume_rpc_packet(&test_packet);

    TEST_ASSERT_TRUE(test_rpc_handle.processing_response);

    serial_rpc_process(&test_rpc_handle);

    test_packet.address = TEST_BUS_TARGET_ADDRESS + 1;

    test_consume_rpc_packet(&test_packet); 

    TEST_ASSERT_FALSE(test_rpc_handle.processing_response);

    test_packet.address = TEST_BUS_TARGET_ADDRESS;

    test_packet.crc = 0;

    test_consume_rpc_packet(&test_packet);

    TEST_ASSERT_FALSE(test_rpc_handle.processing_response);
}

void test_notification_buffer(void)
{
    uint8_t test_notification[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int index;

    test_rpc_handle.notification_buffer_count = 0;
    test_rpc_handle.notification_buffer_read_index = 0;
    test_rpc_handle.notification_buffer_write_index = 0;
    test_rpc_handle.notifications_enabled = true;

    serial_rpc_send_notification(&test_rpc_handle, 0, (void*)test_notification, sizeof(test_notification));

    TEST_ASSERT_EQUAL_UINT32(test_rpc_handle.notification_buffer_count, 1);

    serial_rpc_process(&test_rpc_handle);

    TEST_ASSERT_EQUAL_UINT32(test_rpc_handle.notification_buffer_count, 0);

    for( index = 0 ; index < SERIAL_RPC_NOTIFICATION_BUFFER_SIZE ; index++ )
    {
        serial_rpc_send_notification(&test_rpc_handle, 0, (void*)test_notification, sizeof(test_notification));
        TEST_ASSERT_EQUAL_UINT32((test_rpc_handle.notification_buffer_count), (index + 1));
    }

    TEST_ASSERT_EQUAL( serial_rpc_send_notification(&test_rpc_handle, 0, (void*)test_notification, sizeof(test_notification)), -ENOSPC );
}

void test_control_and_status_packet(void)
{
    uint8_t test_notification[4] = {1, 2, 3, 4};
    
    serial_rpc_control_and_status_packet_t control_and_status_packet = {
        .address = TEST_BUS_TARGET_ADDRESS,
        .packet_type = SERIAL_RPC_PACKET_TYPE_CONTROL_AND_STATUS,
        .enable_notifications = false,
        .notification_buffer_length = 0,
        .notification_buffer_count = 0,
    };
    
    test_update_packet_crc((serial_rpc_packet_t*)&control_and_status_packet);
    
    test_consume_rpc_packet((serial_rpc_packet_t*)&control_and_status_packet);
    
    serial_rpc_process(&test_rpc_handle);
    
    TEST_ASSERT_TRUE(test_rpc_handle.notifications_enabled == 0);
    
    control_and_status_packet.enable_notifications = true;
    
    test_update_packet_crc((serial_rpc_packet_t*)&control_and_status_packet);
    
    test_consume_rpc_packet((serial_rpc_packet_t*)&control_and_status_packet);
    
    serial_rpc_process(&test_rpc_handle);
    
    TEST_ASSERT_TRUE(test_rpc_handle.notifications_enabled == 1);    
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_serial_rpc_packet_size);
    RUN_TEST(test_serial_rpc_control_packet_size);
    RUN_TEST(test_serial_rpc_request_callback);
    RUN_TEST(test_serial_rpc_request_callback_params);
    RUN_TEST(test_invalid_request_packet);
    RUN_TEST(test_notification_buffer);
    RUN_TEST(test_control_and_status_packet);

    return UNITY_END();
}

/* Callback functions. */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_0)
{
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8(handle, 0, (&test_packet_0_request_values.u8[0]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8(handle, 1, (&test_packet_0_request_values.u8[1]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8(handle, 2, (&test_packet_0_request_values.u8[2]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8(handle, 3, (&test_packet_0_request_values.u8[3]));

    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT8(handle, 4, (&test_packet_0_request_values.i8[0]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT8(handle, 5, (&test_packet_0_request_values.i8[1]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT8(handle, 6, (&test_packet_0_request_values.i8[2]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT8(handle, 7, (&test_packet_0_request_values.i8[3]));

    memcpy(&test_packet_0_response_values, &test_packet_0_request_values, sizeof(test_packet_0_request_values));

    test_packet_0_response_values.u8[0]++;
    test_packet_0_response_values.u8[1]++;
    test_packet_0_response_values.u8[2]++;
    test_packet_0_response_values.u8[3]++;

    test_packet_0_response_values.i8[0]++;
    test_packet_0_response_values.i8[1]++;
    test_packet_0_response_values.i8[2]++;
    test_packet_0_response_values.i8[3]++;

    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8(handle, 0, (test_packet_0_response_values.u8[0]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8(handle, 1, (test_packet_0_response_values.u8[1]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8(handle, 2, (test_packet_0_response_values.u8[2]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8(handle, 3, (test_packet_0_response_values.u8[3]));

    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT8(handle, 4, (test_packet_0_response_values.i8[0]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT8(handle, 5, (test_packet_0_response_values.i8[1]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT8(handle, 6, (test_packet_0_response_values.i8[2]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT8(handle, 7, (test_packet_0_response_values.i8[3]));

    test_response_callback_counter_0++;
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_1)
{
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT16(handle, 0, (&test_packet_1_request_values.u16[0]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT16(handle, 2, (&test_packet_1_request_values.u16[1]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT16(handle, 4, (&test_packet_1_request_values.u16[2]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT16(handle, 6, (&test_packet_1_request_values.u16[3]));

    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT16(handle, 8, (&test_packet_1_request_values.i16[0]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT16(handle, 10, (&test_packet_1_request_values.i16[1]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT16(handle, 12, (&test_packet_1_request_values.i16[2]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT16(handle, 14, (&test_packet_1_request_values.i16[3]));

    memcpy(&test_packet_1_response_values, &test_packet_1_request_values, sizeof(test_packet_1_request_values));

    test_packet_1_response_values.u16[0]++;
    test_packet_1_response_values.u16[1]++;
    test_packet_1_response_values.u16[2]++;
    test_packet_1_response_values.u16[3]++;

    test_packet_1_response_values.i16[0]++;
    test_packet_1_response_values.i16[1]++;
    test_packet_1_response_values.i16[2]++;
    test_packet_1_response_values.i16[3]++;

    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16(handle, 0, (test_packet_1_response_values.u16[0]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16(handle, 2, (test_packet_1_response_values.u16[1]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16(handle, 4, (test_packet_1_response_values.u16[2]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16(handle, 6, (test_packet_1_response_values.u16[3]));

    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT16(handle, 8, (test_packet_1_response_values.i16[0]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT16(handle, 10, (test_packet_1_response_values.i16[1]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT16(handle, 12, (test_packet_1_response_values.i16[2]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT16(handle, 14, (test_packet_1_response_values.i16[3]));

    test_response_callback_counter_1++;
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_2)
{
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT32(handle, 0, (&test_packet_2_request_values.u32[0]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT32(handle, 4, (&test_packet_2_request_values.u32[1]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT32(handle, 8, (&test_packet_2_request_values.u32[2]));

    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT32(handle, 16, (&test_packet_2_request_values.i32[0]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT32(handle, 20, (&test_packet_2_request_values.i32[1]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_INT32(handle, 24, (&test_packet_2_request_values.i32[2]));

    memcpy(&test_packet_2_response_values, &test_packet_2_request_values, sizeof(test_packet_2_request_values)); 

    test_packet_2_response_values.u32[0]++;
    test_packet_2_response_values.u32[1]++;
    test_packet_2_response_values.u32[2]++;
    test_packet_2_response_values.u32[3]++;

    test_packet_2_response_values.i32[0]++;
    test_packet_2_response_values.i32[1]++;
    test_packet_2_response_values.i32[2]++;
    test_packet_2_response_values.i32[3]++;

    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT32(handle, 0, (test_packet_2_response_values.u32[0]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT32(handle, 4, (test_packet_2_response_values.u32[1]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT32(handle, 8, (test_packet_2_response_values.u32[2]));

    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT32(handle, 16, (test_packet_2_response_values.i32[0]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT32(handle, 20, (test_packet_2_response_values.i32[1]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT32(handle, 24, (test_packet_2_response_values.i32[2]));

    test_response_callback_counter_2++;
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_response_callback_3)
{
    SERIAL_RPC_REQUEST_PAYLOAD_GET_FLOAT(handle, 0, (&test_packet_3_request_values.flt[0]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_FLOAT(handle, 4, (&test_packet_3_request_values.flt[1]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_FLOAT(handle, 8, (&test_packet_3_request_values.flt[2]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_FLOAT(handle, 12, (&test_packet_3_request_values.flt[3]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_FLOAT(handle, 16, (&test_packet_3_request_values.flt[4]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_FLOAT(handle, 20, (&test_packet_3_request_values.flt[5]));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_FLOAT(handle, 24, (&test_packet_3_request_values.flt[6]));

    memcpy(&test_packet_3_response_values, &test_packet_3_request_values, sizeof(test_packet_3_request_values)); 

    test_packet_3_response_values.flt[0] += 1.25f;
    test_packet_3_response_values.flt[1] += 1.25f;
    test_packet_3_response_values.flt[2] += 1.25f;
    test_packet_3_response_values.flt[3] += 1.25f;
    test_packet_3_response_values.flt[4] += 1.25f;
    test_packet_3_response_values.flt[5] += 1.25f;
    test_packet_3_response_values.flt[6] += 1.25f;

    SERIAL_RPC_RESPONSE_PAYLOAD_SET_FLOAT(handle, 0, (test_packet_3_response_values.flt[0]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_FLOAT(handle, 4, (test_packet_3_response_values.flt[1]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_FLOAT(handle, 8, (test_packet_3_response_values.flt[2]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_FLOAT(handle, 12, (test_packet_3_response_values.flt[3]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_FLOAT(handle, 16, (test_packet_3_response_values.flt[4]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_FLOAT(handle, 20, (test_packet_3_response_values.flt[5]));
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_FLOAT(handle, 24, (test_packet_3_response_values.flt[6]));

    test_response_callback_counter_3++;
}
