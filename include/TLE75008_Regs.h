/**
 * @file TLE75008_Regs.h
 * @author https://github.com/xfp23
 * @brief 寄存器定义
 * @version 0.1
 * @date 2026-04-07
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef TLE75008_REGS_H__
#define TLE75008_REGS_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef union
{
    struct __attribute__((packed))
    {
        uint8_t ERRn : 8;  // 0 : 没有故障 1 ： 对应通道故障 (只读)
        uint8_t OLOFF : 1; // 是否有通道开路 (OR)
        uint8_t : 1;
        uint8_t TER : 1;     // Transmission Error 上一次是否传输成功  默认 ： 1 ； 传输成功置0
        uint8_t MODE : 2;    // 00 ： 无 | 0b01 ： Limp Home Mode | 0b10 : Active Mode | 0b11 默认：IDLE Mode
        uint8_t LOP_VDD : 1; // V_DD 低电压工作范围监控  0b00 : VDD > VDDLOP 0b01 : 自上次读取依赖，至少发生过一次 VDD <= VDD(LOP)
        uint8_t UVRVS : 1;   // 欠压监控 0b00 无欠压 0b01有欠压记录
        uint8_t : 1;
    } bits;

    uint16_t byte; // default value 0x7800
} TLE75008_StandardDiagReg_t;

// // typedef union
// // {
// //     struct __attribute__((packed))
// //     {
// //         /* [Bit 7:0] 数据域：根据地址不同，含义如下：*/
// //         uint16_t DATA : 8;
// //         /**
// //          * OUT      (0000 00): 输出控制 (1:开启, 0:关闭)。默认 0x00。
// //          * MAPIN0   (0001 00): 关联 IN0 引脚 (1:联动, 0:不联动)。注: Ch2 默认 1。
// //          * MAPIN1   (0001 01): 关联 IN1 引脚 (1:联动, 0:不联动)。注: Ch3 默认 1。
// //          * INST     (0001 10): [只读] 输入状态。bit7:TER(传输错误), bit1:IN1状态, bit0:IN0状态。
// //          * DIAG_IOL (0010 00): 离线诊断电流 (1:开启, 0:关闭)。用于检测开路。
// //          * DIAG_OSM (0010 01): [只读] 输出状态监视。1:检测到开路(VDS<VDS_OL), 0:正常。
// //          * HWCR     (0011 00): 硬件配置。bit7:ACT(激活), bit6:RST(复位), bit3-0:PAR(并联)。
// //          * HWCR_OCL (0011 01): [只写] 错误锁存清除。写 1 清除对应通道的故障锁定。
// //          */
// 
// //         uint16_t ADDR1 : 2; // [Bit 9:8]   子地址 (见 Table 16)
// //         uint16_t ADDR0 : 4; // [Bit 13:10] 主地址 (见 Table 16)
// //         uint16_t RW : 2;    // [Bit 15:14] 指令位: 0b01=读 | 0b10=写
// //     } bits;
// 
// //     uint16_t bytes;
// // } TLE75008_StructureReg_t;

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t Out_Ch : 8; // (0000 00): 输出控制 (1:开启, 0:关闭)。默认 0x00。
        uint16_t Cmd : 8;    // R： 0x4002 W : 0x80XX
    } bits;

    uint16_t bytes;
} TLE75008_OUTn_t;

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t Out_Ch : 8; // MAPIN0   (0001 00): 关联 IN0 引脚 (1:联动, 0:不联动)。注: Ch2 默认 1。
        uint16_t Cmd : 8;    // R ： 0x4402 W : 0x84XX
    } bits;

    uint16_t bytes;
} TLE75008_MAPIN0_t;

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t Out_Ch : 8; // 关联 IN1 引脚 (1:联动, 0:不联动)。注: Ch3 默认 1。
        uint16_t Cmd : 8;    // R ： 0x4502 W : 0x85XX
    } bits;

    uint16_t bytes;
} TLE75008_MAPIN1_t;

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t INST_IN0 : 1; // [只读] 输入状态。 bit0:IN0状态。
        uint16_t INST_IN1 : 1; // bit1:IN1状态,
        uint16_t : 5;
        uint16_t TER : 1; // bit7:TER(传输错误),
        uint16_t Cmd : 8; // OR: 0x4602
    } bits;

    uint16_t bytes;
} TLE75008_TER_INST_t;

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t DIAG_IOL_OUTn : 8; // 离线诊断电流 (1:开启, 0:关闭)。用于检测开路。
        uint16_t Cmd : 8;           // R : 0x4802 W : 0x88xx
    } bits;

    uint16_t bytes;
} TLE75008_DiagIol_t;

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t DIAG_OSM_OUTn : 8; // 输出状态监视。1:检测到开路(VDS<VDS_OL), 0:正常。
        uint16_t Cmd : 8;           // OR : 0x4902
    } bits;

    uint16_t bytes;

} TLE75008_DiagOsm_t;

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t HWCR_PAR : 4; // 硬件配置。, bit3-0:PAR(并联)。
        uint16_t : 2;
        uint16_t HWCR_RST : 1; // bit6:RST(复位),
        uint16_t HWCR_ACT : 1; // bit7:ACT(激活)
        uint16_t Cmd : 8;      // R : 0x4C02 W : 0x8Cxx
    } bits;

    uint16_t bytes;
} TLE75008_HWCR_t;

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t HWCR_OCL : 8; // 错误锁存清除。写 1 清除对应通道的故障锁定。
        uint16_t Cmd : 8;      // R : 0x4D02 W : 0x8Dxx
    } bits;

    uint16_t bytes;
} TLE75008_HWCR_OCL_t;

#ifdef __cplusplus
}
#endif

#endif
