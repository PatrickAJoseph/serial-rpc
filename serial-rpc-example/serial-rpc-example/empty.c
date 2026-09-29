#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART2.h>

#include "ti_drivers_config.h"
#include <serial_rpc.h>

#define BUS_TARGET_ADDRESS          (0x00)

UART2_Handle uartHandle;
UART2_Params uartParams;

/* Callback functions for response handling. */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_0_control);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_0_status);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_1_control);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_1_status);

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_0_control);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_0_status);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_1_control);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_1_status);

/* Response callback table. */

SERIAL_RPC_RESPONSE_CALLBACK_TABLE_DEFINE(response_callback_table) = 
{
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0, led_0_control, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(1, led_0_status, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(2, led_1_control, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(3, led_1_status, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(4, button_0_control, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(5, button_0_status, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(6, button_1_control, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(7, button_1_status, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_END,    
};

/* Function to send out bytes using UART. */
static void uartSend(uint8_t* data, size_t length)
{
    int ret;

    ret = UART2_write(uartHandle, data, length, NULL);

    if(ret)
    {
        __asm volatile("bkpt 0");
    }
}

/* Serial RPC handle definition. */
SERIAL_RPC_HANDLE_DEFINE(rpc_handle, BUS_TARGET_ADDRESS, response_callback_table, uartSend);

/* UART read callback function. */
static void uartCallback(UART2_Handle handle, void *buf, size_t count, void *userArg, int_fast16_t status)
{
    uint8_t* data;
    int index;

    for( index = 0 ; index < (int)count ; index++ )
    {
        serial_rpc_handle_rx_byte(&rpc_handle, data[index]);
    }
}

void *mainThread(void *arg0)
{
    static uint8_t dummy;
    int ret;

    GPIO_init();

    /* Initialize UART. */

    uartParams.baudRate = 115200;
    uartParams.readMode = UART2_Mode_CALLBACK;
    uartParams.readCallback = uartCallback;

    uartHandle = UART2_open(CONFIG_UART2_0, &uartParams);

    if(!uartHandle)
    {
        __asm volatile("bkpt 0");
    }

    /* Initialize serial RPC handle. */
    ret = serial_rpc_init(&rpc_handle);

    if(ret)
    {
        __asm volatile("bkpt 0");
    }

    while (1)
    {
        /* Read incoming bytes from UART RX line. */
        UART2_read(uartHandle, &dummy, 1, NULL);

        /* Process existing RPC packets. */
        (void)serial_rpc_process(&rpc_handle);
    }
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_0_control)
{

}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_0_status)
{

}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_1_control)
{

}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_1_status)
{

}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_0_control)
{

}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_0_status)
{

}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_1_control)
{

}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_1_status)
{

}
