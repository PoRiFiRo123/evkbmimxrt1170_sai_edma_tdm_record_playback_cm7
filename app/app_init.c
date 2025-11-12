/*
 * app_init.c
 *
 * Top-level application initialization:
 *   - initialize audio bridge (buffers)
 *   - initialize SAI EDMA (TDM RX/TX) using SDK example functions
 *   - initialize USB device audio (UAC2) using SDK USB example functions
 *
 * This file intentionally uses extern declarations for SDK/example functions so
 * you can map them to the existing names in the SDK sample you imported.
 */

#include "app_config.h"
#include "audio_bridge.h"
/* #include "usb_device_board_init.h" */ /* TODO: Uncomment when USB SDK added */
#include <stdint.h>
#include <stdio.h>

/* --- Externs: match these to functions in your imported SDK examples --- */
/* SAI EDMA init function from `sai_edma_tdm_record_playback_cm7` example.
   Replace with the actual function name in that example if different. */
extern void sai_edma_tdm_record_playback_init(void);

/* USB device application init (from SDK USB audio example or your added USB stack).
   Replace with the actual init function from the SDK example. */
/* extern void USB_DeviceApplicationInit(void); */ /* TODO: Uncomment when USB SDK added */

/* If you use board-specific audio PLL init routines, declare here */
extern void BOARD_AudioPllInit(void); /* optional: set up audio PLL if required */

/* Optionally, SDK may require enabling caches, etc. */
extern void BOARD_EnableCaches(void);

/* Application init */
void APP_Init(void)
{
    /* Optional: enable caches if SDK requires */
    /* BOARD_EnableCaches(); */

    /* Optional: audio PLL setup (if your board requires manual PLL configuration) */
    /* BOARD_AudioPllInit(); */

    /* Initialize the audio bridge ring buffers and state */
    audio_bridge_init();

    /* Initialize SAI EDMA TDM (RX + TX). The SDK example contains the actual EDMA init
       and starts DMA in circular mode. Ensure the function name here matches your example. */
    sai_edma_tdm_record_playback_init();

    /* TODO: USB initialization currently disabled - requires NXP USB SDK middleware
     * Uncomment these after copying USB middleware from NXP SDK:
     * BOARD_USB_DeviceInit();
     * USB_DeviceApplicationInit();
     */

    /* At this point:
       - SAI EDMA should be receiving/transmitting into example DMA buffers,
       - USB device should be ready to enumerate.
       The bridge code connects SAI <-> USB data flows via ring buffers.
    */
}
