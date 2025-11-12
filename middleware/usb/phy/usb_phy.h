/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_PHY_H__
#define __USB_PHY_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief USB PHY configuration structure */
typedef struct _usb_phy_config_struct
{
    uint8_t D_CAL;     /*!< Decode to trim the nominal 17.78mA current source */
    uint8_t TXCAL45DP; /*!< Decode to trim the nominal 45-Ohm series termination resistance to the USB_DP output pin */
    uint8_t TXCAL45DM; /*!< Decode to trim the nominal 45-Ohm series termination resistance to the USB_DM output pin */
} usb_phy_config_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the USB PHY for device mode.
 *
 * This function initializes the USB PHY IP for device mode.
 *
 * @param controllerId USB controller ID; See the #usb_controller_index_t.
 * @param freq The external input clock frequency.
 * @param phyConfig Pointer to the PHY configuration structure.
 * @return kStatus_USB_Success or error.
 */
extern uint32_t USB_EhciPhyInit(uint8_t controllerId, uint32_t freq, usb_phy_config_struct_t *phyConfig);

/*!
 * @brief De-initialize the USB PHY for device mode.
 *
 * This function de-initializes the USB PHY IP for device mode.
 *
 * @param controllerId USB controller ID; See the #usb_controller_index_t.
 * @return kStatus_USB_Success or error.
 */
extern uint32_t USB_EhciPhyDeinit(uint8_t controllerId);

/*!
 * @brief Control PHY to enter/exit low power mode.
 *
 * @param controllerId USB controller ID; See the #usb_controller_index_t.
 * @param enable 1 - enter low power mode, 0 - exit low power mode.
 * @return kStatus_USB_Success or error.
 */
extern uint32_t USB_EhciLowPowerModeInit(uint8_t controllerId, uint8_t enable);

#if defined(__cplusplus)
}
#endif

#endif /* __USB_PHY_H__ */
