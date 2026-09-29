#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART2.h>

#include "ti_drivers_config.h"

UART2_Handle uartHandle;
UART2_Params uartParams;

static void uartCallback(UART2_Handle handle, void *buf, size_t count, void *userArg, int_fast16_t status)
{
    __asm volatile("bkpt 0");
}

void *mainThread(void *arg0)
{
    static uint8_t dummy;

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

    while (1)
    {
        UART2_read(uartHandle, &dummy, 1, NULL);
    }
}
