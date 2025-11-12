/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_AUDIO_SPEAKER_H__
#define __USB_AUDIO_SPEAKER_H__

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_audio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Audio speaker device structure */
typedef struct _usb_audio_speaker_struct
{
    usb_device_handle deviceHandle;           /* USB device handle */
    class_handle_t audioHandle;                /* USB audio class handle */
    uint32_t currentStreamInterfaceAlternateSetting; /* Current alternate setting */
    uint8_t copyProtect;                      /* Copy protect flag */
    uint8_t curMute;                          /* Current mute setting */
    uint8_t curVolume[2];                     /* Current volume setting */
    uint8_t minVolume[2];                     /* Min volume */
    uint8_t maxVolume[2];                     /* Max volume */
    uint8_t resVolume[2];                     /* Resolution volume */
    uint8_t curBass;                          /* Current bass */
    uint8_t minBass;                          /* Min bass */
    uint8_t maxBass;                          /* Max bass */
    uint8_t resBass;                          /* Resolution bass */
    uint8_t curMid;                           /* Current mid-range */
    uint8_t minMid;                           /* Min mid-range */
    uint8_t maxMid;                           /* Max mid-range */
    uint8_t resMid;                           /* Resolution mid-range */
    uint8_t curTreble;                        /* Current treble */
    uint8_t minTreble;                        /* Min treble */
    uint8_t maxTreble;                        /* Max treble */
    uint8_t resTreble;                        /* Resolution treble */
    uint8_t curAutomaticGain;                 /* Current automatic gain */
    uint32_t curDelay[2];                     /* Current delay */
    uint32_t minDelay[2];                     /* Min delay */
    uint32_t maxDelay[2];                     /* Max delay */
    uint32_t resDelay[2];                     /* Resolution delay */
    uint32_t curSamplingFrequency;            /* Current sampling frequency */
    uint32_t minSamplingFrequency;            /* Min sampling frequency */
    uint32_t maxSamplingFrequency;            /* Max sampling frequency */
    uint32_t resSamplingFrequency;            /* Resolution sampling frequency */
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
    uint32_t curSampleFrequency;              /* Current sample frequency */
    uint32_t curClockValid;                   /* Clock valid flag */
    usb_device_control_range_layout3_struct_t freqControlRange; /* Frequency control range */
    usb_device_control_range_layout2_struct_t volumeControlRange; /* Volume control range */
#endif
    uint8_t currentConfiguration;             /* Current configuration */
    uint8_t currentInterfaceAlternateSetting[USB_AUDIO_SPEAKER_INTERFACE_COUNT]; /* Current alternate setting */
    uint8_t speed;                            /* Speed of USB device */
    uint8_t attach;                           /* Device attach/detach status */
} usb_audio_speaker_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief USB device application initialization.
 *
 * This function initializes the USB device, audio class, and prepares for enumeration.
 */
extern void USB_DeviceApplicationInit(void);

/*!
 * @brief USB device audio callback.
 *
 * This function handles audio class events.
 *
 * @param handle Audio class handle.
 * @param event Event type.
 * @param param Event parameter.
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioCallback(class_handle_t handle, uint32_t event, void *param);

/*!
 * @brief USB device callback.
 *
 * This function handles USB device events.
 *
 * @param handle USB device handle.
 * @param event Event type.
 * @param param Event parameter.
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*!
 * @brief Get the global audio speaker instance.
 *
 * @return Pointer to the audio speaker structure.
 */
extern usb_audio_speaker_struct_t *USB_DeviceAudioSpeakerGetInstance(void);

#endif /* __USB_AUDIO_SPEAKER_H__ */
