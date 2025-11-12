/*
 * main.c
 * Application entry for RT1176 SAI <-> USB Audio bridge (CM7)
 *
 * This file assumes the following helper functions exist somewhere in the project:
 *  - void BOARD_ConfigMPU(void);
 *  - void BOARD_InitBootPins(void);
 *  - void BOARD_InitBootClocks(void);
 *  - void BOARD_InitDebugConsole(void);
 *
 * And the app-level initialization function provided in app_init.c:
 *  - void APP_Init(void);
 *
 * If you use FreeRTOS enable USE_FREERTOS below (and provide tasks in freertos/).
 */

#include <stdio.h>
#include <stdint.h>
#include "app_config.h"
#include "fsl_debug_console.h"   /* optional - depends on SDK headers available */
#include "board.h"               /* optional - include if present in SDK */
#include "audio_bridge.h"        /* your bridge header (from codec_bridge/) */

#ifndef USE_FREERTOS
#define USE_FREERTOS 0
#endif

/* Forward declarations for board helpers (provided by your SDK/board files).
   If your included SDK names differ, change these declarations or include the
   correct board header that provides them. */
void BOARD_ConfigMPU(void);
void BOARD_InitBootPins(void);
void BOARD_InitBootClocks(void);
void BOARD_InitDebugConsole(void);

extern void APP_Init(void);

int main(void)
{
    /* Basic board and debug init */
    BOARD_ConfigMPU();            /* optional; some SDKs call this in startup */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Application-level init (SAI edma, USB stack, audio bridge, tasks) */
    APP_Init();

#if USE_FREERTOS
    /* If FreeRTOS is used, APP_Init should have created tasks and we start scheduler here */
    extern void vTaskStartScheduler(void);
    vTaskStartScheduler();

    /* Should never reach here */
    for (;;)
    {
        __asm("wfi");
    }
#else
    /* Bare-metal loop: APP_Init should have started needed DMA transfers and USB device */
    while (1)
    {
        /* Optionally put the core in low-power while waiting for interrupts */
        __asm("wfi");
    }
#endif

    /* Never reached */
    return 0;
}
