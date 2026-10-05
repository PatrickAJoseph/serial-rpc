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

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_u8);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_i8);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_u16);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_i16);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_u32);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_i32);
static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_float);

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
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0x08, test_u8, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0x09, test_i8, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0x0A, test_u16, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0x0B, test_i16, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0x0C, test_u32, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0x0D, test_i32, NULL),
    SERIAL_RPC_RESPONSE_CALLBACK_TABLE_ENTRY_DEFINE(0x0E, test_float, NULL),
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
    uint8_t* data = buf;
    int index;

    for( index = 0 ; index < (int)count ; index++ )
    {
        serial_rpc_handle_rx_byte(&rpc_handle, data[index]);
    }
}

static void gpioCallback(uint_least8_t index); 

uint8_t dummy[SERIAL_RPC_PACKET_SIZE];

void *mainThread(void *arg0)
{
    int ret;

    /* Initialize GPIO. */

    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioCallback);
    GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioCallback);
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);
    GPIO_enableInt(CONFIG_GPIO_BUTTON_1);

    GPIO_init();

    /* Initialize UART. */

    UART2_Params_init(&uartParams);

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
        UART2_read(uartHandle, dummy, SERIAL_RPC_PACKET_SIZE, NULL);

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
struct button_params button1_params;

/* Static functions. */ 

static void gpioCallback(uint_least8_t index)
{
    if(index == CONFIG_GPIO_BUTTON_0)
    {
        button0_params.press_count++;
        serial_rpc_send_notification(&rpc_handle, 0, &button0_params.press_count, 4);
    }

    if(index == CONFIG_GPIO_BUTTON_1)
    {
        button1_params.press_count++;
        serial_rpc_send_notification(&rpc_handle, 1, &button1_params.press_count, 4);
    }
}

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

/**
 *  Resets the button press counter.
 *
 */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_0_control)
{
    bool reset;

    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, ((uint8_t*)(&reset)) );

    if(reset)
    {
        button0_params.press_count = 0;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), 1 );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, 0 );
}

/**
 *  Gets the state of BTN-1.
 */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_0_status)
{
    button0_params.state = GPIO_read(CONFIG_GPIO_BUTTON_0);

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle),  2 );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, (button0_params.state) );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 1, (button0_params.press_count) );
}

/**
 *  Resets the button press counter.
 *
 */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_1_control)
{
    bool reset;

    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, ((uint8_t*)(&reset)) );

    if(reset)
    {
        button1_params.press_count = 0;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), 1 );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, 0 );
}


/**
 *  Gets the state of BTN-2.
 */

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(button_1_status)
{
    button1_params.state = GPIO_read(CONFIG_GPIO_BUTTON_1);

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle),  2 );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, (button1_params.state) );
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 1, (button1_params.press_count) );
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_u8)
{
    uint8_t length;
    uint8_t index;
    uint8_t values[6];
    
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, (&length) );

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), (index + 1), (&values[index]) );
        values[index]++;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), ( length + 1 ));
    
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, length);

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), (index + 1), (values[index]));
    }
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_i8)
{
    uint8_t length;
    uint8_t index;
    int8_t values[6];
    
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, (&length) );

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_REQUEST_PAYLOAD_GET_INT8( (&rpc_handle), (index + 1), (&values[index]) );
        values[index]++;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), ( length + 1 ));
    
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, length);

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT8( (&rpc_handle), (index + 1), (values[index]));
    }
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_u16)
{
    uint8_t length;
    uint8_t index;
    uint16_t values[6];
    
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, (&length) );

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT16( (&rpc_handle), ((2*index) + 1), (&values[index]) );
        values[index]++;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), ( length + 1 ));
    
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, length);

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT16( (&rpc_handle), ((2*index) + 1), (values[index]));
    }
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_i16)
{
    uint8_t length;
    uint8_t index;
    int16_t values[6];
    
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, (&length) );

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_REQUEST_PAYLOAD_GET_INT16( (&rpc_handle), ((2*index) + 1), (&values[index]) );
        values[index]++;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), ( length + 1 ));
    
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, length);

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT16( (&rpc_handle), ((2*index) + 1), (values[index]));
    }
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_u32)
{
    uint8_t length;
    uint8_t index;
    uint32_t values[6];
    
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, (&length) );

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT32( (&rpc_handle), ((4*index) + 1), (&values[index]) );
        values[index]++;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), ( length + 1 ));
    
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, length);

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT32( (&rpc_handle), ((4*index) + 1), (values[index]));
    }
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_i32)
{
    uint8_t length;
    uint8_t index;
    int32_t values[6];
    
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, (&length) );

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_REQUEST_PAYLOAD_GET_INT32( (&rpc_handle), ((4*index) + 1), (&values[index]) );
        values[index]++;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), ( length + 1 ));
    
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, length);

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_RESPONSE_PAYLOAD_SET_INT32( (&rpc_handle), ((4*index) + 1), (values[index]));
    }
}

static SERIAL_RPC_RESPONSE_CALLBACK_DEFINE(test_float)
{
    uint8_t length;
    uint8_t index;
    float values[6];
    
    SERIAL_RPC_REQUEST_PAYLOAD_GET_UINT8( (&rpc_handle), 0, (&length) );

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_REQUEST_PAYLOAD_GET_FLOAT( (&rpc_handle), ((4*index) + 1), (&values[index]) );
        values[index] += 1.0f;
    }

    SERIAL_RPC_RESPONSE_SET_PAYLOAD_LENGTH( (&rpc_handle), ( length + 1 ));
    
    SERIAL_RPC_RESPONSE_PAYLOAD_SET_UINT8( (&rpc_handle), 0, length);

    for( index = 0 ; index < length ; index++ )
    {
        SERIAL_RPC_RESPONSE_PAYLOAD_SET_FLOAT( (&rpc_handle), (index + 1), (values[index]));
    }
}
