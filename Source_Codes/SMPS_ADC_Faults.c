// Start of File
#include "F28x_Project.h"               // Include Device Header File and Examples
#include "Parameters_Initialization.h"  // Include Parameter Load File

extern volatile struct EPWM_REGS *ePWM[];             // Declare External Structure for EPWM Register
extern volatile struct CPUTIMER_REGS *CPU_Timer[];    // Declare External Structure for CPU Timer
extern volatile struct ADC_REGS *ADC_Define[];        // Declare External Structure for ADC Register
extern volatile struct ADC_RESULT_REGS *ADC_Result[]; // Declare External Structure for ADC Result Register

// Initialization of CPU Timer
void CPU_Timer_Initialization(int CPU_TIM_REG, int CPU_TS) // Define Variables to Get CPU Timer Register & CPU Timer Sampling Time Period
{
//    EALLOW;
//    IER |= M_INT1; // Enable Group 1 Interrupt (for CPU Timer 0 Interrupt)
//    EDIS;

    EALLOW;
    (*CPU_Timer[CPU_TIM_REG]).PRD.bit.LSW = CPU_TS; // Set Period to 50us (Use Lower 16bits)
    (*CPU_Timer[CPU_TIM_REG]).TIM.bit.LSW = SET;    // Enable Timer (Lower 16 Bits)
    (*CPU_Timer[CPU_TIM_REG]).TCR.bit.TIE = SET;    // Enable CPU Timer 0 (Send Interrupt Request Timer at Timer = 0)
    EDIS;
}

// Initialization of ADC
void ADC_Initialization(unsigned int ADC_REG , unsigned int SOC, unsigned int ADC_CH, // Define Variables to Get ADC Register, Start of Conversion Selection & ADC Channel Selection
                        unsigned int ADC_TRIG, int ACQ_PRD)                           // Define Variables to Get Acquisition Time Period & Trigger Source Selection
{
    EALLOW;
    CpuSysRegs.PCLKCR13.all = CLK_DISABLE; // Disable ADC Clock
    EDIS;

    if(ADC_REG == ADC_ADCA)                         // Check for ADC A Register
    {
        EALLOW;                                     // Register Protection Off
        CpuSysRegs.PCLKCR13.bit.ADC_A = CLK_ENABLE; // Enable ADC_A Clock
        EDIS;                                       // Register Protection On
    }
    else if(ADC_REG == ADC_ADCB)                    // Check for ADC B Register
    {
        EALLOW;                                     // Register Protection Off
        CpuSysRegs.PCLKCR13.bit.ADC_B = CLK_ENABLE; // Enable ADC_B Clock
        EDIS;                                       // Register Protection On
    }
    else if(ADC_REG == ADC_ADCC)                    // Check for ADC C Register
    {
        EALLOW;                                     // Register Protection Off
        CpuSysRegs.PCLKCR13.bit.ADC_C = CLK_ENABLE; // Enable ADC_C Clock
        EDIS;                                       // Register Protection On
    }
    else if(ADC_REG == ADC_ADCD)                    // Check for ADC D Register
    {
        EALLOW;                                     // Register Protection Off
        CpuSysRegs.PCLKCR13.bit.ADC_D = CLK_ENABLE; // Enable ADC_D Clock
        EDIS;                                       // Register Protection On
    }

    EALLOW;                                                           // Register Protection Off
    // ADC Control 2 Register
    (*ADC_Define[ADC_REG]).ADCCTL2.bit.PRESCALE    = CLK_DIV_4_0;     // ADCCLK = Input Clock/4
    AdcSetMode(ADC_REG, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE); // Set ADC_A Resolution to 12 Bit & Single Signal Mode
    // ADC Control 1 Register
    (*ADC_Define[ADC_REG]).ADCCTL1.bit.INTPULSEPOS = SOC_END;         // Interrupt is Generated at the End of Start of Conversion
    (*ADC_Define[ADC_REG]).ADCCTL1.bit.ADCPWDNZ    = ADC_TURN_ON;     // All Analog Circuitry Inside Core of ADC_B is Powered Up
    EDIS;                                                             // Register Protection On

    DELAY_US(1000); // Delay for 1000us (1ms) to Allow ADC to Power Up

    switch(SOC)                                                               // Determine Start of Conversion Selection
    {
        // SOC0 is Selected
        case 0:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC0CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC0CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 0 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC0CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC1 is Selected
        case 1:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC1CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC1CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 1 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC1CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC2 is Selected
        case 2:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC2CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC2CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 2 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC2CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC3 is Selected
        case 3:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC3CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC3CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 3 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC3CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC4 is Selected
        case 4:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC4CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC4CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 4 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC4CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC5 is Selected
        case 5:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC5CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC5CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 5 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC5CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC6 is Selected
        case 6:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC6CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC6CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 6 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC6CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC7 is Selected
        case 7:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC7CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC7CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 7 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC7CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC8 is Selected
        case 8:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC8CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC8CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 8 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC8CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC9 is Selected
        case 9:  EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC9CTL.bit.ACQPS    = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC9CTL.bit.CHSEL    = ADC_CH;     // ADC Channel 9 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC9CTL.bit.TRIGSEL  = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC10 is Selected
        case 10: EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC10CTL.bit.ACQPS   = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC10CTL.bit.CHSEL   = ADC_CH;     // ADC Channel 10 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC10CTL.bit.TRIGSEL = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC11 is Selected
        case 11: EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC11CTL.bit.ACQPS   = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC11CTL.bit.CHSEL   = ADC_CH;     // ADC Channel 11 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC11CTL.bit.TRIGSEL = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC12 is Selected
        case 12: EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC12CTL.bit.ACQPS   = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC12CTL.bit.CHSEL   = ADC_CH;     // ADC Channel 12 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC12CTL.bit.TRIGSEL = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC13 is Selected
        case 13: EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC13CTL.bit.ACQPS   = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC13CTL.bit.CHSEL   = ADC_CH;     // ADC Channel 13 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC13CTL.bit.TRIGSEL = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC14 is Selected
        case 14: EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC14CTL.bit.ACQPS   = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC14CTL.bit.CHSEL   = ADC_CH;     // ADC Channel 14 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC14CTL.bit.TRIGSEL = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // SOC15 is Selected
        case 15: EALLOW;                                                      // Register Protection Off
                 (*ADC_Define[ADC_REG]).ADCSOC15CTL.bit.ACQPS   = ACQ_PRD;    // Set Sample window
                 (*ADC_Define[ADC_REG]).ADCSOC15CTL.bit.CHSEL   = ADC_CH;     // ADC Channel 15 is Selected
                 (*ADC_Define[ADC_REG]).ADCSOC15CTL.bit.TRIGSEL = ADC_TRIG;   // Configure Trigger Source
                 EDIS;                                                        // Register Protection On
                 break;                                                       // Break to Get out of the Conditional Loop

        // Exceptional Case
        default: break;                                                       // Break to Get out of the Conditional Loop
    }
}

// Initialization of Post-Processing Block of ADC to Determine Major Fault Status
void Major_Faults_Check(unsigned int ADC_REG, unsigned int PP_BLOCK, unsigned int RESULT, // Define Variables to Get ADC Register, Post Processing Block for the ADC Result & ADC Result Selection
                        int UPP_LIM         , int LOW_LIM)                                // Define Variables to Get Current Upper Limit (Over-Current) & Current Lower Limit (Under-Current)
{
    switch(PP_BLOCK)
    {
        // ADC Post Processing Block 1
        case 1:  EALLOW;                                                        // Register Protection Off
                 // ADC PPB1 Config Register
                 (*ADC_Define[ADC_REG]).ADCPPB1CONFIG.bit.CONFIG     = RESULT;  // Select ADC Result to Process
                 (*ADC_Define[ADC_REG]).ADCPPB1CONFIG.bit.TWOSCOMPEN = RESET;   // Disable Conversion to 2's Complement
                 // ADC PPB1 Offset Calibration Register
                 (*ADC_Define[ADC_REG]).ADCPPB1OFFCAL.bit.OFFCAL     = RESET;   // Set Offset Calibration = Zero
                 // ADC PPB1 Offset Reference Register
                 (*ADC_Define[ADC_REG]).ADCPPB1OFFREF                = RESET;   // Set Offset Reference = Zero
                 // ADC PPB1 Trip High Register
                 (*ADC_Define[ADC_REG]).ADCPPB1TRIPHI.bit.LIMITHI    = UPP_LIM; // Set Voltage Trip Upper Limit
                 // ADC PPB1 Trip Low/Trigger Time Stamp Register
                 (*ADC_Define[ADC_REG]).ADCPPB1TRIPLO.bit.LIMITLO    = LOW_LIM; // Set Voltage Trip Lower Limit
                 EDIS;                                                          // Register Protection On
                 break;                                                         // Break to Get out of the Conditional Loop

        // ADC Post Processing Block 2
        case 2:  EALLOW;                                                        // Register Protection Off
                 // ADC PPB2 Config Register
                 (*ADC_Define[ADC_REG]).ADCPPB2CONFIG.bit.CONFIG     = RESULT;  // Select ADC Result to Process
                 (*ADC_Define[ADC_REG]).ADCPPB2CONFIG.bit.TWOSCOMPEN = RESET;   // Disable Conversion to 2's Complement
                 // ADC PPB2 Offset Calibration Register
                 (*ADC_Define[ADC_REG]).ADCPPB2OFFCAL.bit.OFFCAL     = RESET;   // Set Offset Calibration = Zero
                 // ADC PPB2 Offset Reference Register
                 (*ADC_Define[ADC_REG]).ADCPPB2OFFREF                = RESET;   // Set Offset Reference = Zero
                 // ADC PPB2 Trip High Register
                 (*ADC_Define[ADC_REG]).ADCPPB2TRIPHI.bit.LIMITHI    = UPP_LIM; // Set Voltage Trip Upper Limit
                 // ADC PPB2 Trip Low/Trigger Time Stamp Register
                 (*ADC_Define[ADC_REG]).ADCPPB2TRIPLO.bit.LIMITLO    = LOW_LIM; // Set Voltage Trip Lower Limit
                 EDIS;                                                          // Register Protection On
                 break;                                                         // Break to Get out of the Conditional Loop

        // ADC Post Processing Block 3
        case 3:  EALLOW;                                                        // Register Protection Off
                 // ADC PPB3 Config Register
                 (*ADC_Define[ADC_REG]).ADCPPB3CONFIG.bit.CONFIG     = RESULT;  // Select ADC Result to Process
                 (*ADC_Define[ADC_REG]).ADCPPB3CONFIG.bit.TWOSCOMPEN = RESET;   // Disable Conversion to 2's Complement
                 // ADC PPB3 Offset Calibration Register
                 (*ADC_Define[ADC_REG]).ADCPPB3OFFCAL.bit.OFFCAL     = RESET;   // Set Offset Calibration = Zero
                 // ADC PPB3 Offset Reference Register
                 (*ADC_Define[ADC_REG]).ADCPPB3OFFREF                = RESET;   // Set Offset Reference = Zero
                 // ADC PPB3 Trip High Register
                 (*ADC_Define[ADC_REG]).ADCPPB3TRIPHI.bit.LIMITHI    = UPP_LIM; // Set Voltage Trip Upper Limit
                 // ADC PPB3 Trip Low/Trigger Time Stamp Register
                 (*ADC_Define[ADC_REG]).ADCPPB3TRIPLO.bit.LIMITLO    = LOW_LIM; // Set Voltage Trip Lower Limit
                 EDIS;                                                          // Register Protection On
                 break;                                                         // Break to Get out of the Conditional Loop

        // ADC Post Processing Block 4
        case 4:  EALLOW;                                                        // Register Protection Off
                 // ADC PPB4 Config Register
                 (*ADC_Define[ADC_REG]).ADCPPB4CONFIG.bit.CONFIG     = RESULT;  // Select ADC Result to Process
                 (*ADC_Define[ADC_REG]).ADCPPB4CONFIG.bit.TWOSCOMPEN = RESET;   // Disable Conversion to 2's Complement
                 // ADC PPB4 Offset Calibration Register
                 (*ADC_Define[ADC_REG]).ADCPPB4OFFCAL.bit.OFFCAL     = RESET;   // Set Offset Calibration = Zero
                 // ADC PPB4 Offset Reference Register
                 (*ADC_Define[ADC_REG]).ADCPPB4OFFREF                = RESET;   // Set Offset Reference = Zero
                 // ADC PPB4 Trip High Register
                 (*ADC_Define[ADC_REG]).ADCPPB4TRIPHI.bit.LIMITHI    = UPP_LIM; // Set Voltage Trip Upper Limit
                 // ADC PPB4 Trip Low/Trigger Time Stamp Register
                 (*ADC_Define[ADC_REG]).ADCPPB4TRIPLO.bit.LIMITLO    = LOW_LIM; // Set Voltage Trip Lower Limit
                 EDIS;                                                          // Register Protection On
                 break;                                                         // Break to Get out of the Conditional Loop

        // Exceptional Case
        default: break;                                                         // Break to Get out of the Conditional Loop
    }
}

// Initialization of EPWM Trip Zone to Disable EPWM During Major Faults
void Trip_Initialization(unsigned int EPWM_CH) // Define Variable to Get EPWM Channel Selection
{
    EALLOW;                                           // Register Protection Off
    (*ePWM[EPWM_CH]).TZSEL.bit.DCAEVT1 = TZ_ENABLE;   // Enable Trip Event 01 for EPWMxA
    (*ePWM[EPWM_CH]).TZSEL.bit.DCBEVT1 = TZ_ENABLE;   // Enable Trip Event 01 for EPWMxB
    (*ePWM[EPWM_CH]).TZSEL.bit.OSHT1   = TZ_ENABLE;   // Select One-Shot Trip Type
    (*ePWM[EPWM_CH]).TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // Define EPWMxA Behavior During Trip Event 01 as Latched to Low
    (*ePWM[EPWM_CH]).TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // Define EPWMxB Behavior During Trip Event 01 as Latched to Low
    (*ePWM[EPWM_CH]).TZEINT.bit.OST    = TZ_ENABLE;   // Define Interrupt Generation During Trip Event
    EDIS;                                             // Register Protection On
}

__interrupt void EPWM_Trip_ISR()
{
//    if((*ADC_Define[ADC_ADCD]).ADCEVTSTAT.bit.PPB1TRIPHI == true)
//    {
//        Error_Count = Error_Count + 1;
//    }
//
//
//    (*ADC_Define[ADC_ADCD]).ADCEVTSTAT.bit.PPB1TRIPHI;
}

// End of File
