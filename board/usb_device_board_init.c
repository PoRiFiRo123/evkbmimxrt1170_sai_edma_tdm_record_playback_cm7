/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_clock.h"
#include "board.h"
#include "usb_device_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Externdeclare of USB device handle from application layer */
extern usb_device_handle g_UsbDeviceAudioHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initialize USB device clocks.
 *
 * This function configures clocks for USB device operation on RT1176.
 */
void USB_DeviceClockInit(void)
{
    /* Enable USB HS clock */
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);

    /* Note: USB PHY initialization is typically done by the ROM bootloader on RT1176.
     * If additional PHY configuration is needed, it should be added here. */
}

/*!
 * @brief Enable USB device interrupt.
 *
 * This function enables the USB interrupt for the RT1176.
 */
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber                    = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
#endif

    /* Install USB device interrupt handler */
    NVIC_SetPriority((IRQn_Type)irqNumber, 3);
    EnableIRQ((IRQn_Type)irqNumber);
}

/*!
 * @brief Board-specific USB device initialization.
 *
 * Call this function before initializing USB device stack.
 */
void BOARD_USB_DeviceInit(void)
{
    /* Initialize USB clocks */
    USB_DeviceClockInit();
}
