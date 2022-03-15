// Start of File
#include "F28x_Project.h"   // Include Device Header File and Examples
#include <stdint.h>         // Include Standard Integer File
#include <stdbool.h>        // Include Standard Boolean File
#include <stdio.h>          // Include Standard Input/Output File
#include <file.h>           // Include Standard Header File
#include "math.h"           // Include Math Header File
#include "Main_Variables.h" // Include Main Variables File

// SMPS Functions
void EPWM_Initialization();
void EPWM_Interrupt_Initialization();
void EPWM_Trip_Zone_Interrupt_Initialization();
void Close_Loop();
void Control_Parameters_Initialization();
void Low_Pass_Filter_Parameters_Initialization();

// Define ADC & DAC Functions for PSFB
void CPU_Timer_Initialization();
void ADC_Initialization();

// Fault Checks
void Disable_Error();
void Major_Faults_Check();
void Trip_Initialization();

// Execute Main Function
void main(void)
{
    // Initialize System Control (PLL & WatchDog) and enable Peripheral Clocks.
    InitSysCtrl();
    // Initialize GPIO
    InitGpio();
    // Initialize EPWM GPIO
    InitEPwmGpio();

    DINT; // Clear all __interrupts and initialize PIE vector table:

    // Initialize the PIE control registers to their default state.
    InitPieCtrl();

    // Disable CPU __interrupts and clear all CPU __interrupt flags:
    IER = 0x0000; // Disable CPU interrupts
    IFR = 0x0000; // Clear all CPU interrupt flags

    InitPieVectTable(); // Initialize the PIE vector table with pointers to the shell Interrupt.

    // Call CPU Timer Function to Configure CPU Timer 0
    // CPU Timer 0 Initialization for ADC Register
    CPU_Timer_Initialization(CPU_TIM0, CPU_TIM0_SMP_TIME); // Set CPU_TIM_REG = CPU_TIMER0

    // Configure the ADC channels
    // Call ADC Initialization function to Initialize Supply Voltage ADC Channels
    // ADC Initialization for Input R Phase Voltage Feedback
    ADC_Initialization(ADC_ADCA, SOC_00, ADC_CHANNEL_02, // Set ADC_REG = ADC_ADCA, SOC_SEL = SOC_00, ADC_CH_SEL = ADC_CHANNEL_02
                       ACQUISITION_TIME, Trig_CPU_Tx00); // Set ACQ_PRD = ACQUISITION_TIME, TRIG_SEL = Trig_CPU_Tx00
    // ADC Initialization for Input Y Phase Voltage Feedback
    ADC_Initialization(ADC_ADCA, SOC_01, ADC_CHANNEL_03, // Set ADC_REG = ADC_ADCA, SOC_SEL = SOC_01, ADC_CH_SEL = ADC_CHANNEL_03
                       ACQUISITION_TIME, Trig_CPU_Tx00); // Set ACQ_PRD = ACQUISITION_TIME, TRIG_SEL = Trig_CPU_Tx00
    // ADC Initialization for Input B Phase Voltage Feedback
    ADC_Initialization(ADC_ADCA, SOC_02, ADC_CHANNEL_04, // Set ADC_REG = ADC_ADCA, SOC_SEL = SOC_02, ADC_CH_SEL = ADC_CHANNEL_04
                       ACQUISITION_TIME, Trig_CPU_Tx00); // Set ACQ_PRD = ACQUISITION_TIME, TRIG_SEL = Trig_CPU_Tx00

    // Call ADC Initialization function to Initialize DC Bus ADC Channels
    // ADC Initialization for DC Bus Voltage Feedback
    ADC_Initialization(ADC_ADCB, SOC_00, ADC_CHANNEL_00, // Set ADC_REG = ADC_ADCB, SOC_SEL = SOC_00, ADC_CH_SEL = ADC_CHANNEL_00
                       ACQUISITION_TIME, Trig_CPU_Tx00); // Set ACQ_PRD = ACQUISITION_TIME, TRIG_SEL = Trig_CPU_Tx00
    // ADC Initialization for DC Bus Current Feedback
    ADC_Initialization(ADC_ADCB, SOC_01, ADC_CHANNEL_02, // Set ADC_REG = ADC_ADCB, SOC_SEL = SOC_01, ADC_CH_SEL = ADC_CHANNEL_00
                       ACQUISITION_TIME, Trig_CPU_Tx00); // Set ACQ_PRD = ACQUISITION_TIME, TRIG_SEL = Trig_CPU_Tx00

    // Call ADC Initialization function to Initialize Supply Current ADC Channels
    // ADC Initialization for Input R Phase Current Feedback
    ADC_Initialization(ADC_ADCC, SOC_00, ADC_CHANNEL_02, // Set ADC_REG = ADC_ADCC, SOC_SEL = SOC_00, ADC_CH_SEL = ADC_CHANNEL_02
                       ACQUISITION_TIME, Trig_CPU_Tx00); // Set ACQ_PRD = ACQUISITION_TIME, TRIG_SEL = Trig_CPU_Tx00
    // ADC Initialization for Input Y Phase Current Feedback
    ADC_Initialization(ADC_ADCC, SOC_01, ADC_CHANNEL_03, // Set ADC_REG = ADC_ADCC, SOC_SEL = SOC_01, ADC_CH_SEL = ADC_CHANNEL_03
                       ACQUISITION_TIME, Trig_CPU_Tx00); // Set ACQ_PRD = ACQUISITION_TIME, TRIG_SEL = Trig_CPU_Tx00
    // ADC Initialization for Input B Phase Current Feedback
    ADC_Initialization(ADC_ADCC, SOC_02, ADC_CHANNEL_04, // Set ADC_REG = ADC_ADCC, SOC_SEL = SOC_02, ADC_CH_SEL = ADC_CHANNEL_04
                       ACQUISITION_TIME, Trig_CPU_Tx00); // Set ACQ_PRD = ACQUISITION_TIME, TRIG_SEL = Trig_CPU_Tx00

    // Call ADC Initialization function to Initialize DC Output ADC Channels
    // ADC Initialization for Output DC Voltage Feedback
    ADC_Initialization(ADC_ADCD, SOC_00, ADC_CHANNEL_01, // Set ADC_REG = ADC_ADCD, SOC_SEL = SOC_00, ADC_CH_SEL = ADC_CHANNEL_01
                       Trig_CPU_Tx00, ACQUISITION_TIME); // Set TRIG_SEL = Trig_CPU_Tx00, ACQ_PRD = ACQUISITION_TIME
    // ADC Initialization for Output DC Current Feedback
    ADC_Initialization(ADC_ADCD, SOC_01, ADC_CHANNEL_02, // Set ADC_REG = ADC_ADCD, SOC_SEL = SOC_01, ADC_CH_SEL = ADC_CHANNEL_02
                       Trig_CPU_Tx00, ACQUISITION_TIME); // Set TRIG_SEL = Trig_CPU_Tx00, ACQ_PRD = ACQUISITION_TIME
    // ADC Initialization for Current Reference for Slave
    ADC_Initialization(ADC_ADCD, SOC_02, ADC_CHANNEL_03, // Set ADC_REG = ADC_ADCD, SOC_SEL = SOC_02, ADC_CH_SEL = ADC_CHANNEL_03
                       Trig_CPU_Tx00, ACQUISITION_TIME); // Set TRIG_SEL = Trig_CPU_Tx00, ACQ_PRD = ACQUISITION_TIME

    // Call EPWM Initialization Function to Initialize EPWM for SMPS
    // Leg 1 (0 Degree)
    EPWM_Initialization(SMPS_ZERO, TB_CTR_ZERO, SMPS_PERIOD, // Set EPWM_CH_SEL = SMPS_ZERO, SNC_SRC = TB_CTR_ZERO, TB_PRD = SMPS_PERIOD
                        ZERO, DUTY_VALUE, DUTY_FIFTY);       // Set PHSE_OFF = ZERO, CMP_A = DUTY_VALUE, CMP_B = DUTY_FIFTY
    // Leg 2 (180 Degree)
    EPWM_Initialization(SMPS_HALF, TB_SYNC_IN, SMPS_PERIOD,  // Set EPWM_CH_SEL = SMPS_HALF, SNC_SRC = TB_SYNC_IN, TB_PRD = SMPS_PERIOD
                        PHASE, DUTY_VALUE, DUTY_FIFTY);      // Set PHSE_OFF = PHASE, CMP_A = DUTY_VALUE, CMP_B = DUTY_FIFTY

    // Call Control Loop Parameter Function to Reset the Calculated Variables and Load the Constant Values of the Control Loop
    // Voltage Control Loop Parameters Load and Reset
    Control_Parameters_Initialization(VOLTAGE, VOLTAGE_REFERENCE,                           // Set LOOP_PARAM = VOLTAGE
                                      VOLTAGE_FEEDBACK_MAXIMUM, VOLTAGE_FEEDBACK_MINIMUM,   // Set FBK_MAX = VOLTAGE_FEEDBACK_MAXIMUM, FBK_MIN = VOLTAGE_FEEDBACK_MINIMUM
                                      VOLTAGE_REFERENCE_MAXIMUM, VOLTAGE_REFERENCE_MINIMUM, // Set REF_MAX = VOLTAGE_REFERENCE_MAXIMUM, REF_MIN = VOLTAGE_REFERENCE_MINIMUM
                                      VOLTAGE_PI_MAXIMUM, VOLTAGE_PI_MINIMUM,               // Set PI_MAX = VOLTAGE_PI_MAXIMUM, PI_MIN = VOLTAGE_PI_MINIMUM
                                      VOLTAGE_Kp, VOLTAGE_Ki);                              // Set Kp_CNST = VOLTAGE_Kp, Ki_CNST = VOLTAGE_Ki
    // Current Control Loop Parameters Load and Reset
    Control_Parameters_Initialization(CURRENT, CURRENT_REFERENCE,                           // Set LOOP_PARAM = CURRENT
                                      CURRENT_FEEDBACK_MAXIMUM, CURRENT_FEEDBACK_MINIMUM,   // Set FBK_MAX = CURRENT_FEEDBACK_MAXIMUM, FBK_MIN = CURRENT_FEEDBACK_MINIMUM
                                      CURRENT_REFERENCE_MAXIMUM, CURRENT_REFERENCE_MINIMUM, // Set REF_MAX = CURRENT_REFERENCE_MAXIMUM, REF_MIN = CURRENT_REFERENCE_MINIMUM
                                      CURRENT_PI_MAXIMUM, CURRENT_PI_MINIMUM,               // Set PI_MAX = CURRENT_PI_MAXIMUM, PI_MIN = CURRENT_PI_MINIMUM
                                      CURRENT_Kp, CURRENT_Ki);                              // Set Kp_CNST = CURRENT_Kp, Ki_CNST = CURRENT_Ki

    // Call Control Loop Parameter Function to Reset the Calculated Variables and Load the Constant Values of the Low Pass Filter
    // DC Output Voltage Feedback Parameters Load and Reset
    Low_Pass_Filter_Parameters_Initialization(V_DC_OUT, LPF_TS, LPF_INV_TS); // Set FILT_PARAM = V_DC_OUT, TS = LPF_TS, INV_TS = LPF_INV_TS
    // DC Output Current Feedback Parameters Load and Reset
    Low_Pass_Filter_Parameters_Initialization(I_DC_OUT, LPF_TS, LPF_INV_TS); // Set FILT_PARAM = I_DC_OUT, TS = LPF_TS, INV_TS = LPF_INV_TS

    // Call EPWM Interrupt Initialization Function to Initialize EPWM Interrupt Interrupt
    // Interrupt for EPWM Leg 1 (0 Degree)
    EPWM_Interrupt_Initialization(SMPS_ZERO); // Set EPWM (Leg 1) Interrupt to Update the Value of Comparator
    // Interrupt for EPWM Leg 2 (180 Degrees)
    EPWM_Interrupt_Initialization(SMPS_HALF); // Set EPWM (Leg 2) Interrupt to Update the Value of Comparator

//    // Call EPWM Interrupt Initialization Function to Initialize EPWM Trip Zone Interrupt
//    // Interrupt for EPWM Leg 1 (0 Degree)
//    EPWM_Trip_Zone_Interrupt_Initialization(SMPS_ZERO); // Set EPWM (Leg 1) Interrupt to Execute Trip Logic
//    // Interrupt for EPWM Leg 2 (180 Degrees)
//    EPWM_Trip_Zone_Interrupt_Initialization(SMPS_HALF); // Set EPWM (Leg 2) Interrupt to Execute Trip Logic

//    // Output DC Voltage Fault Check
//    Major_Faults_Check(ADC_ADCD, PPB_01, ADC_CHANNEL_01,                    // Set ADC_REG = ADC_ADCD, PP_BLOCK = PPB_01, RESULT = ADC_CHANNEL_02
//                       VOLTAGE_FEEDBACK_MAXIMUM, VOLTAGE_FEEDBACK_MINIMUM); // Set UPP_LIM = VOLTAGE_FEEDBACK_MAXIMUM, LOW_LIM = VOLTAGE_FEEDBACK_MINIMUM
//    // Output DC Current Fault Check
//    Major_Faults_Check(ADC_ADCD, PPB_02, ADC_CHANNEL_02,                    // Set ADC_REG = ADC_ADCD, PP_BLOCK = PPB_02, RESULT = ADC_CHANNEL_02
//                       CURRENT_FEEDBACK_MAXIMUM, CURRENT_FEEDBACK_MINIMUM); // Set UPP_LIM = CURRENT_FEEDBACK_MAXIMUM, LOW_LIM = CURRENT_FEEDBACK_MINIMUM

//    // Initialize Trip Zone for EPWM of Leg 1 (0 Degree)
//    Trip_Initialization(SMPS_ZERO); // Set EPWM_CH_SEL = SMPS_ZERO
//    // Initialize Trip Zone for EPWM of Leg 2 (180 Degree)
//    Trip_Initialization(SMPS_HALF); // Set EPWM_CH_SEL = SMPS_HALF

    // Set Fill Terminal to High In Order to Enable EPWM7 and EPWM8
    Disable_Error();

    // Step 5. Enable interrupts:
    EINT; // Enable Global interrupt INTM
    ERTM; // Enable Global Real Time Interrupt DBGM

    // Infinite Loop
    while(true)
    {
        Close_Loop(); // Call Close Loop Function to Evaluate and Update the Duty Cycle
    }
}

// Disable Error Signal
void Disable_Error(void)
{
    EALLOW;                                        // Register Protection Off
    // GPIO A Mux 2 Register (GPIO16 to 31)
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = GPIO_UNLOCK; // Select GPIO62 as a GPIO pin
    // GPIO A Direction Register (GPIO0 to 31)
    GpioCtrlRegs.GPADIR.bit.GPIO20  = GPIO_OUTPUT; // Configure GPIO62 as Output Pin
    // GPIO A Pull Up Disable Register (GPIO0 to 31)
    GpioCtrlRegs.GPAPUD.bit.GPIO20  = GPIO_PULLUP; // Configure GPIO62 as Pull-up Terminal
    // GPIO A Data Set Register (GPIO0 to 31)
    GpioDataRegs.GPASET.bit.GPIO20  = GPYSET;      // Tie GPIO62 to Always High
    EDIS;                                          // Register Protection On
}

// End of File
