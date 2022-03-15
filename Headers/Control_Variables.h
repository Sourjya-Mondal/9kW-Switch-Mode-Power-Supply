// Start of File
#ifndef HEADERS_CONTROL_VARIABLES_H_
#define HEADERS_CONTROL_VARIABLES_H_

#include "Parameters_Initialization.h" // Include Parameters Initialization

extern float DUTY_VALUE; // Declare External Variable for Duty Cycle of PWM of Leg 1 (0 Degree)
extern float DUTY_FIFTY; // Declare External Variable for Duty Cycle of PWM of Leg 2 (180 Degree)

extern bool MASTER_STATUS;     // Declare External Variable for Priority Status (Master or Slave) of the Module
extern bool INITIAL_CONDITION; // Declare External Variable for Operating Condition of the Module

extern volatile struct EPWM_REGS *ePWM[];             // Declare External Structure for EPWM Register
extern volatile struct ADC_RESULT_REGS *ADC_Result[]; // Declare External Structure for ADC Result Register
extern volatile struct DAC_REGS *DAC_Define[];        // Declare External Structure for DAC Register

struct PI_Loop {
                 float PI_OUTPUT;         // Instance for Control Loop PI Output
                 float PI_MAXIMUM;        // Instance for Control Loop Maximum PI Output
                 float PI_MINIMUM;        // Instance for Control Loop Minimum PI Output
                 float REFERENCE;         // Instance for Control Loop Reference Signal
                 float REFERENCE_MAXIMUM; // Instance for Control Loop Maximum Reference Signal
                 float REFERENCE_MINIMUM; // Instance for Control Loop Minimum Reference Signal
                 float FEEDBACK;          // Instance for Control Loop Feedback Signal
                 float FEEDBACK_MAXIMUM;  // Instance for Control Loop Maximum Feedback Signal
                 float FEEDBACK_MINIMUM;  // Instance for Control Loop Minimum Feedback Signal
                 float ERROR;             // Instance for Control Loop Error Signal
                 float PROPORTIONAL;      // Instance for Control Loop Proportional Output
                 float INTEGRAL;          // Instance for Control Loop Integral Output
                 float PREVIOUS_INTEGRAL; // Instance for Control Loop Previous Integral Output
                 float Kp;                // Instance for Control Loop Proportional Constant (Kp)
                 float Ki;                // Instance for Control Loop Integral Constant (Ki)
};

struct PI_Loop SMPS_VOLTAGE; // Structure for Voltage Control Loop
struct PI_Loop SMPS_CURRENT; // Structure for Current Control Loop

struct PI_Loop *Control[] = {
                              &SMPS_VOLTAGE, // Instance to Point to Address of Structure for Voltage Control Loop
                              &SMPS_CURRENT, // Instance to Point to Address of Structure for Current Control Loop
};

// Structure for Low Pass Filter
struct LPF {
             float LPF_SAMPLING_PERIOD;      // Instance for Low Pass Filter Sampling Time Period
             float LPF_COMP_SAMPLING_PERIOD; // Instance for Low Pass Filter (1 - Sampling Time Period)
             float LPF_PRESENT_INPUT;        // Instance for Low Pass Filter Present Input
             float LPF_PRESENT_OUTPUT;       // Instance for Low Pass Filter Present Output
             float LPF_PREVIOUS_OUTPUT;      // Instance for Low Pass Filter Previous Output
             float LPF_RETURN_OUTPUT;        // Instance for Low Pass Filter Return output
};

struct LPF OUTPUT_DC_VOLTAGE;        // Structure for Output DC Voltage (from ADC D1)
struct LPF OUTPUT_DC_CURRENT;        // Structure for Output DC Current (from ADC D2)
struct LPF MASTER_CURRENT_REFERENCE; // Structure for Current Reference form Master Module (from ADC D3)
struct LPF SUPPLY_RPHASE_VOLTAGE;    // Structure for Supply R Phase Voltage (from ADC A2)
struct LPF SUPPLY_YPHASE_VOLTAGE;    // Structure for Supply Y Phase Voltage (from ADC A3)
struct LPF SUPPLY_BPHASE_VOLTAGE;    // Structure for Supply B Phase Voltage (from ADC A4)
struct LPF SUPPLY_RPHASE_CURRENT;    // Structure for Supply R Phase Current (from ADC C2)
struct LPF SUPPLY_YPHASE_CURRENT;    // Structure for Supply Y Phase Current (from ADC C3)
struct LPF SUPPLY_BPHASE_CURRENT;    // Structure for Supply B Phase Current (from ADC C4)
struct LPF DC_BUS_VOLTAGE;           // Structure for DC Bus Voltage (from ADC B0)
struct LPF DC_BUS_CURRENT;           // Structure for DC Bus Current (from ADC B2)

// Structure to Point Various Signals
struct LPF *LP_FILTER[] = {
                            &OUTPUT_DC_VOLTAGE,        // Instance to Point to Address of Structure for Output DC Voltage
                            &OUTPUT_DC_CURRENT,        // Instance to Point to Address of Structure for Output DC Current
                            &MASTER_CURRENT_REFERENCE, // Instance to Point to Address of Structure for Current Reference
                            &SUPPLY_RPHASE_VOLTAGE,    // Instance to Point to Address of Structure for Supply R Phase Voltage
                            &SUPPLY_YPHASE_VOLTAGE,    // Instance to Point to Address of Structure for Supply Y Phase Voltage
                            &SUPPLY_BPHASE_VOLTAGE,    // Instance to Point to Address of Structure for Supply B Phase Voltage
                            &SUPPLY_RPHASE_CURRENT,    // Instance to Point to Address of Structure for Supply R Phase Current
                            &SUPPLY_YPHASE_CURRENT,    // Instance to Point to Address of Structure for Supply Y Phase Current
                            &SUPPLY_BPHASE_CURRENT,    // Instance to Point to Address of Structure for Supply B Phase Current
                            &DC_BUS_VOLTAGE,           // Instance to Point to Address of Structure for DC Bus Voltage
                            &DC_BUS_CURRENT,           // Instance to Point to Address of Structure for DC Bus Current
};

#endif /* HEADERS_CONTROL_VARIABLES_H_ */

// End of File
