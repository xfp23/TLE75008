/**
 * @file TLE75008_types.h
 * @author https://github.com/xfp23
 * @brief 
 * @version 0.1
 * @date 2026-04-07
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef TLE75008_TYPES_H__
#define TLE75008_TYPES_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 是否使用独立片选
 * 
 */
#define TLE75008_USE_CS (1)


typedef enum 
{
    TLE75008_OK,
    TLE75008_ERR,
    TLE75008_SPI_ERR,
    TLE75008_MEMORY_ERR,
}TLE75008_Status_t;

typedef enum
{
    TLE75008_SPI_MODE, // spi控制
    TLE75008_PWM_MODE, // 通过IN0 IN1 直接驱动通道2和3输出pwm
}TLE75008_DriverMode_t;

typedef enum
{
    TLE75008_MODE_SLEEP,
    TLE75008_MODE_LIMP_HOME,
    TLE75008_MODE_IDLE,
    TLE75008_MODE_ACTIVE,
}TLE75008_Mode_t;

typedef enum {
    TLE75008_CHANNEL_0,
    TLE75008_CHANNEL_1,
    TLE75008_CHANNEL_2,
    TLE75008_CHANNEL_3,
    TLE75008_CHANNEL_4,
    TLE75008_CHANNEL_5,
    TLE75008_CHANNEL_6,
    TLE75008_CHANNEL_7,
}TLE75008_Channel_t;

typedef enum {
    TLE75008_OUTPUT_DISABLE,
    TLE75008_OUTPUT_ENABLE,
}TLE75008_Output_t;



typedef  void(*TLE75008_GPIO_Func_t)(uint8_t param);

typedef int(*TLE75008_Spi_Func_t)(uint8_t *txbuf,uint8_t *rxbuf,size_t tx_len,size_t rx_len);


typedef union 
{
    struct __attribute__((packed))
    {
        uint8_t CH0_1 :1;
        uint8_t CH2_3 :1;
        uint8_t CH4_5 :1;
        uint8_t CH6_7 :1;
        uint8_t :4;
    }bits;

    uint8_t bytes;
}TLE75008_Parallel_t; // 并联保护

typedef struct /* __attribute__((packed)) */{

    TLE75008_GPIO_Func_t Idle_func; // idle引脚
#if TLE75008_USE_CS
    TLE75008_GPIO_Func_t Cs_func; // 片选
#endif
    TLE75008_GPIO_Func_t In0_func; // in0 gpio操作，如果选择了pwm模式，请在此函数填写占空比的更新函数
    TLE75008_GPIO_Func_t In1_func; // in1 gpio操作，如果选择了pwm模式，请在此函数填写占空比的更新函数
    // TLE75008_Spi_Func_t spi_tx;
    // TLE75008_Spi_Func_t spi_rx;
    TLE75008_Spi_Func_t spi_tx_rx;
    TLE75008_DriverMode_t driver_mode; // 驱动模式，选择pwm还是spi
}TLE75008_Conf_t;  

typedef struct 
{
    TLE75008_Conf_t conf;
}TLE75008_Obj_t;

typedef TLE75008_Obj_t *TLE75008_Handle_t; // 句柄

#ifdef __cplusplus
}
#endif
 

#endif
