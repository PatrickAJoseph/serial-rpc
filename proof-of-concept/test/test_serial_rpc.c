#include "unity.h"
#include "../serial_rpc.h"

void setUp(void)
{
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

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_serial_rpc_packet_size);
    RUN_TEST(test_serial_rpc_control_packet_size);

    return UNITY_END();
}