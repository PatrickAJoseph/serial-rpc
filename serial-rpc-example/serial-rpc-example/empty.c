#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART2.h>

#include <ti/devices/cc23x0r5/driverlib/hapi.h>

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

/****************************** Application code starts here *********************8*/

/* Type definitions. */

struct led_params {
    bool state;
    uint8_t blink_count;
    uint16_t blink_interval;
    uint16_t total_blink_count;
};

struct button_params {
    bool state;
    uint8_t press_count;
};

/* Global variables. */

struct led_params led0_params;
struct led_params led1_params;
struct button_params button0_params;
struct button_params buton1_params;

/** 
 *  LED0 control RPC request packet payload has the following structure.
 *
 *  BYTE0       : set_led_0_state
 *  BYTE1       : led_0_state
 *  BYTE2       : set_led_0_blink_count
 *  BYTE3       : led_0_blink_count
 *  BYTE4       : set_led_0_blink_interval_ms
 *  BYTE5 & 6   : led_0_blink_interval_ms
 *  BYTE7       : led_0_blink    
*/

/**
 *  Controls the onboard red LED.
 */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_0_control)
{
    bool set_led0_state;
    bool state;
    bool set_led0_blink_count;
    uint8_t blink_count;
    bool set_led0_blink_interval_ms;
    uint16_t blink_interval;
    bool blink;

    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 0, ((uint8_t*)&set_led0_state));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 1, ((uint8_t*)&state));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 2, ((uint8_t*)&set_led0_blink_count));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 3, ((uint8_t*)&blink_count));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 4, ((uint8_t*)&set_led0_blink_interval_ms));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT16((&rpc_handle), 5, ((uint16_t*)&blink_interval));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 7, ((uint8_t*)&blink));

    if(set_led0_state) {
        led0_params.state = state;
        GPIO_write(CONFIG_GPIO_LED_0, state);
    }

    if(set_led0_blink_count) {
        led0_params.blink_count = blink_count;
    }

    if(set_led0_blink_interval_ms) {
        led0_params.blink_interval = blink_interval;
    }

    if(blink) 
    {
        int index;

        for(index = 0 ; index < led0_params.blink_count ; index++)
        {
            GPIO_write(CONFIG_GPIO_LED_0, 1);
            HapiWaitUs( 1000 * led0_params.blink_interval );
            GPIO_write(CONFIG_GPIO_LED_0, 0);
            HapiWaitUs( 1000 * led0_params.blink_interval );
            led0_params.total_blink_count++;
        }
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH((&rpc_handle), 1);
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8((&rpc_handle), 0, 0);
}

/**
  *  Gets status of LED0 (red LED).
  *
  *  BYTE0: current state of LED.
  *  BYTE1 & 2: number of times the LED has to blinked.
 */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_0_status)
{
    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH((&rpc_handle), 3);
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, (led0_params.state) );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16( (&rpc_handle), 1, (led0_params.total_blink_count)  );
}

/** 
 *  LED1 control RPC request packet payload has the following structure.
 *
 *  BYTE0       : set_led_1_state
 *  BYTE1       : led_1_state
 *  BYTE2       : set_led_1_blink_count
 *  BYTE3       : led_1_blink_count
 *  BYTE4       : set_led_1_blink_interval_ms
 *  BYTE5 & 6   : led_1_blink_interval_ms
 *  BYTE7       : led_1_blink    
*/

/**
 *  Controls the onboard green LED.
 */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_1_control)
{
    bool set_led1_state;
    bool state;
    bool set_led1_blink_count;
    uint8_t blink_count;
    bool set_led1_blink_interval_ms;
    uint16_t blink_interval;
    bool blink;

    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 0, ((uint8_t*)&set_led1_state));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 1, ((uint8_t*)&state));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 2, ((uint8_t*)&set_led1_blink_count));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 3, ((uint8_t*)&blink_count));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 4, ((uint8_t*)&set_led1_blink_interval_ms));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT16((&rpc_handle), 5, ((uint16_t*)&blink_interval));
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8((&rpc_handle), 7, ((uint8_t*)&blink));

    if(set_led1_state) {
        led1_params.state = state;
        GPIO_write(CONFIG_GPIO_LED_1, state);
    }

    if(set_led1_blink_count) {
        led1_params.blink_count = blink_count;
    }

    if(set_led1_blink_interval_ms) {
        led1_params.blink_interval = blink_interval;
    }

    if(blink) 
    {
        int index;

        for(index = 0 ; index < led1_params.blink_count ; index++)
        {
            GPIO_write(CONFIG_GPIO_LED_1, 1);
            HapiWaitUs( 1000 * led1_params.blink_interval );
            GPIO_write(CONFIG_GPIO_LED_1, 0);
            HapiWaitUs( 1000 * led1_params.blink_interval );
            led1_params.total_blink_count++;
        }
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH((&rpc_handle), 1);
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8((&rpc_handle), 0, 0);
}

/**
  *  Gets status of LED0 (red LED).
  *
  *  BYTE0: current state of LED.
  *  BYTE1 & 2: number of times the LED has to blinked.
 */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(led_1_status)
{
    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH((&rpc_handle), 5);
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, (led1_params.state) );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16( (&rpc_handle), 1, (led1_params.blink_count)  );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16( (&rpc_handle), 3, (led1_params.blink_interval) ); 
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_0_control)
{
    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH((&rpc_handle), 3);
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, (led1_params.state) );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16( (&rpc_handle), 1, (led1_params.total_blink_count)  );
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
