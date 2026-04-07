/**
 * @file TLE75008.c
 * @author https://github.com/xfp23
 * @brief 英飞凌 TLE75008 电机驱动源码实现
 * @version 0.1
 * @date 2026-04-07
 *
 * @copyright Copyright (c) 2026
 *
 */
#include "TLE75008.h"
#include "TLE75008_Regs.h"
#include "string.h"
#include "stdlib.h"

#define CHECK_PTR(x)             \
    do                           \
    {                            \
        if (x == NULL)           \
        {                        \
            return TLE75008_ERR; \
        }                        \
    } while (0)

// static TLE75008_Status_t spi_transmit(TLE75008_Handle_t handle, uint8_t *data, size_t len)
// {
//     CHECK_PTR(handle);
//     // CHECK_PTR(handle->conf.spi_tx);

// #if TLE75008_USE_CS
//     handle->conf.Cs_func(0);
// #endif

//     if (handle->conf.spi_tx_rx(,, len) != 0)
//     {
// #if TLE75008_USE_CS
//         handle->conf.Cs_func(1);
// #endif
//         return TLE75008_SPI_ERR;
//     }

//     handle->conf.Cs_func(1);
//     return TLE75008_OK;
// }

// static TLE75008_Status_t spi_recive(TLE75008_Handle_t handle, uint8_t *data, size_t len)
// {
//     CHECK_PTR(handle);
//     CHECK_PTR(handle->conf.spi_rx);

// #if TLE75008_USE_CS
//     handle->conf.Cs_func(0);
// #endif

//     if (handle->conf.spi_rx(data, len) != 0)
//     {
// #if TLE75008_USE_CS
//         handle->conf.Cs_func(1);
// #endif
//         return TLE75008_SPI_ERR;
//     }

//     handle->conf.Cs_func(1);
//     return TLE75008_OK;
// }

TLE75008_Status_t TLE75008_Init(TLE75008_Handle_t *handle, TLE75008_Conf_t *conf)
{
    if (*handle != NULL || conf == NULL || handle == NULL)
    {
        return TLE75008_ERR;
    }

    (*handle) = calloc(1, sizeof(TLE75008_Obj_t));

    if (*handle == NULL)
    {
        return TLE75008_MEMORY_ERR; // 内存分配失败
    }

    (*handle)->conf = (*conf);


    if((*handle)->conf.driver_mode == TLE75008_SPI_MODE)
    {
        // spi控制模式
    } else if((*handle)->conf.driver_mode == TLE75008_PWM_MODE)
    {
        // 直接pwm驱动
    }

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_HardWareSleep(TLE75008_Handle_t handle)
{
    CHECK_PTR(handle);

    handle->conf.Idle_func(0);
    handle->conf.In0_func(0);
    handle->conf.In1_func(0);

    return TLE75008_OK;
}

void TLE75008_DeInit(TLE75008_Handle_t *handle)
{
    if (handle == NULL || *handle == NULL)
        return;

    free(*handle);
    *handle = NULL;
}

TLE75008_Status_t TLE75008_SoftWareReset(TLE75008_Handle_t handle)
{

}

TLE75008_Status_t TLE75008_SetMode(TLE75008_Handle_t handle,TLE75008_Mode_t mode)
{
    CHECK_PTR(handle);

    switch (mode)
    {
    case TLE75008_MODE_SLEEP:
    handle->conf.Idle_func(0);
    handle->conf.In0_func(0);
    handle->conf.In1_func(0);
        break;
    case TLE75008_MODE_LIMP_HOME:
    handle->conf.Idle_func(0);
    handle->conf.In0_func(1);
    handle->conf.In1_func(0);
        break;
    case TLE75008_MODE_IDLE:
    handle->conf.Idle_func(1);
    handle->conf.In0_func(1);
    handle->conf.In1_func(1);

        break;
    case TLE75008_MODE_ACTIVE:
        /* code */
        break;
    default:
        break;
    }
}


TLE75008_Status_t TLE75008_OutputChx(TLE75008_Handle_t handle, uint8_t mask, uint8_t value) {
    uint16_t tx_buf, rx_buf;
    uint8_t current_out;

    tx_buf = 0x4002; 
    handle->conf.spi_tx_rx((uint8_t*)&tx_buf, (uint8_t*)&rx_buf, 2,2);

    tx_buf = 0x4101;
    handle->conf.spi_tx_rx((uint8_t*)&tx_buf, (uint8_t*)&rx_buf, 2,2); // 拿回数据

    current_out = (uint8_t)(rx_buf & 0xFF);

    uint8_t next_out = (current_out & ~mask) | (value & mask);

    // --- 第三帧：把改好的值写回去 ---
    tx_buf = 0x8000 | next_out; // 80 是写 OUT 的基准命令
    handle->conf.spi_tx_rx((uint8_t*)&tx_buf, (uint8_t*)&rx_buf, 2,2);

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_OutputChx(TLE75008_Handle_t handle, TLE75008_Channel_t Channel, TLE75008_Output_t value)
{
    CHECK_PTR(handle);

    TLE75008_OUTn_t reg = {0};
    uint16_t cmd = 0x4002;

    // 发送命令
    int ret = handle->conf.spi_tx_rx((uint8_t*)&cmd,NULL,2,0);
    if(ret != 0)
    {
        return TLE75008_SPI_ERR;
    }
    cmd = 0x4101;
    handle->conf.spi_tx_rx((uint8_t*)&cmd,(uint8_t*)&reg.bytes,2,2);
    if(ret != 0)
    {
        return TLE75008_SPI_ERR;
    }

    if(value)
    {
        reg.bits.Out_Ch = reg.bits.Out_Ch | 1 << Channel;
    }else {
        reg.bits.Out_Ch = reg.bits.Out_Ch & 1 << Channel;
    }

    ret = handle->conf.spi_tx_rx((uint8_t*)&reg.bytes,NULL,2,0);

        if(ret != 0)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}