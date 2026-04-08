
[En](./readme.md)

# TLE75008 驱动库说明文档

本库为 Infineon TLE75008 8通道高边开关芯片的通用 C 语言驱动程序。支持通过 SPI 进行诊断、配置和控制。

## 硬件规格
- **SPI 模式**: CPOL = 0, CPHA = 1 (空闲低电平，第二个边沿采样)
- **最大 SPI 频率**: 5 MHz
- **控制通道数**: 8 通道

---

## 快速开始

1. **定义 SPI 与 GPIO 回调函数**:
   根据您的硬件平台（如 STM32, ESP32, AUTOSAR 等）实现 SPI 读写和 GPIO 控制。
2. **填充配置结构体**:
   设置 `TLE75008_Conf_t`。
3. **初始化设备**:
   调用 `TLE75008_Init` 获取设备句柄。
4. **控制输出**:
   使用 `TLE75008_OutputChx` 控制单通道或 `TLE75008_OutputChBatch` 批量控制。

```c
// 示例：简单初始化
TLE75008_Handle_t myDevice;
TLE75008_Conf_t config = {
    .spi_tx_rx = my_platform_spi_transfer,
    .Idle_func = my_idle_pin_ctrl,
    .Cs_func = my_cs_pin_ctrl
};

if (TLE75008_Init(&myDevice, &config) == TLE75008_OK) {
    // 开启通道 0
    TLE75008_OutputChx(myDevice, TLE75008_CHANNEL_0, TLE75008_OUTPUT_ON);
}
```

---

## API 原型说明

### TLE75008_Init
`TLE75008_Status_t TLE75008_Init(TLE75008_Handle_t *handle, TLE75008_Conf_t *conf);`
- **handle**: 指向句柄的指针，初始化成功后会被赋值。
- **conf**: 硬件配置信息，包含 SPI 和 GPIO 的函数指针。

### TLE75008_DeInit
`void TLE75008_DeInit(TLE75008_Handle_t *handle);`
- **handle**: 设备句柄，释放相关资源。

### TLE75008_OutputChx
`TLE75008_Status_t TLE75008_OutputChx(TLE75008_Handle_t handle, TLE75008_Channel_t channel, TLE75008_Output_t value);`
- **handle**: 设备句柄。
- **channel**: 目标通道索引 (0-7)。
- **value**: 状态设定（ON / OFF）。

### TLE75008_OutputChBatch
`TLE75008_Status_t TLE75008_OutputChBatch(TLE75008_Handle_t handle, TLE75008_ChMask_t mask);`
- **handle**: 设备句柄。
- **mask**: 8位通道掩码，每一位代表一个通道。

### TLE75008_SetMapInx
`TLE75008_Status_t TLE75008_SetMapInx(TLE75008_Handle_t handle, TLE75008_Input_t input, TLE75008_ChMask_t mask);`
- **handle**: 设备句柄。
- **input**: 选择物理输入引脚 IN0 或 IN1。
- **mask**: 映射到该输入引脚的通道掩码（1表示连接）。

### TLE75008_GetInstStatus
`TLE75008_Status_t TLE75008_GetInstStatus(TLE75008_Handle_t handle, TLE75008_Inst_t *value);`
- **handle**: 设备句柄。
- **value**: 输出参数，用于存放当前输入引脚电平和 SPI 错误状态。

### TLE75008_EnChxFaultDiag
`TLE75008_Status_t TLE75008_EnChxFaultDiag(TLE75008_Handle_t handle, TLE75008_ChMask_t mask);`
- **handle**: 设备句柄。
- **mask**: 需要开启故障诊断（开路/短路检测）的通道掩码。

### TLE75008_ChxFaultDete
`TLE75008_Status_t TLE75008_ChxFaultDete(TLE75008_Handle_t handle, TLE75008_ChMask_t *mask);`
- **handle**: 设备句柄。
- **mask**: 输出参数，返回存在故障的通道掩码。

### TLE75008_SoftWareReset
`TLE75008_Status_t TLE75008_SoftWareReset(TLE75008_Handle_t handle);`
- **handle**: 设备句柄。执行软件复位。

---

## 结构体说明

### TLE75008_Conf_t
硬件抽象层配置结构体。
- **Idle_func**: GPIO 回调，控制 IDLE 引脚。
- **Cs_func**: GPIO 回调，控制 CS 片选引脚（需启用 `TLE75008_USE_CS`）。
- **In0_func / In1_func**: GPIO 回调，控制硬件输入引脚。若使用 PWM 模式可在此实现占空比更新。
- **spi_tx**: SPI 发送单向回调。
- **spi_rx**: SPI 接收单向回调。
- **spi_tx_rx**: SPI 全双工交换回调（推荐）。

### TLE75008_Inst_t
实时状态结构体。
- **In0_value**: 物理引脚 IN0 的逻辑状态。
- **In1_value**: 物理引脚 IN1 的逻辑状态。
- **trans_err**: 上一次 SPI 传输是否存在错误 (TER)。

### TLE75008_ChMask_t (Union)
通道选择掩码。
- **bits**: 按位访问 (CH_0 到 CH_7)。
- **mask**: 原始 8 位数据访问。

### TLE75008_Parallel_t (Union)
并联保护配置掩码。
- **bits.CH0_1**: 通道0和1并联。
- **bits.CH2_3**: 通道2和3并联。
- **bits.CH4_5**: 通道4和5并联。
- **bits.CH6_7**: 通道6和7并联。
- **bytes**: 原始字节数据。

---

## 枚举值说明

### TLE75008_Status_t (执行结果)
- `TLE75008_OK`: 操作成功。
- `TLE75008_ERR`: 通用错误。
- `TLE75008_SPI_ERR`: 通信失败。
- `TLE75008_MEMORY_ERR`: 内存分配错误。

### TLE75008_Channel_t (通道索引)
- `TLE75008_CHANNEL_0` 至 `TLE75008_CHANNEL_7`: 通道 0 到 7。

### TLE75008_Output_t (输出电平)
- `TLE75008_OUTPUT_OFF`: 通道关闭。
- `TLE75008_OUTPUT_ON`: 通道开启。

### TLE75008_Mode_t (工作模式)
- `TLE75008_MODE_SLEEP`: 睡眠模式（极低功耗）。
- `TLE75008_MODE_LIMP_HOME`: 跛行模式（SPI只读，由IN0/IN1硬件控制）。
- `TLE75008_MODE_IDLE`: 空闲模式。
- `TLE75008_MODE_ACTIVE`: 激活模式（正常工作）。

### TLE75008_Input_t (硬件输入口)
- `TLE75008_INPUT_0`: 指向物理 IN0。
- `TLE75008_INPUT_1`: 指向物理 IN1。