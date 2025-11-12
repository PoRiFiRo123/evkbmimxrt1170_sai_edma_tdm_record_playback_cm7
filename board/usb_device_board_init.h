/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_DEVICE_BOARD_INIT_H_
#define _USB_DEVICE_BOARD_INIT_H_

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Board-specific USB device initialization.
 *
 * Call this function before initializing USB device stack.
 */
void BOARD_USB_DeviceInit(void);

/*!
 * @brief Initialize USB device clocks and PHY.
 */
void USB_DeviceClockInit(void);

/*!
 * @brief Enable USB device interrupt.
 */
void USB_DeviceIsrEnable(void);

#if defined(__cplusplus)
}
#endif

#endif /* _USB_DEVICE_BOARD_INIT_H_ */
