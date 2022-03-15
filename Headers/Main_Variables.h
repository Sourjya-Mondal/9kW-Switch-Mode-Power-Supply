// Start of File
#ifndef HEADERS_MAIN_VARIABLES_H_
#define HEADERS_MAIN_VARIABLES_H_

#include "Parameters_Initialization.h" // Include Parameters Initialization

//
// Define Structure to Point Address of DAC Register
volatile struct CPUTIMER_REGS *CPU_Timer[] = {
                                               &CpuTimer0Regs, // Instance to Point to Address of CPU Timer 0 Register
                                               &CpuTimer1Regs, // Instance to Point to Address of CPU Timer 1 Register
                                               &CpuTimer2Regs, // Instance to Point to Address of CPU Timer 2 Register
};

// Define Structure to Point Address of EPWM Register
volatile struct EPWM_REGS *ePWM[] = {
                                      &EPwm1Regs,  // Instance to Point to Address of EPWM Channel 0 Register (Do Not Use ePWM[0] as it is not defined since there is no EPwm0regs in TMS320F28377S)
                                      &EPwm1Regs,  // Instance to Point to Address of EPWM Channel 1 Register
                                      &EPwm2Regs,  // Instance to Point to Address of EPWM Channel 2 Register
                                      &EPwm3Regs,  // Instance to Point to Address of EPWM Channel 3 Register
                                      &EPwm4Regs,  // Instance to Point to Address of EPWM Channel 4 Register
                                      &EPwm5Regs,  // Instance to Point to Address of EPWM Channel 5 Register
                                      &EPwm6Regs,  // Instance to Point to Address of EPWM Channel 6 Register
                                      &EPwm7Regs,  // Instance to Point to Address of EPWM Channel 7 Register
                                      &EPwm8Regs,  // Instance to Point to Address of EPWM Channel 8 Register
                                      &EPwm9Regs,  // Instance to Point to Address of EPWM Channel 9 Register (Do Not Use ePWM[9] since there is no hardware provision to use this channel in HRT-1302)
                                      &EPwm10Regs, // Instance to Point to Address of EPWM Channel 10 Register (Do Not Use ePWM[10] since there is no hardware provision to use this channel in HRT-1302)
                                      &EPwm11Regs, // Instance to Point to Address of EPWM Channel 11 Register (Do Not Use ePWM[11] since there is no hardware provision to use this channel in HRT-1302)
                                      &EPwm12Regs, // Instance to Point to Address of EPWM Channel 12 Register (Do Not Use ePWM[12] since there is no hardware provision to use this channel in HRT-1302)
};

// Define Structure to Point Address of ADC Register
volatile struct ADC_REGS *ADC_Define[] = {
                                           &AdcaRegs, // Instance to Point to Address of ADC A Register
                                           &AdcbRegs, // Instance to Point to Address of ADC B Register
                                           &AdccRegs, // Instance to Point to Address of ADC C Register
                                           &AdcdRegs, // Instance to Point to Address of ADC D Register
};

// Define Structure to Point Address of ADC Result Register
volatile struct ADC_RESULT_REGS *ADC_Result[] = {
                                                  &AdcaResultRegs, // Instance to Point to Address of ADC A Result Register
                                                  &AdcbResultRegs, // Instance to Point to Address of ADC B Result Register
                                                  &AdccResultRegs, // Instance to Point to Address of ADC C Result Register
                                                  &AdcdResultRegs, // Instance to Point to Address of ADC D Result Register
};

// Define Structure to Point Address of DAC Register
volatile struct DAC_REGS *DAC_Define[] = {
                                           &DacaRegs, // Instance to Point to Address of DAC A Register
                                           &DacbRegs, // Instance to Point to Address of DAC B Register
                                           &DaccRegs, // Instance to Point to Address of DAC C Register
};

float DUTY_VALUE = RESET; // Declare External Variable for Duty Cycle of PWM of Leg 1 (0 Degree)
float DUTY_FIFTY = RESET; // Declare External Variable for Duty Cycle of PWM of Leg 2 (180 Degree)

unsigned int Fault_Count = 0;

bool MASTER_STATUS     = true;  // Declare Variable for Priority Status (Master or Slave) of the Module
bool INITIAL_CONDITION = true;  // Declare Variable for Operating Condition of the Module
bool PSFB_Fault        = false; // Declare Variable for Determining Fault Status of the

#endif /* HEADERS_MAIN_VARIABLES_H_ */

// End of File
