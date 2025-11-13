/*
 * app_init.c
 *
 * Top-level application initialization:
 *   - initialize audio bridge (buffers)
 *   - initialize SAI EDMA (TDM RX/TX) using SDK example functions
 *   - initialize USB device audio (UAC2) using SDK USB example functions
 */

#include "app_config.h"
#include "audio_bridge.h"
#include "usb_device_board_init.h"
#include <stdint.h>
#include <stdio.h>

/* --- Externs: match these to functions in your imported SDK examples --- */
extern void sai_edma_tdm_record_playback_init(void);
extern void USB_DeviceApplicationInit(void);

/* Application init */
void APP_Init(void)
{
    /* Initialize the audio bridge ring buffers and state */
    audio_bridge_init();

    /* Initialize SAI EDMA TDM (RX + TX) */
    sai_edma_tdm_record_playback_init();

    /* Initialize USB PHY, clocks, and controller */
    BOARD_USB_DeviceInit();

    /* Initialize USB device stack and UAC2 device */
    USB_DeviceApplicationInit();

    /* At this point:
       - SAI EDMA is receiving/transmitting audio data via EDMA
       - Audio bridge ring buffers are ready
       - USB device will enumerate when cable is plugged in
    */
}
