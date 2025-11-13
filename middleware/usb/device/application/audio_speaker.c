/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_audio.h"
#include "usb_device_descriptor.h"
#include "audio_speaker.h"
#include "audio_bridge.h"
#include "usb_device_board_init.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USB audio transfer buffer size - must hold at least one 1ms frame */
#define AUDIO_SPEAKER_DATA_BUFFER_SIZE (FS_ISO_OUT_ENDP_PACKET_SIZE * 2)
#define AUDIO_RECORDER_DATA_BUFFER_SIZE (FS_ISO_OUT_ENDP_PACKET_SIZE * 2)

/* Feedback endpoint support */
#if (!defined(USB_DEVICE_AUDIO_USE_SYNC_MODE)) || (!USB_DEVICE_AUDIO_USE_SYNC_MODE)
#define USB_AUDIO_USE_FEEDBACK_ENDPOINT 1
#else
#define USB_AUDIO_USE_FEEDBACK_ENDPOINT 0
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void USB_DeviceAudioSpeakerStatusReset(void);
static usb_status_t USB_DeviceAudioSpeakerRequest(class_handle_t handle, uint32_t event, void *param);
static usb_status_t USB_DeviceAudioSpeakerSetInterface(class_handle_t handle,
                                                        uint8_t interface,
                                                        uint8_t alternateSetting);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Global audio speaker instance */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static usb_audio_speaker_struct_t s_audioSpeaker;

/* Audio OUT (playback) data buffer - receives data from host */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_audioSpeakerDataBuffer[AUDIO_SPEAKER_DATA_BUFFER_SIZE];

/* Audio IN (recording) data buffer - sends data to host */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_audioRecorderDataBuffer[AUDIO_RECORDER_DATA_BUFFER_SIZE];

#if USB_AUDIO_USE_FEEDBACK_ENDPOINT
/* Feedback data buffer (3 or 4 bytes depending on USB audio class version) */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_audioFeedbackBuffer[4] = {0x00, 0x00, 0xC0, 0x00}; /* 48.000 kHz nominal */
#endif

/* External references */
extern usb_device_class_struct_t g_UsbDeviceAudioClass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Get the audio speaker instance.
 */
usb_audio_speaker_struct_t *USB_DeviceAudioSpeakerGetInstance(void)
{
    return &s_audioSpeaker;
}

/*!
 * @brief Reset audio speaker status.
 */
static void USB_DeviceAudioSpeakerStatusReset(void)
{
    s_audioSpeaker.currentStreamInterfaceAlternateSetting = 0U;
    s_audioSpeaker.copyProtect = 0x01U;
    s_audioSpeaker.curMute = 0x00U;
    s_audioSpeaker.curVolume[0] = 0x00U;
    s_audioSpeaker.curVolume[1] = 0x80U;
    s_audioSpeaker.minVolume[0] = 0x00U;
    s_audioSpeaker.minVolume[1] = 0x80U;
    s_audioSpeaker.maxVolume[0] = 0xFFU;
    s_audioSpeaker.maxVolume[1] = 0x7FU;
    s_audioSpeaker.resVolume[0] = 0x01U;
    s_audioSpeaker.resVolume[1] = 0x00U;
    s_audioSpeaker.curBass = 0x00U;
    s_audioSpeaker.minBass = 0x80U;
    s_audioSpeaker.maxBass = 0x7FU;
    s_audioSpeaker.resBass = 0x01U;
    s_audioSpeaker.curMid = 0x00U;
    s_audioSpeaker.minMid = 0x80U;
    s_audioSpeaker.maxMid = 0x7FU;
    s_audioSpeaker.resMid = 0x01U;
    s_audioSpeaker.curTreble = 0x00U;
    s_audioSpeaker.minTreble = 0x80U;
    s_audioSpeaker.maxTreble = 0x7FU;
    s_audioSpeaker.resTreble = 0x01U;
    s_audioSpeaker.curAutomaticGain = 0x01U;
    s_audioSpeaker.curDelay[0] = 0x00U;
    s_audioSpeaker.curDelay[1] = 0x40U;
    s_audioSpeaker.minDelay[0] = 0x00U;
    s_audioSpeaker.minDelay[1] = 0x00U;
    s_audioSpeaker.maxDelay[0] = 0xFFU;
    s_audioSpeaker.maxDelay[1] = 0xFFU;
    s_audioSpeaker.resDelay[0] = 0x00U;
    s_audioSpeaker.resDelay[1] = 0x01U;
    s_audioSpeaker.curSamplingFrequency = 48000U;
    s_audioSpeaker.minSamplingFrequency = 48000U;
    s_audioSpeaker.maxSamplingFrequency = 48000U;
    s_audioSpeaker.resSamplingFrequency = 0U;

#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
    s_audioSpeaker.curSampleFrequency = 48000U;
    s_audioSpeaker.curClockValid = 1U;
    s_audioSpeaker.freqControlRange.wNumSubRanges = 1U;
    s_audioSpeaker.freqControlRange.wMIN = 48000U;
    s_audioSpeaker.freqControlRange.wMAX = 48000U;
    s_audioSpeaker.freqControlRange.wRES = 0U;
    s_audioSpeaker.volumeControlRange.wNumSubRanges = 1U;
    s_audioSpeaker.volumeControlRange.wMIN = 0x8000U;
    s_audioSpeaker.volumeControlRange.wMAX = 0x7FFFU;
    s_audioSpeaker.volumeControlRange.wRES = 1U;
#endif
}

/*!
 * @brief Handle SET_INTERFACE request for audio streaming interface.
 */
static usb_status_t USB_DeviceAudioSpeakerSetInterface(class_handle_t handle,
                                                        uint8_t interface,
                                                        uint8_t alternateSetting)
{
    usb_device_endpoint_init_struct_t epInitStruct;
    usb_device_endpoint_callback_struct_t epCallback;
    usb_status_t error = kStatus_USB_Success;

    if (interface == USB_AUDIO_SPEAKER_STREAM_INTERFACE_INDEX)
    {
        s_audioSpeaker.currentStreamInterfaceAlternateSetting = alternateSetting;

        if (alternateSetting == USB_AUDIO_SPEAKER_STREAM_INTERFACE_ALTERNATE_1)
        {
            /* Alternate setting 1: Audio streaming is active */

            /* Initialize isochronous OUT endpoint (host -> device, playback) */
            epInitStruct.zlt = 0U;
            epInitStruct.interval = FS_ISO_OUT_ENDP_INTERVAL;
            epInitStruct.endpointAddress = USB_AUDIO_SPEAKER_STREAM_ENDPOINT |
                                          (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
            epInitStruct.maxPacketSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
            epInitStruct.transferType = USB_ENDPOINT_ISOCHRONOUS;

            USB_DeviceInitEndpoint(s_audioSpeaker.deviceHandle, &epInitStruct, &epCallback);

#if USB_AUDIO_USE_FEEDBACK_ENDPOINT
            /* Initialize feedback endpoint (device -> host, asynchronous feedback) */
            epInitStruct.zlt = 0U;
            epInitStruct.interval = FS_ISO_FEEDBACK_ENDP_INTERVAL;
            epInitStruct.endpointAddress = USB_AUDIO_SPEAKER_FEEDBACK_ENDPOINT |
                                          (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
            epInitStruct.maxPacketSize = FS_ISO_FEEDBACK_ENDP_PACKET_SIZE;
            epInitStruct.transferType = USB_ENDPOINT_ISOCHRONOUS;

            USB_DeviceInitEndpoint(s_audioSpeaker.deviceHandle, &epInitStruct, &epCallback);
#endif

            /* Prime the first OUT transfer to start receiving audio from host */
            USB_DeviceAudioRecv(handle, USB_AUDIO_SPEAKER_STREAM_ENDPOINT,
                              s_audioSpeakerDataBuffer, FS_ISO_OUT_ENDP_PACKET_SIZE);

#if USB_AUDIO_USE_FEEDBACK_ENDPOINT
            /* Send first feedback packet */
            USB_DeviceAudioSend(handle, USB_AUDIO_SPEAKER_FEEDBACK_ENDPOINT,
                              s_audioFeedbackBuffer, FS_ISO_FEEDBACK_ENDP_PACKET_SIZE);
#endif

            PRINTF("USB Audio: Streaming started (alternate setting 1)\r\n");
        }
        else if (alternateSetting == USB_AUDIO_SPEAKER_STREAM_INTERFACE_ALTERNATE_0)
        {
            /* Alternate setting 0: Audio streaming is stopped */

            /* Deinitialize isochronous OUT endpoint */
            USB_DeviceDeinitEndpoint(s_audioSpeaker.deviceHandle,
                                    USB_AUDIO_SPEAKER_STREAM_ENDPOINT |
                                    (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));

#if USB_AUDIO_USE_FEEDBACK_ENDPOINT
            /* Deinitialize feedback endpoint */
            USB_DeviceDeinitEndpoint(s_audioSpeaker.deviceHandle,
                                    USB_AUDIO_SPEAKER_FEEDBACK_ENDPOINT |
                                    (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));
#endif

            PRINTF("USB Audio: Streaming stopped (alternate setting 0)\r\n");
        }
        else
        {
            error = kStatus_USB_InvalidRequest;
        }
    }

    return error;
}

/*!
 * @brief Handle audio class specific requests.
 */
static usb_status_t USB_DeviceAudioSpeakerRequest(class_handle_t handle, uint32_t event, void *param)
{
    usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;
    usb_status_t error = kStatus_USB_Success;

    switch (event)
    {
        case USB_DEVICE_AUDIO_FU_GET_CUR_MUTE_CONTROL:
            request->buffer = &s_audioSpeaker.curMute;
            request->length = sizeof(s_audioSpeaker.curMute);
            break;
        case USB_DEVICE_AUDIO_FU_GET_CUR_VOLUME_CONTROL:
            request->buffer = s_audioSpeaker.curVolume;
            request->length = sizeof(s_audioSpeaker.curVolume);
            break;
        case USB_DEVICE_AUDIO_FU_GET_CUR_BASS_CONTROL:
            request->buffer = &s_audioSpeaker.curBass;
            request->length = sizeof(s_audioSpeaker.curBass);
            break;
        case USB_DEVICE_AUDIO_FU_GET_CUR_MID_CONTROL:
            request->buffer = &s_audioSpeaker.curMid;
            request->length = sizeof(s_audioSpeaker.curMid);
            break;
        case USB_DEVICE_AUDIO_FU_GET_CUR_TREBLE_CONTROL:
            request->buffer = &s_audioSpeaker.curTreble;
            request->length = sizeof(s_audioSpeaker.curTreble);
            break;
        case USB_DEVICE_AUDIO_FU_GET_MIN_VOLUME_CONTROL:
            request->buffer = s_audioSpeaker.minVolume;
            request->length = sizeof(s_audioSpeaker.minVolume);
            break;
        case USB_DEVICE_AUDIO_FU_GET_MAX_VOLUME_CONTROL:
            request->buffer = s_audioSpeaker.maxVolume;
            request->length = sizeof(s_audioSpeaker.maxVolume);
            break;
        case USB_DEVICE_AUDIO_FU_GET_RES_VOLUME_CONTROL:
            request->buffer = s_audioSpeaker.resVolume;
            request->length = sizeof(s_audioSpeaker.resVolume);
            break;
        case USB_DEVICE_AUDIO_EP_GET_CUR_SAMPLING_FREQ_CONTROL:
            request->buffer = (uint8_t *)&s_audioSpeaker.curSamplingFrequency;
            request->length = sizeof(s_audioSpeaker.curSamplingFrequency);
            break;
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
        case USB_DEVICE_AUDIO_CS_GET_CUR_SAMPLING_FREQ_CONTROL:
            request->buffer = (uint8_t *)&s_audioSpeaker.curSampleFrequency;
            request->length = sizeof(s_audioSpeaker.curSampleFrequency);
            break;
        case USB_DEVICE_AUDIO_CS_SET_CUR_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = (uint8_t *)&s_audioSpeaker.curSampleFrequency;
            }
            break;
        case USB_DEVICE_AUDIO_CS_GET_CUR_CLOCK_VALID_CONTROL:
            request->buffer = (uint8_t *)&s_audioSpeaker.curClockValid;
            request->length = sizeof(s_audioSpeaker.curClockValid);
            break;
        case USB_DEVICE_AUDIO_CS_GET_RANGE_SAMPLING_FREQ_CONTROL:
            request->buffer = (uint8_t *)&s_audioSpeaker.freqControlRange;
            request->length = sizeof(s_audioSpeaker.freqControlRange);
            break;
        case USB_DEVICE_AUDIO_FU_GET_RANGE_VOLUME_CONTROL:
            request->buffer = (uint8_t *)&s_audioSpeaker.volumeControlRange;
            request->length = sizeof(s_audioSpeaker.volumeControlRange);
            break;
#endif
        case USB_DEVICE_AUDIO_FU_SET_CUR_MUTE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.curMute;
            }
            else
            {
                PRINTF("Set Mute: %d\r\n", s_audioSpeaker.curMute);
            }
            break;
        case USB_DEVICE_AUDIO_FU_SET_CUR_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.curVolume;
            }
            else
            {
                PRINTF("Set Volume: 0x%02x%02x\r\n", s_audioSpeaker.curVolume[1], s_audioSpeaker.curVolume[0]);
            }
            break;
        default:
            error = kStatus_USB_InvalidRequest;
            break;
    }
    return error;
}

/*!
 * @brief Audio class callback.
 */
usb_status_t USB_DeviceAudioCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    usb_device_endpoint_callback_message_struct_t *epCbParam;

    switch (event)
    {
        case kUSB_DeviceAudioEventStreamSendResponse:
            /* Feedback endpoint send complete - send next feedback packet */
#if USB_AUDIO_USE_FEEDBACK_ENDPOINT
            epCbParam = (usb_device_endpoint_callback_message_struct_t *)param;
            if ((epCbParam->length != 0) && (epCbParam->length != 0xFFFFFFFFU))
            {
                /* Continue sending feedback */
                error = USB_DeviceAudioSend(handle, USB_AUDIO_SPEAKER_FEEDBACK_ENDPOINT,
                                          s_audioFeedbackBuffer, FS_ISO_FEEDBACK_ENDP_PACKET_SIZE);
            }
#endif
            break;

        case kUSB_DeviceAudioEventStreamRecvResponse:
            /* Audio OUT data received from host (playback data) */
            epCbParam = (usb_device_endpoint_callback_message_struct_t *)param;
            if ((epCbParam->length != 0) && (epCbParam->length != 0xFFFFFFFFU))
            {
                /* Push received USB audio data to bridge for SAI TX playback */
                audio_bridge_push_usb_out(epCbParam->buffer, epCbParam->length);

                /* Prime next receive */
                error = USB_DeviceAudioRecv(handle, USB_AUDIO_SPEAKER_STREAM_ENDPOINT,
                                          s_audioSpeakerDataBuffer, FS_ISO_OUT_ENDP_PACKET_SIZE);
            }
            break;

        default:
            if ((event > kUSB_DeviceAudioEventRecvResponse) && (event < kUSB_DeviceAudioEventControlSendResponse))
            {
                /* Handle audio class specific requests */
                error = USB_DeviceAudioSpeakerRequest(handle, event, param);
            }
            break;
    }

    return error;
}

/*!
 * @brief USB device callback.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8 = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
            /* Reset device state */
            s_audioSpeaker.attach = 0U;
            s_audioSpeaker.currentConfiguration = 0U;
            USB_DeviceAudioSpeakerStatusReset();
            PRINTF("USB Device: Bus reset\r\n");
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed */
            error = USB_DeviceClassGetSpeed(CONTROLLER_ID, &s_audioSpeaker.speed);
            if (kStatus_USB_Success == error)
            {
                USB_DeviceSetSpeed(handle, s_audioSpeaker.speed);
            }
#endif
            break;

        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                s_audioSpeaker.attach = 0U;
                s_audioSpeaker.currentConfiguration = 0U;
            }
            else if (USB_AUDIO_SPEAKER_CONFIGURE_INDEX == (*temp8))
            {
                s_audioSpeaker.attach = 1U;
                s_audioSpeaker.currentConfiguration = *temp8;
                PRINTF("USB Device: Configured\r\n");
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;

        case kUSB_DeviceEventSetInterface:
            if (s_audioSpeaker.attach)
            {
                uint8_t interface = (uint8_t)((*temp8 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp8 & 0x00FFU);

                error = USB_DeviceAudioSpeakerSetInterface(s_audioSpeaker.audioHandle,
                                                          interface, alternateSetting);
            }
            break;

        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                *temp8 = s_audioSpeaker.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;

        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                uint8_t interface = (uint8_t)((*temp8 & 0xFF00U) >> 0x08U);
                if (interface < USB_AUDIO_SPEAKER_INTERFACE_COUNT)
                {
                    *temp8 = s_audioSpeaker.currentInterfaceAlternateSetting[interface];
                    error = kStatus_USB_Success;
                }
                else
                {
                    error = kStatus_USB_InvalidRequest;
                }
            }
            break;

        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;

        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                            (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;

        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;

        default:
            break;
    }

    return error;
}

/*!
 * @brief USB device application initialization.
 */
void USB_DeviceApplicationInit(void)
{
    usb_status_t error = kStatus_USB_Success;

    /* Initialize the audio speaker structure */
    s_audioSpeaker.speed = USB_SPEED_FULL;
    s_audioSpeaker.attach = 0U;
    s_audioSpeaker.audioHandle = (class_handle_t)NULL;
    s_audioSpeaker.deviceHandle = NULL;

    /* Reset audio status */
    USB_DeviceAudioSpeakerStatusReset();

    /* Initialize USB device */
    error = USB_DeviceClassInit(CONTROLLER_ID, &g_UsbDeviceAudioClass, &s_audioSpeaker.deviceHandle);
    if (error != kStatus_USB_Success)
    {
        PRINTF("USB Device Init failed: error 0x%x\r\n", error);
        return;
    }

    /* Get audio class handle */
    s_audioSpeaker.audioHandle = g_UsbDeviceAudioClass.classHandle;

    /* Enable USB device interrupt */
    USB_DeviceIsrEnable();

    /* Run USB device */
    error = USB_DeviceRun(s_audioSpeaker.deviceHandle);
    if (error != kStatus_USB_Success)
    {
        PRINTF("USB Device Run failed: error 0x%x\r\n", error);
        return;
    }

    PRINTF("USB Audio device initialized successfully\r\n");
    PRINTF("Waiting for USB enumeration...\r\n");
    PRINTF("Connect USB cable to see device on your laptop\r\n");
}

/*!
 * @brief USB OTG1 interrupt handler for RT1176.
 */
void USB_OTG1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(s_audioSpeaker.deviceHandle);
}
