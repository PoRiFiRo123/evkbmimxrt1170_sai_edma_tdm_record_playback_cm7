/*
 * USB Device Configuration for RT1176
 * This file configures the USB device stack for RT1176's IP3511HS controller
 */

#ifndef _USB_DEVICE_CONFIG_H_
#define _USB_DEVICE_CONFIG_H_

/*! @brief RT1176 uses IP3511HS USB controller */
#define USB_DEVICE_CONFIG_LPCIP3511HS (1U)

/*! @brief Number of endpoints supported (including EP0) */
#define USB_DEVICE_CONFIG_ENDPOINTS (4U)

/*! @brief USB device speed: 0 = Full Speed, 1 = High Speed */
#define USB_DEVICE_CONFIG_EHCI_MAX_ITD (0U)
#define USB_DEVICE_CONFIG_EHCI_MAX_SITD (0U)

/*! @brief Memory alignment for USB buffers */
#define USB_DATA_ALIGN_SIZE (64U)
/* USB_DATA_ALIGN_SIZE_MULTIPLE is defined as a macro function in usb_spec.h */

/*! @brief USB buffer cache control */
#ifndef USB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE
#define USB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE (1U)
#endif

/*! @brief Whether device is self powered. 1U supported, 0U not supported */
#ifndef USB_DEVICE_CONFIG_SELF_POWER
#define USB_DEVICE_CONFIG_SELF_POWER (1U)
#endif

/*! @brief Whether device remote wakeup is supported. 1U supported, 0U not supported */
#ifndef USB_DEVICE_CONFIG_REMOTE_WAKEUP
#define USB_DEVICE_CONFIG_REMOTE_WAKEUP (0U)
#endif

/*! @brief USB device control IN/OUT transfer buffer size */
#define USB_DEVICE_CONFIG_SETUP_PACKET_SIZE (8U)

/*! @brief Device vendor ID (NXP) */
#define USB_DEVICE_VID (0x1FC9U)

/*! @brief Device product ID */
#define USB_DEVICE_PID (0x0098U)

#endif /* _USB_DEVICE_CONFIG_H_ */
