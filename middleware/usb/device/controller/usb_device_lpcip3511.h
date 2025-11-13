/*
 * USB Device Controller Driver for RT1176 (IP3511HS)
 * Based on NXP IP3511 USB High-Speed Device Controller
 */

#ifndef _USB_DEVICE_LPCIP3511_H_
#define _USB_DEVICE_LPCIP3511_H_

#include "usb.h"
#include "usb_device_config.h"
#include "usb_device_dci.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief USB endpoint status structure */
typedef struct _usb_device_lpcip3511_endpoint_state_struct
{
    uint8_t *transferBuffer;          /*!< Address of buffer containing data to be transmitted */
    uint32_t transferLength;          /*!< Length of data to transmit */
    uint32_t transferDone;            /*!< Transferred length */
    uint16_t maxPacketSize;           /*!< Maximum packet size */
    uint8_t stateUnion;               /*!< Endpoint state union */
    uint8_t epPacketBuffer[64];       /*!< Endpoint buffer for small packets */
} usb_device_lpcip3511_endpoint_state_struct_t;

/*! @brief USB device LPC IP3511 state structure */
typedef struct _usb_device_lpcip3511_state_struct
{
    usb_device_struct_t *deviceHandle;  /*!< Device handle */
    void *registerBase;                 /*!< Register base address */
    usb_device_lpcip3511_endpoint_state_struct_t endpointState[16]; /*!< Endpoint state */
    uint8_t controllerId;               /*!< Controller ID */
    uint8_t isResetting;                /*!< Is resetting */
    uint8_t setupData[8];               /*!< Setup packet buffer */
} usb_device_lpcip3511_state_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the USB device controller.
 *
 * @param controllerId The controller id.
 * @param handle Pointer to the device handle.
 * @param lpcip3511Handle Double pointer to the controller state structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceLpcIp3511Init(uint8_t controllerId,
                                      usb_device_handle handle,
                                      usb_device_lpcip3511_state_struct_t **lpcip3511Handle);

/*!
 * @brief De-initialize the USB device controller.
 *
 * @param lpcip3511Handle Pointer to the controller state structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceLpcIp3511Deinit(usb_device_lpcip3511_state_struct_t *lpcip3511Handle);

/*!
 * @brief Send data through a specified endpoint.
 *
 * @param lpcip3511Handle Pointer to the controller state structure.
 * @param endpointAddress Endpoint index.
 * @param buffer The memory address to save the data.
 * @param length The length of the data.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceLpcIp3511Send(usb_device_lpcip3511_state_struct_t *lpcip3511Handle,
                                      uint8_t endpointAddress,
                                      uint8_t *buffer,
                                      uint32_t length);

/*!
 * @brief Receive data through a specified endpoint.
 *
 * @param lpcip3511Handle Pointer to the controller state structure.
 * @param endpointAddress Endpoint index.
 * @param buffer The memory address to save the data.
 * @param length The length of the data.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceLpcIp3511Recv(usb_device_lpcip3511_state_struct_t *lpcip3511Handle,
                                      uint8_t endpointAddress,
                                      uint8_t *buffer,
                                      uint32_t length);

/*!
 * @brief Cancel the pending transfer in a specified endpoint.
 *
 * @param lpcip3511Handle Pointer to the controller state structure.
 * @param endpointAddress Endpoint address.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceLpcIp3511Cancel(usb_device_lpcip3511_state_struct_t *lpcip3511Handle, uint8_t endpointAddress);

/*!
 * @brief Control the status of the selected endpoint.
 *
 * @param lpcip3511Handle Pointer to the controller state structure.
 * @param type The command type.
 * @param param The param of the command.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceLpcIp3511Control(usb_device_lpcip3511_state_struct_t *lpcip3511Handle,
                                         usb_device_control_type_t type,
                                         void *param);

#if defined(__cplusplus)
}
#endif

#endif /* _USB_DEVICE_LPCIP3511_H_ */
