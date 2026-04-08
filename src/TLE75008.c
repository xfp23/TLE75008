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

static TLE75008_Status_t spi_transmit(TLE75008_Handle_t handle, uint8_t *data, size_t len)
{
    CHECK_PTR(handle);
    CHECK_PTR(handle->conf.spi_tx);

#if TLE75008_USE_CS
    handle->conf.Cs_func(0);
#endif

    if (handle->conf.spi_tx(data, len) != 0)
    {
#if TLE75008_USE_CS
        handle->conf.Cs_func(1);
#endif
        return TLE75008_SPI_ERR;
    }

    handle->conf.Cs_func(1);
    return TLE75008_OK;
}

static TLE75008_Status_t spi_recive(TLE75008_Handle_t handle, uint8_t *data, size_t len)
{
    CHECK_PTR(handle);
    CHECK_PTR(handle->conf.spi_rx);

#if TLE75008_USE_CS
    handle->conf.Cs_func(0);
#endif

    if (handle->conf.spi_rx(data, len) != 0)
    {
#if TLE75008_USE_CS
        handle->conf.Cs_func(1);
#endif
        return TLE75008_SPI_ERR;
    }

    handle->conf.Cs_func(1);
    return TLE75008_OK;
}

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

    // if((*handle)->conf.driver_mode == TLE75008_SPI_MODE)
    // {
    //     // spi控制模式
    // } else if((*handle)->conf.driver_mode == TLE75008_PWM_MODE)
    // {
    //     // 直接pwm驱动
    // }
    (*handle)->conf.Idle_func(1);

    return TLE75008_OK;
}

// TLE75008_Status_t TLE75008_EnteryActiveMode(TLE75008_Handle_t handle)
// {
//     CHECK_PTR(handle);

//     handle->conf.Idle_func(0);
//     handle->conf.In0_func(1);
//     handle->conf.In0_func(1);

//     return TLE75008_OK;
// }

void TLE75008_DeInit(TLE75008_Handle_t *handle)
{
    if (handle == NULL || *handle == NULL)
        return;

    free(*handle);
    *handle = NULL;
}

// TLE75008_Status_t TLE75008_SoftWareReset(TLE75008_Handle_t handle)
// {

// }

// TLE75008_Status_t TLE75008_SetMode(TLE75008_Handle_t handle,TLE75008_Mode_t mode)
// {
//     CHECK_PTR(handle);

//     switch (mode)
//     {
//     case TLE75008_MODE_SLEEP:
//     handle->conf.Idle_func(0);
//     handle->conf.In0_func(0);
//     handle->conf.In1_func(0);
//         break;
//     case TLE75008_MODE_LIMP_HOME:
//     handle->conf.Idle_func(0);
//     handle->conf.In0_func(1);
//     handle->conf.In1_func(0);
//         break;
//     case TLE75008_MODE_IDLE:
//     handle->conf.Idle_func(1);
//     handle->conf.In0_func(1);
//     handle->conf.In1_func(1);

//         break;
//     case TLE75008_MODE_ACTIVE:
//         /* code */
//         break;
//     default:
//         break;
//     }
// }

// TLE75008_Status_t TLE75008_OutputChx(TLE75008_Handle_t handle, uint8_t mask, uint8_t value)
// {
//     uint16_t tx_buf, rx_buf;
//     uint8_t current_out;

//     tx_buf = 0x4002;
//     handle->conf.spi_tx_rx((uint8_t *)&tx_buf, (uint8_t *)&rx_buf, 2, 2);

//     tx_buf = 0x4101;
//     handle->conf.spi_tx_rx((uint8_t *)&tx_buf, (uint8_t *)&rx_buf, 2, 2); // 拿回数据

//     current_out = (uint8_t)(rx_buf & 0xFF);

//     uint8_t next_out = (current_out & ~mask) | (value & mask);

//     // --- 第三帧：把改好的值写回去 ---
//     tx_buf = 0x8000 | next_out; // 80 是写 OUT 的基准命令
//     handle->conf.spi_tx_rx((uint8_t *)&tx_buf, (uint8_t *)&rx_buf, 2, 2);

//     return TLE75008_OK;
// }

// TLE75008_Status_t TLE75008_OutputChx(TLE75008_Handle_t handle, TLE75008_Channel_t Channel, TLE75008_Output_t value)
// {
//     CHECK_PTR(handle);

//     TLE75008_OUTn_t reg = {0};
//     uint16_t cmd = 0x4002;

//     // 发送命令
//     int ret = handle->conf.spi_tx_rx((uint8_t*)&cmd,NULL,2,0);
//     if(ret != 0)
//     {
//         return TLE75008_SPI_ERR;
//     }
//     cmd = 0x4101;
//     handle->conf.spi_tx_rx((uint8_t*)&cmd,(uint8_t*)&reg.bytes,2,2);
//     if(ret != 0)
//     {
//         return TLE75008_SPI_ERR;
//     }

//     if(value)
//     {
//         reg.bits.Out_Ch = reg.bits.Out_Ch | 1 << Channel;
//     }else {
//         reg.bits.Out_Ch = reg.bits.Out_Ch & 1 << Channel;
//     }

//     ret = handle->conf.spi_tx_rx((uint8_t*)&reg.bytes,NULL,2,0);

//         if(ret != 0)
//     {
//         return TLE75008_SPI_ERR;
//     }

//     return TLE75008_OK;
// }

TLE75008_Status_t TLE75008_OutputChx(TLE75008_Handle_t handle, TLE75008_Channel_t Channel, TLE75008_Output_t value)
{
    CHECK_PTR(handle);

    TLE75008_OUTn_t reg_tx = {0}; // 发送寄存器
    TLE75008_OUTn_t reg_rx = {0}; // 接收寄存器

    reg_tx.bytes = 0x4002;

    if (spi_transmit(handle, &reg_tx.bytes, 2) != 0)
    {
        return TLE75008_SPI_ERR;
    }

    // reg_tx.bytes = 0x4101; // 发个标准命令，理论上我发0也可以，但是数据手册说不以10开头的命令都是错误
    // if (handle->conf.spi_tx_rx((uint8_t*)&reg_tx.bytes, (uint8_t*)&reg_rx.bytes, 2, 2) != 0) {
    //     return TLE75008_SPI_ERR;
    // }
    if (spi_recive(handle, &reg_rx.bytes, 2) != 0)
        return TLE75008_SPI_ERR;

    reg_tx.bytes = 0x00;

    reg_tx.bits.Out_Ch = reg_rx.bits.Out_Ch;

    if (value == TLE75008_OUTPUT_ON)
    {
        reg_tx.bits.Out_Ch |= (1 << Channel);
    }
    else
    {
        reg_tx.bits.Out_Ch &= ~(1 << Channel);
    }

    reg_tx.bits.Cmd = 0x80;

    if (spi_transmit(handle,(uint8_t *)&reg_tx.bytes,2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_OutputChBatch(TLE75008_Handle_t handle, TLE75008_ChMask_t mask)
{
    CHECK_PTR(handle);

    TLE75008_OUTn_t reg_tx = {0}; // 发送寄存器
    reg_tx.bits.Cmd = 0x80;
    reg_tx.bits.Out_Ch = mask.mask;

    if (spi_transmit(handle, &reg_tx.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }
    return TLE75008_OK;
}

static TLE75008_Status_t TLE75008_SetMapIn0(TLE75008_Handle_t handle, TLE75008_ChMask_t mask)
{
    CHECK_PTR(handle);

    // TLE75008_MAPIN0_t reg_r = {.bytes = 0x4402};
    TLE75008_MAPIN0_t reg_w = {.bits.Cmd = 0x84};

    reg_w.bits.Out_Ch = mask.mask;

    if (spi_transmit(handle, &reg_w.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}

static TLE75008_Status_t TLE75008_SetMapIn1(TLE75008_Handle_t handle, TLE75008_ChMask_t mask)
{
    CHECK_PTR(handle);

    // TLE75008_MAPIN0_t reg_r = {.bytes = 0x4402};
    TLE75008_MAPIN1_t reg_w = {.bits.Cmd = 0x85};

    reg_w.bits.Out_Ch = mask.mask;

    if (spi_transmit(handle, &reg_w.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_SetMapInx(TLE75008_Handle_t handle, TLE75008_Input_t ch, TLE75008_ChMask_t mask)
{
    if (ch == TLE75008_INPUT_0)
    {
        return TLE75008_SetMapIn0(handle, mask);
    }
    else if (ch == TLE75008_INPUT_1)
    {
        return TLE75008_SetMapIn1(handle, mask);
    }

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_GetInstStatus(TLE75008_Handle_t handle, TLE75008_Inst_t *value)
{
    CHECK_PTR(handle);

    TLE75008_TER_INST_t reg = {.bytes = 0x4602};

    if (spi_transmit(handle, &reg.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    reg.bytes = 0x00;

    if (spi_recive(handle, &reg.bytes, 2) != TLE75008_OK)
    {
        TLE75008_SPI_ERR;
    }

    value->In0_value = reg.bits.INST_IN0;
    value->In1_value = reg.bits.INST_IN1;
    value->trans_err = reg.bits.TER;

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_EnChxFaultDiag(TLE75008_Handle_t handle, TLE75008_ChMask_t mask)
{
    CHECK_PTR(handle);

    TLE75008_DiagIol_t reg = {.bits.Cmd = 0x88};

    reg.bits.DIAG_IOL_OUTn = mask.mask;

    if (spi_transmit(handle, &reg.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}

/**
 * @brief 故障检测
 *
 * @param handle
 * @param mask
 * @return TLE75008_Status_t
 */
TLE75008_Status_t TLE75008_ChxFaultDete(TLE75008_Handle_t handle, TLE75008_ChMask_t *mask)
{
    CHECK_PTR(handle);

    TLE75008_DiagOsm_t reg = {.bytes = 0x4902};

    if (spi_transmit(handle, &reg.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    reg.bytes = 0x00;

    if (spi_recive(handle, &reg.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    mask->mask = reg.bits.DIAG_OSM_OUTn;

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_SoftWareReset(TLE75008_Handle_t handle)
{
    CHECK_PTR(handle);

    TLE75008_HWCR_t reg = {.bits.Cmd = 0x8C};

    reg.bits.HWCR_RST = 0x01;

    if (spi_transmit(handle, &reg.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_SetParallProtect(TLE75008_Handle_t handle, TLE75008_Parallel_t mask)
{
    CHECK_PTR(handle);

    TLE75008_HWCR_t reg = {.bits.Cmd = 0x8C};

    reg.bits.HWCR_PAR = mask.bytes;
    reg.bits.HWCR_ACT = 1;
    reg.bits.HWCR_RST = 0;

    if (spi_transmit(handle, &reg.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_ClearFaultChx(TLE75008_Handle_t handle, TLE75008_ChMask_t mask)
{
    TLE75008_HWCR_OCL_t reg = {.bits.Cmd = 0x8D};

    reg.bits.HWCR_OCL = mask.mask;

    if (spi_transmit(handle, &reg.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_EnterLimpHome(TLE75008_Handle_t handle)
{

    CHECK_PTR(handle);

    handle->conf.Idle_func(0);
    handle->conf.In0_func(1);
    handle->conf.In1_func(0);

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_EnterIdle(TLE75008_Handle_t handle)
{

    CHECK_PTR(handle);

    handle->conf.Idle_func(1);
    handle->conf.In0_func(0);
    handle->conf.In1_func(0);

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_EnterSleep(TLE75008_Handle_t handle)
{
    CHECK_PTR(handle);

    handle->conf.Idle_func(0);
    handle->conf.In0_func(0);
    handle->conf.In1_func(0);

    return TLE75008_OK;
}

TLE75008_Status_t TLE75008_ActiveChip(TLE75008_Handle_t handle, TLE75008_ChipActive_t active)
{
    CHECK_PTR(handle);

    TLE75008_HWCR_t reg_r = {.bytes = 0x4C02};
    TLE75008_HWCR_t reg_w = {.bits.Cmd = 0x8C};

    handle->conf.Idle_func(1);
    handle->conf.In0_func(0);
    handle->conf.In1_func(0);

    if (spi_transmit(handle, &reg_r.bytes, 2) != TLE75008_OK)
    {
        TLE75008_SPI_ERR;
    }

    reg_r.bytes = 0x00;

    if (spi_recive(handle, &reg_r.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    reg_w.bits.HWCR_ACT = active;
    reg_w.bits.HWCR_RST = 0;
    reg_w.bits.HWCR_PAR = reg_r.bits.HWCR_PAR;

    if (spi_transmit(handle, &reg_w.bytes, 2) != TLE75008_OK)
    {
        return TLE75008_SPI_ERR;
    }

    return TLE75008_OK;
}
