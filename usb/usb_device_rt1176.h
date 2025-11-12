/*
 * USB Device Header for RT1176
 */

#ifndef _USB_DEVICE_RT1176_H_
#define _USB_DEVICE_RT1176_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Initialize USB device */
void USB_DeviceApplicationInit(void);

/*! @brief USB device audio processing task (call periodically from main loop or timer) */
void USB_DeviceAudioTask(void);

/*! @brief USB interrupt handler (called from USB_OTG1_IRQHandler) */
void USB_DeviceIsrHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* _USB_DEVICE_RT1176_H_ */
