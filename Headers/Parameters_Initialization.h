// Start of File
#ifndef HEADERS_PARAMETERS_INITIALIZATION_H_
#define HEADERS_PARAMETERS_INITIALIZATION_H_

//
// Define Generic parameters
#define RESET    0
#define SET      1
#define TURN_OFF 0
#define TURN_ON  1
//
// Define Flag Operation Parameters
#define IGNORE_INTERRUPT     0
#define AKNOWLEDGE_INTERRUPT 1
#define FLAG_IGNORE          0
#define FLAG_CLEAR           1
//
// Define CPU Timer Interrupt Parameters
#define CLK_DISABLE           0
#define CLK_ENABLE            1
#define CPU_TIM0              0
#define CPU_TIM1              1
#define CPU_TIM2              2
#define CPU_TIM0_SMP_TIME 10000
//
// Define ADC Parameters
#define CLK_DIV_1_0       0
#define CLK_DIV_1_5       1
#define CLK_DIV_2_0       2
#define CLK_DIV_2_5       3
#define CLK_DIV_3_0       4
#define CLK_DIV_3_5       5
#define CLK_DIV_4_0       6
#define CLK_DIV_4_5       7
#define CLK_DIV_5_0       8
#define CLK_DIV_5_5       9
#define CLK_DIV_6_0      10
#define CLK_DIV_6_5      11
#define CLK_DIV_7_0      12
#define CLK_DIV_7_5      13
#define CLK_DIV_8_0      14
#define CLK_DIV_8_5      15
#define ADC_TURN_OFF      0
#define ADC_TURN_ON       1
#define ACQUISITION_TIME 99
#define PPB_01            1
#define PPB_02            2
#define PPB_03            3
#define PPB_04            4
//
// Define EPWM Parameters
#define SMPS_ZERO      1
#define SMPS_HALF      2
#define SMPS_PERIOD 6666
//#define PWMxA 0
#define PWMxA                     0.6*SMPS_PERIOD
#define PWMxB                  3333
#define ZERO                      0
#define PHASE                  3333
#define SOFT_START_TIME_PERIOD    0.0001
//
#define VOLTAGE 0
#define CURRENT 1
//
// Define Voltage Loop Parameters
#define VOLTAGE_REFERENCE         1470
#define VOLTAGE_REFERENCE_MAXIMUM 1500
#define VOLTAGE_REFERENCE_MINIMUM    0
#define VOLTAGE_FEEDBACK_MAXIMUM  4096
#define VOLTAGE_FEEDBACK_MINIMUM     0
#define VOLTAGE_Kp                   1.000000
#define VOLTAGE_Ki                   0.001000
#define VOLTAGE_PI_MAXIMUM        2950
#define VOLTAGE_PI_MINIMUM           0
//
// Define Current Loop Parameters
#define CURRENT_REFERENCE         2920
#define CURRENT_REFERENCE_MAXIMUM 2950
#define CURRENT_REFERENCE_MINIMUM    0
#define CURRENT_FEEDBACK_MAXIMUM  4096
#define CURRENT_FEEDBACK_MINIMUM     0
#define CURRENT_Kp                   0.025000
#define CURRENT_Ki                   0.000001
#define CURRENT_PI_MAXIMUM        5000
#define CURRENT_PI_MINIMUM           0
//
//Define DAC Registers
#define DAC_DACA   0
#define DAC_DACB   1
#define DAC_DACC   2
#define SYSCLK     0
#define VREF       1
#define DAC_ENABLE 1
//
// Low Pass Filter Parameter
#define LPF_TS     0.010000
#define LPF_INV_TS 1.000000-LPF_TS
//
// ADC Signals
#define V_DC_OUT  0
#define I_DC_OUT  1
#define I_REF_MT  2
#define V_RPHASE  3
#define V_YPHASE  4
#define V_BPHASE  5
#define I_RPHASE  6
#define I_YPHASE  7
#define I_BPHASE  8
#define V_DC_BUS  9
#define I_DC_BUS 10
//
// Define
#define ADC_CHANNEL_00  0
#define ADC_CHANNEL_01  1
#define ADC_CHANNEL_02  2
#define ADC_CHANNEL_03  3
#define ADC_CHANNEL_04  4
#define ADC_CHANNEL_05  5
#define ADC_CHANNEL_06  6
#define ADC_CHANNEL_07  7
#define ADC_CHANNEL_08  8
#define ADC_CHANNEL_09  9
#define ADC_CHANNEL_10 10
#define ADC_CHANNEL_11 11
#define ADC_CHANNEL_12 12
#define ADC_CHANNEL_13 13
#define ADC_CHANNEL_14 14
#define ADC_CHANNEL_15 15
//
//ADC SOC Parameter
#define SOC_00   0
#define SOC_01   1
#define SOC_02   2
#define SOC_03   3
#define SOC_04   4
#define SOC_05   5
#define SOC_06   6
#define SOC_07   7
#define SOC_08   8
#define SOC_09   9
#define SOC_10  10
#define SOC_11  11
#define SOC_12  12
#define SOC_13  13
#define SOC_14  14
#define SOC_15  15
//
// ADC Trigger Source Parameter
#define Trig_Software   0
#define Trig_CPU_Tx00   1
#define Trig_CPU_Tx01   2
#define Trig_CPU_Tx02   3
#define Trig_External   4
#define Trig_EPWMx01A   5
#define Trig_EPWMx01B   6
#define Trig_EPWMx02A   7
#define Trig_EPWMx02B   8
#define Trig_EPWMx03A   9
#define Trig_EPWMx03B  10
#define Trig_EPWMx04A  11
#define Trig_EPWMx04B  12
#define Trig_EPWMx05A  13
#define Trig_EPWMx05B  14
#define Trig_EPWMx06A  15
#define Trig_EPWMx06B  16
#define Trig_EPWMx07A  17
#define Trig_EPWMx07B  18
#define Trig_EPWMx08A  19
#define Trig_EPWMx08B  20
#define Trig_EPWMx09A  21
#define Trig_EPWMx09B  22
#define Trig_EPWMx10A  23
#define Trig_EPWMx10B  24
#define Trig_EPWMx11A  25
#define Trig_EPWMx11B  26
#define Trig_EPWMx12A  27
#define Trig_EPWMx12B  28
//
// ADC Behaviour Parameter
#define ACQ_END 0
#define SOC_END 1
//
#define ET_ENABLE 1

#endif /* HEADERS_PARAMETERS_INITIALIZATION_H_ */

// End of File
