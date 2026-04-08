/**
 * @file    TLE75008.h
 * @author  https://github.com/xf23
 * @brief   Driver for TLE75008 high-side switch
 *
 * @note
 * SPI Mode: CPOL = 0, CPHA = 1
 * (Idle low, sample on second edge)
 * Max SPI Frequency: 5 MHz
 *
 * @version 0.2
 * @date    2026-04-07
 *
 * @copyright Copyright (c) 2026
 */

#ifndef TLE75008_H__
#define TLE75008_H__

#include "TLE75008_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize device instance
 *
 * @param handle Pointer to device handle
 * @param conf   Hardware configuration
 * @return Status code
 */
TLE75008_Status_t TLE75008_Init(TLE75008_Handle_t *handle, TLE75008_Conf_t *conf);

/**
 * @brief Deinitialize device
 *
 * @param handle Device handle
 */
void TLE75008_DeInit(TLE75008_Handle_t *handle);

/**
 * @brief Set single channel output state
 *
 * @param handle  Device handle
 * @param channel Channel index
 * @param value   ON / OFF
 * @return Status code
 */
TLE75008_Status_t TLE75008_OutputChx(TLE75008_Handle_t handle,TLE75008_Channel_t channel,TLE75008_Output_t value);

/**
 * @brief Set multiple channel outputs using mask
 *
 * @param handle Device handle
 * @param mask   Channel mask
 * @return Status code
 */
TLE75008_Status_t TLE75008_OutputChBatch(TLE75008_Handle_t handle,TLE75008_ChMask_t mask);

/**
 * @brief Map input pin (INx) to output channels
 *
 * @param handle Device handle
 * @param input  Input pin (IN0 / IN1)
 * @param mask   Channel mask (1 = connected)
 * @return Status code
 */
TLE75008_Status_t TLE75008_SetMapInx(TLE75008_Handle_t handle,TLE75008_Input_t input,TLE75008_ChMask_t mask);

/**
 * @brief Get instant device status
 *
 * Includes:
 *  - IN0 / IN1 level
 *  - Last SPI transmission error (TER)
 *
 * @param handle Device handle
 * @param value  Output status struct
 * @return Status code
 */
TLE75008_Status_t TLE75008_GetInstStatus(TLE75008_Handle_t handle,TLE75008_Inst_t *value);

/**
 * @brief Enable fault diagnosis for channels
 *
 * (Open-load / Short detection)
 *
 * @param handle Device handle
 * @param mask   Channel mask
 * @return Status code
 */
TLE75008_Status_t TLE75008_EnChxFaultDiag(TLE75008_Handle_t handle,TLE75008_ChMask_t mask);

/**
 * @brief Read fault detection result
 *
 * @param handle Device handle
 * @param mask   Output fault mask
 * @return Status code
 */
TLE75008_Status_t TLE75008_ChxFaultDete(TLE75008_Handle_t handle,TLE75008_ChMask_t *mask);

/**
 * @brief Perform software reset
 *
 * @param handle Device handle
 * @return Status code
 */
TLE75008_Status_t TLE75008_SoftWareReset(TLE75008_Handle_t handle);

/**
 * @brief Configure parallel protection
 *
 * @param handle Device handle
 * @param mask   Parallel protection configuration
 * @return Status code
 */
TLE75008_Status_t TLE75008_SetParallProtect(TLE75008_Handle_t handle,TLE75008_Parallel_t mask);

/**
 * @brief Clear fault flags of selected channels
 *
 * @param handle Device handle
 * @param mask   Channel mask
 * @return Status code
 */
TLE75008_Status_t TLE75008_ClearFaultChx(TLE75008_Handle_t handle,TLE75008_ChMask_t mask);

/**
 * @brief Force device into Limp Home mode
 *
 * @warning
 * - SPI becomes read-only
 * - Only hardware inputs control outputs
 *
 * @param handle Device handle
 * @return Status code
 */
TLE75008_Status_t TLE75008_EnterLimpHome(TLE75008_Handle_t handle);

/**
 * @brief Enter Idle mode (low power)
 *
 * @param handle Device handle
 * @return Status code
 */
TLE75008_Status_t TLE75008_EnterIdle(TLE75008_Handle_t handle);

/**
 * @brief Enter Sleep mode (lowest power)
 *
 * @param handle Device handle
 * @return Status code
 */
TLE75008_Status_t TLE75008_EnterSleep(TLE75008_Handle_t handle);

/**
 * @brief Enable or disable Active mode
 *
 * @param handle Device handle
 * @param active Enable / Disable
 * @return Status code
 */
TLE75008_Status_t TLE75008_ActiveChip(TLE75008_Handle_t handle,TLE75008_ChipActive_t active);

#ifdef __cplusplus
}
#endif

#endif