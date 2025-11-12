/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_SPEC_H__
#define __USB_SPEC_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Compiler-specific structure packing macros */
#if defined(__ICCARM__)
#define STRUCT_PACKED __packed
#define STRUCT_UNPACKED
#elif defined(__GNUC__)
#define STRUCT_PACKED
#define STRUCT_UNPACKED __attribute__((__packed__))
#elif defined(__CC_ARM)
#define STRUCT_PACKED __packed
#define STRUCT_UNPACKED
#else
#define STRUCT_PACKED
#define STRUCT_UNPACKED
#endif

/*! @brief USB Global variable macro */
#ifndef USB_GLOBAL
#define USB_GLOBAL
#endif

/*! @brief USB RAM address alignment macro */
#ifndef USB_RAM_ADDRESS_ALIGNMENT
#define USB_RAM_ADDRESS_ALIGNMENT(n) __attribute__((aligned(n)))
#endif

/*! @brief USB DMA alignment size */
#define USB_DATA_ALIGN_SIZE (4U)
#define USB_DATA_ALIGN_SIZE_MULTIPLE(n) (((n) + USB_DATA_ALIGN_SIZE - 1U) & (~(USB_DATA_ALIGN_SIZE - 1U)))

/*! @brief Byte order conversion macros */
#define USB_SHORT_FROM_LITTLE_ENDIAN(n) ((uint16_t)(n))
#define USB_LONG_FROM_LITTLE_ENDIAN(n)  ((uint32_t)(n))
#define USB_SHORT_TO_LITTLE_ENDIAN(n)   ((uint16_t)(n))
#define USB_LONG_TO_LITTLE_ENDIAN(n)    ((uint32_t)(n))

#define USB_SHORT_FROM_BIG_ENDIAN(n) \
    ((uint16_t)(((((uint16_t)(n)) & 0x00FFU) << 8U) | ((((uint16_t)(n)) & 0xFF00U) >> 8U)))
#define USB_LONG_FROM_BIG_ENDIAN(n)                                    \
    ((uint32_t)(((((uint32_t)(n)) & 0x000000FFU) << 24U) |             \
                ((((uint32_t)(n)) & 0x0000FF00U) << 8U) |              \
                ((((uint32_t)(n)) & 0x00FF0000U) >> 8U) |              \
                ((((uint32_t)(n)) & 0xFF000000U) >> 24U)))
#define USB_SHORT_TO_BIG_ENDIAN(n) USB_SHORT_FROM_BIG_ENDIAN(n)
#define USB_LONG_TO_BIG_ENDIAN(n) USB_LONG_FROM_BIG_ENDIAN(n)

/*! @brief USB speed */
#define USB_SPEED_FULL (0x00U)
#define USB_SPEED_LOW (0x01U)
#define USB_SPEED_HIGH (0x02U)
#define USB_SPEED_SUPER (0x03U)

/*! @brief USB direction */
#define USB_IN (0x01U)
#define USB_OUT (0x00U)

/*! @brief Control request type */
#define USB_REQUEST_TYPE_DIR_MASK (0x80U)
#define USB_REQUEST_TYPE_DIR_SHIFT (7U)
#define USB_REQUEST_TYPE_DIR_OUT (0x00U)
#define USB_REQUEST_TYPE_DIR_IN (0x80U)

#define USB_REQUEST_TYPE_TYPE_MASK (0x60U)
#define USB_REQUEST_TYPE_TYPE_SHIFT (5U)
#define USB_REQUEST_TYPE_TYPE_STANDARD (0x00U)
#define USB_REQUEST_TYPE_TYPE_CLASS (0x20U)
#define USB_REQUEST_TYPE_TYPE_VENDOR (0x40U)

#define USB_REQUEST_TYPE_RECIPIENT_MASK (0x1FU)
#define USB_REQUEST_TYPE_RECIPIENT_SHIFT (0U)
#define USB_REQUEST_TYPE_RECIPIENT_DEVICE (0x00U)
#define USB_REQUEST_TYPE_RECIPIENT_INTERFACE (0x01U)
#define USB_REQUEST_TYPE_RECIPIENT_ENDPOINT (0x02U)
#define USB_REQUEST_TYPE_RECIPIENT_OTHER (0x03U)

/*! @brief Standard request */
#define USB_REQUEST_STANDARD_GET_STATUS (0x00U)
#define USB_REQUEST_STANDARD_CLEAR_FEATURE (0x01U)
#define USB_REQUEST_STANDARD_SET_FEATURE (0x03U)
#define USB_REQUEST_STANDARD_SET_ADDRESS (0x05U)
#define USB_REQUEST_STANDARD_GET_DESCRIPTOR (0x06U)
#define USB_REQUEST_STANDARD_SET_DESCRIPTOR (0x07U)
#define USB_REQUEST_STANDARD_GET_CONFIGURATION (0x08U)
#define USB_REQUEST_STANDARD_SET_CONFIGURATION (0x09U)
#define USB_REQUEST_STANDARD_GET_INTERFACE (0x0AU)
#define USB_REQUEST_STANDARD_SET_INTERFACE (0x0BU)
#define USB_REQUEST_STANDARD_SYNCH_FRAME (0x0CU)

/*! @brief Descriptor type */
#define USB_DESCRIPTOR_TYPE_DEVICE (0x01U)
#define USB_DESCRIPTOR_TYPE_CONFIGURE (0x02U)
#define USB_DESCRIPTOR_TYPE_STRING (0x03U)
#define USB_DESCRIPTOR_TYPE_INTERFACE (0x04U)
#define USB_DESCRIPTOR_TYPE_ENDPOINT (0x05U)
#define USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER (0x06U)
#define USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION (0x07U)
#define USB_DESCRIPTOR_TYPE_INTERFASE_POWER (0x08U)
#define USB_DESCRIPTOR_TYPE_OTG (0x09U)
#define USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION (0x0BU)
#define USB_DESCRIPTOR_TYPE_BOS (0x0FU)
#define USB_DESCRIPTOR_TYPE_DEVICE_CAPABILITY (0x10U)

/*! @brief Descriptor length */
#define USB_DESCRIPTOR_LENGTH_DEVICE (0x12U)
#define USB_DESCRIPTOR_LENGTH_CONFIGURE (0x09U)
#define USB_DESCRIPTOR_LENGTH_INTERFACE (0x09U)
#define USB_DESCRIPTOR_LENGTH_ENDPOINT (0x07U)
#define USB_DESCRIPTOR_LENGTH_DEVICE_QUALIFIER (0x0AU)
#define USB_DESCRIPTOR_LENGTH_OTG (0x05U)
#define USB_DESCRIPTOR_LENGTH_INTERFACE_ASSOCIATION (0x08U)

/*! @brief Endpoint address and direction */
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK (0x80U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT (7U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT (0U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN (0x80U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_NUMBER_MASK (0x0FU)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_NUMBER_SHFIT (0U)

/*! @brief Endpoint type */
#define USB_ENDPOINT_CONTROL (0x00U)
#define USB_ENDPOINT_ISOCHRONOUS (0x01U)
#define USB_ENDPOINT_BULK (0x02U)
#define USB_ENDPOINT_INTERRUPT (0x03U)

/*! @brief Configuration attributes */
#define USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_D7_MASK (0x80U)
#define USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_D7_SHIFT (7U)

#define USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_SELF_POWERED_MASK (0x40U)
#define USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_SELF_POWERED_SHIFT (6U)

#define USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_REMOTE_WAKEUP_MASK (0x20U)
#define USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_REMOTE_WAKEUP_SHIFT (5U)

/*! @brief Control max packet size */
#define USB_CONTROL_MAX_PACKET_SIZE (64U)

/*! @brief Macro for data align */
#ifndef USB_DATA_ALIGN_SIZE_MULTIPLE
#define USB_DATA_ALIGN_SIZE_MULTIPLE (4U)
#endif
#ifndef USB_DATA_ALIGN_SIZE
#define USB_DATA_ALIGN_SIZE (4U)
#endif

/*! @brief USB DMA align */
#define USB_DMA_ALIGN_MASK (USB_DATA_ALIGN_SIZE - 1U)

#define USB_DMA_ALIGN(n) (((n) + USB_DMA_ALIGN_MASK) & (~USB_DMA_ALIGN_MASK))

/*! @brief USB DMA align memory */
#if (defined(USB_DATA_ALIGN_SIZE) && (USB_DATA_ALIGN_SIZE > 0U))
#if defined(__ICCARM__)
#define USB_DMA_INIT_DATA_ALIGN(n) _Pragma(#n)
#define USB_DMA_NONINIT_DATA_ALIGN(n) _Pragma(#n)
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define USB_DMA_INIT_DATA_ALIGN(n) __attribute__((aligned(n)))
#define USB_DMA_NONINIT_DATA_ALIGN(n) __attribute__((aligned(n))) __attribute__((zero_init))
#elif defined(__GNUC__)
#define USB_DMA_INIT_DATA_ALIGN(n) __attribute__((aligned(n)))
#define USB_DMA_NONINIT_DATA_ALIGN(n) __attribute__((aligned(n))) __attribute__((section(".noinit")))
#else
#error Toolchain not supported!
#endif
#else
#define USB_DMA_INIT_DATA_ALIGN(n)
#define USB_DMA_NONINIT_DATA_ALIGN(n)
#endif

/*! @brief USB short transfer */
#define USB_SHORT_GET_LOW(x) (((uint16_t)x) & 0xFFU)
#define USB_SHORT_GET_HIGH(x) ((uint8_t)(((uint16_t)x) >> 8U) & 0xFFU)

#define USB_LONG_GET_BYTE0(x) ((uint8_t)(((uint32_t)(x))) & 0xFFU)
#define USB_LONG_GET_BYTE1(x) ((uint8_t)(((uint32_t)(x)) >> 8U) & 0xFFU)
#define USB_LONG_GET_BYTE2(x) ((uint8_t)(((uint32_t)(x)) >> 16U) & 0xFFU)
#define USB_LONG_GET_BYTE3(x) ((uint8_t)(((uint32_t)(x)) >> 24U) & 0xFFU)

#define USB_MEM4_ALIGN_MASK (0x03U)

#endif /* __USB_SPEC_H__ */
