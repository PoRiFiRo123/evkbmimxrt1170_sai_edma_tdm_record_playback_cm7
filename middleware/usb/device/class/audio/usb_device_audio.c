/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"

#if ((defined(USB_DEVICE_CONFIG_AUDIO)) && (USB_DEVICE_CONFIG_AUDIO > 0U))
#include "usb_device_audio.h"

/* Optional: enable the simple audio bridge integration.
   Define AUDIO_BRIDGE_ENABLE (1) in your app_config.h or project settings to enable.
   When enabled, the USB class will:
     - push received OUT packets into audio_bridge_push_usb_out(...)
     - pull IN packets from audio_bridge_pop_for_usb(...) automatically on send-complete
*/
#ifndef AUDIO_BRIDGE_ENABLE
#define AUDIO_BRIDGE_ENABLE 0U
#endif

#if (AUDIO_BRIDGE_ENABLE)
#include "audio_bridge.h" /* your bridge API: push/pop functions (you must implement) */
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static usb_status_t USB_DeviceAudioAllocateHandle(usb_device_audio_struct_t **handle);
static usb_status_t USB_DeviceAudioFreeHandle(usb_device_audio_struct_t *handle);
usb_status_t USB_DeviceAudioInterruptIn(usb_device_handle handle,
                                        usb_device_endpoint_callback_message_struct_t *message,
                                        void *callbackParam);
usb_status_t USB_DeviceAudioIsochronousIn(usb_device_handle handle,
                                          usb_device_endpoint_callback_message_struct_t *message,
                                          void *callbackParam);
usb_status_t USB_DeviceAudioIsochronousOut(usb_device_handle handle,
                                           usb_device_endpoint_callback_message_struct_t *message,
                                           void *callbackParam);
usb_status_t USB_DeviceAudioStreamEndpointsInit(usb_device_audio_struct_t *audioHandle);
usb_status_t USB_DeviceAudioStreamEndpointsDeinit(usb_device_audio_struct_t *audioHandle);
usb_status_t USB_DeviceAudioControlEndpointsInit(usb_device_audio_struct_t *audioHandle);
usb_status_t USB_DeviceAudioControlEndpointsDeinit(usb_device_audio_struct_t *audioHandle);
usb_status_t USB_DeviceAudioGetCurAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                   usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioSetCurAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                   usb_device_control_request_struct_t *controlRequest);
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
usb_status_t USB_DeviceAudioGetRangeAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                     usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioGetClockSource(usb_device_audio_struct_t *audioHandle,
                                           usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioSetClockSource(usb_device_audio_struct_t *audioHandle,
                                           usb_device_control_request_struct_t *controlRequest);
#else
usb_status_t USB_DeviceAudioGetMinAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                   usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioGetMaxAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                   usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioGetResAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                   usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioSetMinAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                   usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioSetMaxAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                   usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioSetResAudioFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                                   usb_device_control_request_struct_t *controlRequest);
#endif
usb_status_t USB_DeviceAudioSetFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                           usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioSetRequestEndpoint(usb_device_audio_struct_t *audioHandle,
                                               usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioGetRequestEndpoint(usb_device_audio_struct_t *audioHandle,
                                               usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioSetControlTerminal(usb_device_audio_struct_t *audioHandle,
                                               usb_device_control_request_struct_t *controlRequest,
                                               uint8_t terminal_type);
usb_status_t USB_DeviceAudioGetControlTerminal(usb_device_audio_struct_t *audioHandle,
                                               usb_device_control_request_struct_t *controlRequest,
                                               uint8_t terminal_type);
usb_status_t USB_DeviceAudioGetFeatureUnit(usb_device_audio_struct_t *audioHandle,
                                           usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioSetRequestInterface(usb_device_audio_struct_t *audioHandle,
                                                usb_device_control_request_struct_t *controlRequest);
usb_status_t USB_DeviceAudioGetRequestInterface(usb_device_audio_struct_t *audioHandle,
                                                usb_device_control_request_struct_t *controlRequest);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_GLOBAL USB_RAM_ADDRESS_ALIGNMENT(USB_DATA_ALIGN_SIZE) static usb_device_audio_struct_t
    s_UsbDeviceAudioHandle[USB_DEVICE_CONFIG_AUDIO];

/* Buffer used when pulling IN packets from the audio bridge.
   Size picks FS_ISO_OUT_ENDP_PACKET_SIZE if present; otherwise fallback to 1536 (safe for 8ch/48k/4B slots). */
#ifndef USB_AUDIO_IN_PACKET_BUFFER_SIZE
#if defined(FS_ISO_OUT_ENDP_PACKET_SIZE)
#define USB_AUDIO_IN_PACKET_BUFFER_SIZE FS_ISO_OUT_ENDP_PACKET_SIZE
#else
#define USB_AUDIO_IN_PACKET_BUFFER_SIZE 1536
#endif
#endif

/* static buffer for IN packet generation from bridge */
static uint8_t s_audioInPacket[USB_AUDIO_IN_PACKET_BUFFER_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Allocate a device audio class handle.
 *
 * This function allocates a device audio class handle.
 *
 * @param handle          It is out parameter, is used to return pointer of the device audio class handle to the caller.
 *
 * @retval kStatus_USB_Success              Get a device audio class handle successfully.
 * @retval kStatus_USB_Busy                 Cannot allocate a device audio class handle.
 */
static usb_status_t USB_DeviceAudioAllocateHandle(usb_device_audio_struct_t **handle)
{
    uint32_t count;
    OSA_SR_ALLOC();

    OSA_ENTER_CRITICAL();
    for (count = 0; count < USB_DEVICE_CONFIG_AUDIO; count++)
    {
        if (NULL == s_UsbDeviceAudioHandle[count].handle)
        {
            *handle = &s_UsbDeviceAudioHandle[count];
            OSA_EXIT_CRITICAL();
            return kStatus_USB_Success;
        }
    }
    OSA_EXIT_CRITICAL();
    return kStatus_USB_Busy;
}

/*!
 * @brief Free a device audio class handle.
 *
 * This function frees a device audio class handle.
 *
 * @param handle          The device audio class handle.
 *
 * @retval kStatus_USB_Success              Free device audio class handle successfully.
 */
static usb_status_t USB_DeviceAudioFreeHandle(usb_device_audio_struct_t *handle)
{
    OSA_SR_ALLOC();

    OSA_ENTER_CRITICAL();
    handle->handle           = NULL;
    handle->configStruct     = (usb_device_class_config_struct_t *)NULL;
    handle->configuration    = 0U;
    handle->controlAlternate = 0U;
    handle->streamAlternate  = 0U;
    OSA_EXIT_CRITICAL();
    return kStatus_USB_Success;
}

/*!
 * @brief Interrupt IN endpoint callback function.
 *
 * This callback function is used to notify uplayer the transfser result of a transfer.
 * This callback pointer is passed when the interrupt IN pipe initialized.
 *
 * @param handle          The device handle. It equals the value returned from USB_DeviceInit.
 * @param message         The result of the interrupt IN pipe transfer.
 * @param callbackParam  The parameter for this callback. It is same with
 * usb_device_endpoint_callback_struct_t::callbackParam. In the class, the value is the audio class handle.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceAudioInterruptIn(usb_device_handle handle,
                                        usb_device_endpoint_callback_message_struct_t *message,
                                        void *callbackParam)
{
    usb_device_audio_struct_t *audioHandle;
    usb_status_t error = kStatus_USB_Error;

    /* Get the audio class handle */
    audioHandle = (usb_device_audio_struct_t *)callbackParam;

    if (NULL == audioHandle)
    {
        return kStatus_USB_InvalidHandle;
    }

    if ((NULL != audioHandle->configStruct) && (NULL != audioHandle->configStruct->classCallback))
    {
        /* Notify the application control data sent by calling the audio class callback.
        classCallback is initialized in classInit of s_UsbDeviceClassInterfaceMap,
        it is from the second parameter of classInit */
        error = audioHandle->configStruct->classCallback((class_handle_t)audioHandle,
                                                         kUSB_DeviceAudioEventControlSendResponse, message);
    }

    return error;
}

/*!
 * @brief ISO IN endpoint callback function.
 *
 * This callback function is used to notify uplayer the transfser result of a transfer.
 * This callback pointer is passed when the ISO IN pipe initialized.
 *
 * @param handle          The device handle. It equals the value returned from USB_DeviceInit.
 * @param message         The result of the ISO IN pipe transfer.
 * @param callbackParam  The parameter for this callback. It is same with
 * usb_device_endpoint_callback_struct_t::callbackParam. In the class, the value is the audio class handle.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceAudioIsochronousIn(usb_device_handle handle,
                                          usb_device_endpoint_callback_message_struct_t *message,
                                          void *callbackParam)
{
    usb_device_audio_struct_t *audioHandle;
    usb_status_t status = kStatus_USB_Error;

    /* Get the audio class handle */
    audioHandle = (usb_device_audio_struct_t *)callbackParam;
    if (NULL == audioHandle)
    {
        return kStatus_USB_InvalidHandle;
    }
    audioHandle->streamInPipeBusy = 0U;

    if ((NULL != audioHandle->configStruct) && (NULL != audioHandle->configStruct->classCallback))
    {
        /* Notify the application stream data sent by calling the audio class callback.
        classCallback is initialized in classInit of s_UsbDeviceClassInterfaceMap,
        it is from the second parameter of classInit */
        status = audioHandle->configStruct->classCallback((class_handle_t)audioHandle,
                                                          kUSB_DeviceAudioEventStreamSendResponse, message);
    }

#if (AUDIO_BRIDGE_ENABLE)
    /* Bridge: automatically prepare next IN packet by pulling from the bridge buffer */
    {
        size_t got;
        /* Try to pop data from audio bridge into local buffer */
        got = audio_bridge_pop_for_usb(s_audioInPacket, sizeof(s_audioInPacket));
        if (got > 0)
        {
            /* send next IN packet (endpoint address uses stream endpoint number macro) */
            uint8_t ep = (uint8_t)(USB_AUDIO_SPEAKER_STREAM_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));
            /* Note: USB_DeviceAudioSend will set streamInPipeBusy and call USB_DeviceSendRequest */
            (void)USB_DeviceAudioSend((class_handle_t)audioHandle, ep, s_audioInPacket, (uint32_t)got);
        }
    }
#endif

    return status;
}

/*!
 * @brief ISO OUT endpoint callback function.
 *
 * This callback function is used to notify uplayer the transfser result of a transfer.
 * This callback pointer is passed when the ISO OUT pipe initialized.
 *
 * @param handle          The device handle. It equals the value returned from USB_DeviceInit.
 * @param message         The result of the ISO OUT pipe transfer.
 * @param callbackParam  The parameter for this callback. It is same with
 * usb_device_endpoint_callback_struct_t::callbackParam. In the class, the value is the audio class handle.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceAudioIsochronousOut(usb_device_handle handle,
                                           usb_device_endpoint_callback_message_struct_t *message,
                                           void *callbackParam)
{
    usb_device_audio_struct_t *audioHandle;
    usb_status_t status = kStatus_USB_Error;

    /* Get the audio class handle */
    audioHandle = (usb_device_audio_struct_t *)callbackParam;
    if (NULL == audioHandle)
    {
        return kStatus_USB_InvalidHandle;
    }
    audioHandle->streamOutPipeBusy = 0U;

    if ((NULL != audioHandle->configStruct) && (NULL != audioHandle->configStruct->classCallback))
    {
        /* classCallback is initialized in classInit of s_UsbDeviceClassInterfaceMap,
        it is from the second parameter of classInit */
        status = audioHandle->configStruct->classCallback((class_handle_t)audioHandle,
                                                          kUSB_DeviceAudioEventStreamRecvResponse, message);
    }

#if (AUDIO_BRIDGE_ENABLE)
    /* Bridge: push OUT (host->device) data into bridge buffer and re-arm receive */
    if ((message != NULL) && (message->buffer != NULL) && (message->length > 0U))
    {
        /* Push the data to the bridge (application will consume and forward to SAI) */
        audio_bridge_push_usb_out((const uint8_t *)message->buffer, (size_t)message->length);
    }

    /* Re-arm the OUT endpoint to receive the next USB packet.
       Use the same buffer pointer as provided by the host (message->buffer). */
    {
        uint8_t ep = (uint8_t)(USB_AUDIO_SPEAKER_STREAM_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));
        uint32_t pktSize = (uint32_t)USB_AUDIO_IN_PACKET_BUFFER_SIZE;
#if defined(FS_ISO_OUT_ENDP_PACKET_SIZE)
        pktSize = (uint32_t)FS_ISO_OUT_ENDP_PACKET_SIZE;
#endif
        /* Re-arm receive. If application provided class-level handling, we still re-arm here.
           Ignore return value to avoid breaking callback flow. */
        (void)USB_DeviceAudioRecv((class_handle_t)audioHandle, ep, (uint8_t *)message->buffer, pktSize);
    }
#endif

    return status;
}

/*!
 * @brief Initialize the stream endpoints of the audio class.
 *
 * This callback function is used to initialize the stream endpoints of the audio class.
 *
 * @param audioHandle          The device audio class handle. It equals the value returned from
 * usb_device_class_config_struct_t::classHandle.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceAudioStreamEndpointsInit(usb_device_audio_struct_t *audioHandle)
{
    usb_device_interface_list_t *interfaceList;
    usb_device_interface_struct_t *interface = (usb_device_interface_struct_t *)NULL;
    usb_status_t status                      = kStatus_USB_Error;
    uint8_t count;
    uint8_t index;

    /* Check the configuration is valid or not. */
    if (0U == audioHandle->configuration)
    {
        return status;
    }

    /* Check the configuration is valid or not. */
    if (audioHandle->configuration > audioHandle->configStruct->classInfomation->configurations)
    {
        return status;
    }

    if (NULL == audioHandle->configStruct->classInfomation->interfaceList)
    {
        return status;
    }

    /* Get the interface list of the new configuration. */
    interfaceList = &audioHandle->configStruct->classInfomation->interfaceList[audioHandle->configuration - 1U];

    /* Find stream interface by using the alternate setting of the interface. */
    for (count = 0U; count < interfaceList->count; count++)
    {
        if ((USB_DEVICE_CONFIG_AUDIO_CLASS_CODE == interfaceList->interfaces[count].classCode) &&
            (USB_DEVICE_AUDIO_STREAM_SUBCLASS == interfaceList->interfaces[count].subclassCode))
        {
            for (index = 0; index < interfaceList->interfaces[count].count; index++)
            {
                if (interfaceList->interfaces[count].interface[index].alternateSetting == audioHandle->streamAlternate)
                {
                    interface = &interfaceList->interfaces[count].interface[index];
                    break;
                }
            }
            audioHandle->streamInterfaceNumber = interfaceList->interfaces[count].interfaceNumber;
            break;
        }
    }
    if (NULL == interface)
    {
        return status;
    }
    /* Keep new stream interface handle. */
    audioHandle->streamInterfaceHandle = interface;

    /* Initialize the endpoints of the new interface. */
    for (count = 0U; count < interface->endpointList.count; count++)
    {
        usb_device_endpoint_init_struct_t epInitStruct;
        usb_device_endpoint_callback_struct_t epCallback;
        epInitStruct.zlt             = 0U;
        epInitStruct.interval        = interface->endpointList.endpoint[count].interval;
        epInitStruct.endpointAddress = interface->endpointList.endpoint[count].endpointAddress;
        epInitStruct.maxPacketSize   = interface->endpointList.endpoint[count].maxPacketSize;
        epInitStruct.transferType    = interface->endpointList.endpoint[count].transferType;

        if ((USB_ENDPOINT_ISOCHRONOUS == (epInitStruct.transferType & USB_DESCRIPTOR_ENDPOINT_ATTRIBUTE_TYPE_MASK)) &&
            (USB_IN == ((epInitStruct.endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) >>
                        USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)))
        {
            epCallback.callbackFn    = USB_DeviceAudioIsochronousIn;
        }
        else if ((USB_ENDPOINT_ISOCHRONOUS ==
                  (epInitStruct.transferType & USB_DESCRIPTOR_ENDPOINT_ATTRIBUTE_TYPE_MASK)) &&
                 (USB_OUT == ((epInitStruct.endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) >>
                              USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)))
        {
            epCallback.callbackFn    = USB_DeviceAudioIsochronousOut;
        }
        else if ((USB_ENDPOINT_INTERRUPT ==
                  (epInitStruct.transferType & USB_DESCRIPTOR_ENDPOINT_ATTRIBUTE_TYPE_MASK)) &&
                 (USB_IN == ((epInitStruct.endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) >>
                             USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)))
        {
            epCallback.callbackFn    = USB_DeviceAudioInterruptIn;
        }
        else
        {
        }
        epCallback.callbackParam = audioHandle;

        status = USB_DeviceInitEndpoint(audioHandle->handle, &epInitStruct, &epCallback);
    }
    return status;
}

#endif /* USB_DEVICE_CONFIG_AUDIO */
