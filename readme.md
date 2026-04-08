
[中文](./readme.zh.md)

# TLE75008 Driver Library Documentation

This library is a universal C driver for the Infineon TLE75008 8-channel high-side switch. It supports diagnosis, configuration, and control via SPI.

## Hardware Specifications
- **SPI Mode**: CPOL = 0, CPHA = 1 (Idle low, sample on second edge)
- **Max SPI Frequency**: 5 MHz
- **Control Channels**: 8 Channels

---

## Quick Start

1.  **Define SPI and GPIO Callback Functions**:
    Implement SPI read/write and GPIO control according to your hardware platform (e.g., STM32, ESP32, AUTOSAR, etc.).
2.  **Fill the Configuration Structure**:
    Set up the `TLE75008_Conf_t` members.
3.  **Initialize the Device**:
    Call `TLE75008_Init` to obtain the device handle.
4.  **Control Outputs**:
    Use `TLE75008_OutputChx` for single-channel control or `TLE75008_OutputChBatch` for batch control.

```c
// Example: Simple Initialization
TLE75008_Handle_t myDevice;
TLE75008_Conf_t config = {
    .spi_tx_rx = my_platform_spi_transfer,
    .Idle_func = my_idle_pin_ctrl,
    .Cs_func = my_cs_pin_ctrl
};

if (TLE75008_Init(&myDevice, &config) == TLE75008_OK) {
    // Turn on Channel 0
    TLE75008_OutputChx(myDevice, TLE75008_CHANNEL_0, TLE75008_OUTPUT_ON);
}
```

---

## API Prototype Descriptions

### TLE75008_Init
`TLE75008_Status_t TLE75008_Init(TLE75008_Handle_t *handle, TLE75008_Conf_t *conf);`
- **handle**: Pointer to the device handle; assigned upon successful initialization.
- **conf**: Hardware configuration information, including SPI and GPIO function pointers.

### TLE75008_DeInit
`void TLE75008_DeInit(TLE75008_Handle_t *handle);`
- **handle**: Device handle to be de-initialized and resources released.

### TLE75008_OutputChx
`TLE75008_Status_t TLE75008_OutputChx(TLE75008_Handle_t handle, TLE75008_Channel_t channel, TLE75008_Output_t value);`
- **handle**: Device handle.
- **channel**: Target channel index (0-7).
- **value**: Output state setting (ON / OFF).

### TLE75008_OutputChBatch
`TLE75008_Status_t TLE75008_OutputChBatch(TLE75008_Handle_t handle, TLE75008_ChMask_t mask);`
- **handle**: Device handle.
- **mask**: 8-bit channel mask, where each bit represents a channel.

### TLE75008_SetMapInx
`TLE75008_Status_t TLE75008_SetMapInx(TLE75008_Handle_t handle, TLE75008_Input_t input, TLE75008_ChMask_t mask);`
- **handle**: Device handle.
- **input**: Selects physical input pin IN0 or IN1.
- **mask**: Channel mask to be mapped to the selected input pin (1 = connected).

### TLE75008_GetInstStatus
`TLE75008_Status_t TLE75008_GetInstStatus(TLE75008_Handle_t handle, TLE75008_Inst_t *value);`
- **handle**: Device handle.
- **value**: Output parameter; stores the current input pin levels and SPI error status.

### TLE75008_EnChxFaultDiag
`TLE75008_Status_t TLE75008_EnChxFaultDiag(TLE75008_Handle_t handle, TLE75008_ChMask_t mask);`
- **handle**: Device handle.
- **mask**: Channel mask for which fault diagnosis (Open-load / Short-circuit detection) is to be enabled.

### TLE75008_ChxFaultDete
`TLE75008_Status_t TLE75008_ChxFaultDete(TLE75008_Handle_t handle, TLE75008_ChMask_t *mask);`
- **handle**: Device handle.
- **mask**: Output parameter; returns the mask of channels where a fault is detected.

### TLE75008_SoftWareReset
`TLE75008_Status_t TLE75008_SoftWareReset(TLE75008_Handle_t handle);`
- **handle**: Device handle. Performs a software reset on the device.

---

## Structure Descriptions

### TLE75008_Conf_t
Hardware Abstraction Layer (HAL) configuration structure.
- **Idle_func**: GPIO callback to control the IDLE pin.
- **Cs_func**: GPIO callback to control the CS (Chip Select) pin (required if `TLE75008_USE_CS` is enabled).
- **In0_func / In1_func**: GPIO callbacks to control hardware input pins. If PWM mode is used, implement duty cycle updates within these functions.
- **spi_tx**: Callback for unidirectional SPI transmission.
- **spi_rx**: Callback for unidirectional SPI reception.
- **spi_tx_rx**: Callback for full-duplex SPI transfer (recommended).

### TLE75008_Inst_t
Real-time status structure.
- **In0_value**: Logic state of the physical IN0 pin.
- **In1_value**: Logic state of the physical IN1 pin.
- **trans_err**: Indicates if the last SPI transmission had an error (Transmission Error - TER).

### TLE75008_ChMask_t (Union)
Channel selection mask.
- **bits**: Access by individual bit (CH_0 to CH_7).
- **mask**: Access as a raw 8-bit data byte.

### TLE75008_Parallel_t (Union)
Parallel protection configuration mask.
- **bits.CH0_1**: Channels 0 and 1 in parallel.
- **bits.CH2_3**: Channels 2 and 3 in parallel.
- **bits.CH4_5**: Channels 4 and 5 in parallel.
- **bits.CH6_7**: Channels 6 and 7 in parallel.
- **bytes**: Access as raw byte data.

---

## Enumeration Descriptions

### TLE75008_Status_t (Execution Result)
- `TLE75008_OK`: Operation successful.
- `TLE75008_ERR`: General error.
- `TLE75008_SPI_ERR`: Communication failure.
- `TLE75008_MEMORY_ERR`: Memory allocation error.

### TLE75008_Channel_t (Channel Index)
- `TLE75008_CHANNEL_0` through `TLE75008_CHANNEL_7`: Channel 0 to 7.

### TLE75008_Output_t (Output Level)
- `TLE75008_OUTPUT_OFF`: Channel turned OFF.
- `TLE75008_OUTPUT_ON`: Channel turned ON.

### TLE75008_Mode_t (Operating Mode)
- `TLE75008_MODE_SLEEP`: Sleep mode (ultra-low power).
- `TLE75008_MODE_LIMP_HOME`: Limp Home mode (SPI is read-only, controlled via hardware pins IN0/IN1).
- `TLE75008_MODE_IDLE`: Idle mode.
- `TLE75008_MODE_ACTIVE`: Active mode (normal operation).

### TLE75008_Input_t (Hardware Input Ports)
- `TLE75008_INPUT_0`: Refers to physical pin IN0.
- `TLE75008_INPUT_1`: Refers to physical pin IN1.