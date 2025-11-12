/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_MISC_H__
#define __USB_MISC_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Define USB printf */
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#if defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE < 1)
#define usb_echo printf
#else
#include "fsl_debug_console.h"
#define usb_echo PRINTF
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @brief Define big endian */
#define USB_BIG_ENDIAN (0U)
#define USB_LITTLE_ENDIAN (1U)

/*! @brief Define current endian */
#ifndef ENDIANNESS
#define ENDIANNESS USB_LITTLE_ENDIAN
#endif

/*! @brief Define USB OSA event handle */
#define USB_OSA_SR_ALLOC() uint32_t usbOsaCurrentSr
#define USB_OSA_ENTER_CRITICAL() usbOsaCurrentSr = USB_OsaEnterCritical()
#define USB_OSA_EXIT_CRITICAL() USB_OsaExitCritical(usbOsaCurrentSr)

/*! @brief Define USB OSA functions */
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

static inline uint32_t USB_OsaEnterCritical(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

static inline void USB_OsaExitCritical(uint32_t primask)
{
    __set_PRIMASK(primask);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @brief Define for SDK OSA compatibility */
#ifndef OSA_SR_ALLOC
#define OSA_SR_ALLOC() USB_OSA_SR_ALLOC()
#endif

#ifndef OSA_ENTER_CRITICAL
#define OSA_ENTER_CRITICAL() USB_OSA_ENTER_CRITICAL()
#endif

#ifndef OSA_EXIT_CRITICAL
#define OSA_EXIT_CRITICAL() USB_OSA_EXIT_CRITICAL()
#endif

#endif /* __USB_MISC_H__ */
