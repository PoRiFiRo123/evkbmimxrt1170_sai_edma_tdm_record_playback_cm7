/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_H__
#define __USB_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "usb_spec.h"
#include "usb_misc.h"
#include "usb_device_config.h"

/*!
 * @addtogroup usb_drv
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines USB stack major version */
#define USB_STACK_VERSION_MAJOR (2U)
/*! @brief Defines USB stack minor version */
#define USB_STACK_VERSION_MINOR (4U)
/*! @brief Defines USB stack bugfix version */
#define USB_STACK_VERSION_BUGFIX (1U)

/*! @brief USB stack version definition */
#define USB_MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))

/*! @brief USB error code */
typedef enum _usb_status
{
    kStatus_USB_Success = 0x00U, /*!< Success */
    kStatus_USB_Error,           /*!< Failed */

    kStatus_USB_Busy,                       /*!< Busy */
    kStatus_USB_InvalidHandle,              /*!< Invalid handle */
    kStatus_USB_InvalidParameter,           /*!< Invalid parameter */
    kStatus_USB_InvalidRequest,             /*!< Invalid request */
    kStatus_USB_ControllerNotFound,         /*!< Controller cannot be found */
    kStatus_USB_InvalidControllerInterface, /*!< Invalid controller interface */

    kStatus_USB_NotSupported,      /*!< Configuration is not supported */
    kStatus_USB_Retry,             /*!< Enumeration get configuration retry */
    kStatus_USB_TransferStall,     /*!< Transfer stalled */
    kStatus_USB_TransferFailed,    /*!< Transfer failed */
    kStatus_USB_AllocFail,         /*!< Allocation failed */
    kStatus_USB_LackSwapBuffer,    /*!< Insufficient swap buffer for KHCI */
    kStatus_USB_TransferCancel,    /*!< The transfer cancelled */
    kStatus_USB_BandwidthFail,     /*!< Allocate bandwidth failed */
    kStatus_USB_MSDStatusFail,     /*!< For MSD, the CSW status means fail */
    kStatus_USB_EHCIAttached,
    kStatus_USB_EHCIDetached,
    kStatus_USB_Timeout,
} usb_status_t;

/*! @brief USB host handle type define */
typedef void *usb_host_handle;

/*! @brief USB device handle type define. For device stack, the device handle is same as the controller handle */
typedef void *usb_device_handle;

/*! @brief USB OTG handle type define */
typedef void *usb_otg_handle;

/*! @brief USB controller ID */
typedef enum _usb_controller_index
{
    kUSB_ControllerKhci0 = 0U, /*!< KHCI 0U */
    kUSB_ControllerKhci1 = 1U, /*!< KHCI 1U, Currently, there are no platforms which have two KHCI IPs, this is reserved
                                  to be used in the future. */
    kUSB_ControllerEhci0 = 2U, /*!< EHCI 0U */
    kUSB_ControllerEhci1 = 3U, /*!< EHCI 1U, Currently, there are no platforms which have two EHCI IPs, this is reserved
                                  to be used in the future. */

    kUSB_ControllerLpcIp3511Fs0 = 4U, /*!< LPC USB IP3511 FS controller 0 */
    kUSB_ControllerLpcIp3511Fs1 =
        5U, /*!< LPC USB IP3511 FS controller 1, there are no platforms which have two IP3511 IPs, this is reserved
               to be used in the future. */

    kUSB_ControllerLpcIp3511Hs0 = 6U, /*!< LPC USB IP3511 HS controller 0 */
    kUSB_ControllerLpcIp3511Hs1 =
        7U, /*!< LPC USB IP3511 HS controller 1, there are no platforms which have two IP3511 IPs, this is reserved
               to be used in the future. */

    kUSB_ControllerOhci0 = 8U, /*!< OHCI 0U */
    kUSB_ControllerOhci1 = 9U, /*!< OHCI 1U, Currently, there are no platforms which have two OHCI IPs, this is reserved
                                  to be used in the future. */

    kUSB_ControllerIp3516Hs0 = 10U, /*!< IP3516HS 0U */
    kUSB_ControllerIp3516Hs1 =
        11U, /*!< IP3516HS 1U, Currently, there are no platforms which have two IP3516HS IPs, this is reserved
                to be used in the future. */

    kUSB_ControllerDwc30 = 12U, /*!< DWC3 0U */
    kUSB_ControllerDwc31 = 13U, /*!< DWC3 1U */
} usb_controller_index_t;

/*! @brief USB stack version structure */
typedef struct _usb_version
{
    uint8_t major;  /*!< Major */
    uint8_t minor;  /*!< Minor */
    uint8_t bugfix; /*!< Bug fix */
} usb_version_t;

/*! @brief USB controller type */
typedef enum _usb_controller_type
{
    kUSB_ControllerTypeKhci = 1U,       /*!< KHCI */
    kUSB_ControllerTypeEhci = 2U,       /*!< EHCI */
    kUSB_ControllerTypeLpcIp3511Fs = 3U, /*!< LPC IP3511 FS controller */
    kUSB_ControllerTypeLpcIp3511Hs = 4U, /*!< LPC IP3511 HS controller */
    kUSB_ControllerTypeOhci = 5U,       /*!< OHCI */
    kUSB_ControllerTypeIp3516Hs = 6U,   /*!< IP3516HS */
    kUSB_ControllerTypeDwc3 = 7U,       /*!< DWC3 */
} usb_controller_type_t;

/*! @brief USB Setup Packet Structure */
typedef struct _usb_setup_struct
{
    uint8_t bmRequestType;  /*!< Request type */
    uint8_t bRequest;       /*!< Specific request */
    uint16_t wValue;        /*!< Value */
    uint16_t wIndex;        /*!< Index */
    uint16_t wLength;       /*!< Length of data */
} usb_setup_struct_t;

/*! @brief USB Device String Descriptor */
typedef struct _usb_language
{
    uint8_t **string;       /*!< String descriptor array */
    uint32_t *length;       /*!< String length array */
    uint16_t languageId;    /*!< Language ID */
} usb_language_t;

/*! @brief USB Device Language List */
typedef struct _usb_language_list
{
    uint8_t *languageString;     /*!< Language ID string descriptor */
    uint32_t stringLength;       /*!< Language string length */
    usb_language_t *languageList; /*!< Language list */
    uint8_t languageCount;       /*!< Language count */
} usb_language_list_t;

/*! @brief USB Device Struct */
typedef struct _usb_device_struct
{
    usb_device_handle handle;           /*!< Device handle */
    usb_device_handle controllerHandle; /*!< Controller handle */
    uint8_t controllerId;               /*!< Controller ID */
    uint8_t deviceAddress;              /*!< Device address */
    uint8_t state;                      /*!< Device state */
    uint8_t *setupBuffer;               /*!< Setup packet buffer */
} usb_device_struct_t;

/*! @brief USB Device Control Type */
typedef enum _usb_device_control_type
{
    kUSB_DeviceControlRun = 0U,
    kUSB_DeviceControlStop,
    kUSB_DeviceControlEndpointInit,
    kUSB_DeviceControlEndpointDeinit,
    kUSB_DeviceControlEndpointStall,
    kUSB_DeviceControlEndpointUnstall,
    kUSB_DeviceControlGetDeviceStatus,
    kUSB_DeviceControlGetEndpointStatus,
    kUSB_DeviceControlSetDeviceAddress,
    kUSB_DeviceControlGetSynchFrame,
    kUSB_DeviceControlResume,
    kUSB_DeviceControlSuspend,
    kUSB_DeviceControlSetDefaultStatus,
    kUSB_DeviceControlGetSpeed,
    kUSB_DeviceControlSetTestMode,
} usb_device_control_type_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Gets the USB stack version.
 *
 * @param version Pointer to a version structure.
 */
void USB_GetVersion(usb_version_t *version);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __USB_H__ */
