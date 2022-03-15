// Start of File
#include "F28x_Project.h"      // Include Device Header File and Examples
#include "Control_Variables.h" // Include Control Variables File

// SMPS Control Loop Functions
float Low_Pass_Filter();   // Declare function to Filer out Noise from Acquired Feedback from ADC
float Control_Loop();      // Declare function to Execute Control Loop
void DAC_Initialization(); // Declare function to Configure DAC to send Current Reference from Master to Slaves

// SMPS Interrupts
__interrupt void EPWM_Compare_ISR(); // Define Interrupt Function for EPWM1 & EPWM2
__interrupt void EPWM_Trip_ISR();    // Define Interrupt Function to trip EPWM1 & EPWM2

// Initialization of EPWM Channels
void EPWM_Initialization(unsigned int EPWM_CH, unsigned int SYNC, unsigned int TB_PRD, // Define Variables to Get EPWM Channel Selection, Synchronization Source & Time Base Period
                         unsigned int PHS_OFF, int CMP_A        , int CMP_B)           // Define Variables to Get Phase Offset, Comparator A & Comparator B
{
    EALLOW;                                         // Register Protection Off
    // Peripheral Clock Gating Registers
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = CLK_DISABLE; // Disable TBCLK within the ePWM.
    EDIS;                                           // Register Protection On

    // EALLOW;
    // Configure EPWM Channel A & B
    // Time Base Control Register
    (*ePWM[EPWM_CH]).TBCTL.bit.SYNCOSEL   = SYNC;            // Sync EPWM1 to Down-stream (CTR = ZRO)
    if(SYNC == TB_SYNC_IN)
        (*ePWM[EPWM_CH]).TBCTL.bit.PHSEN  = TB_ENABLE;       // Enable Phase Offset Register
    else
        (*ePWM[EPWM_CH]).TBCTL.bit.PHSEN  = TB_DISABLE;      // Disable Phase Offset Register
    (*ePWM[EPWM_CH]).TBCTL.bit.PRDLD      = TB_IMMEDIATE;    // Set Immediate Load
    (*ePWM[EPWM_CH]).TBCTL.bit.PHSDIR     = TB_UP;           // Count Up After The Synchronization Event
    // Time Base Period Register
    (*ePWM[EPWM_CH]).TBPRD                = TB_PRD;          // Update the Counter Peak
    // Time Base Phase Offset Register
    (*ePWM[EPWM_CH]).TBPHS.bit.TBPHS      = PHS_OFF;         // Enable Phase Offset
    // Time Base Counter Register
    (*ePWM[EPWM_CH]).TBCTR                = TB_DISABLE;      // Enable Time Base Counter
    // Time Base Control Register
    (*ePWM[EPWM_CH]).TBCTL.bit.HSPCLKDIV  = TB_DIV1;         // Divide Clock By 1
    (*ePWM[EPWM_CH]).TBCTL.bit.CLKDIV     = TB_DIV1;         // Divide Clock By 1
    (*ePWM[EPWM_CH]).TBCTL.bit.CTRMODE    = TB_COUNT_UP;     // Up-Down Count Mode
    // Counter Compare Control Register
    (*ePWM[EPWM_CH]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // Load On CTR = ZRO and CTR = PRD
    (*ePWM[EPWM_CH]).CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD; // Load On CTR = ZRO and CTR = PRD
    // Counter Compare A
    (*ePWM[EPWM_CH]).CMPA.bit.CMPA        = CMP_A;           // Load Counter Compare A
    // Counter Compare B
    (*ePWM[EPWM_CH]).CMPB.bit.CMPB        = CMP_B;           // Load Counter Compare B
    // Action Qualifier Control Register For Output A
    (*ePWM[EPWM_CH]).AQCTLA.all           = AQ_NO_ACTION;    // Reset AQCTLA Register
    (*ePWM[EPWM_CH]).AQCTLA.bit.ZRO       = AQ_SET;          // Set at CTR = ZERO
    (*ePWM[EPWM_CH]).AQCTLA.bit.CAU       = AQ_CLEAR;        // Clear at CTR = CMPA (Up Direction)
    // Action Qualifier Control Register For Output B
    (*ePWM[EPWM_CH]).AQCTLB.all           = AQ_NO_ACTION;    // Reset AQCTLB Register
    (*ePWM[EPWM_CH]).AQCTLB.bit.ZRO       = AQ_CLEAR;        // Clear at CTR = ZERO
    (*ePWM[EPWM_CH]).AQCTLB.bit.CBU       = AQ_SET;          // Set at CTR = CMPA (Up Direction)
    // Event Trigger Selection Register
    (*ePWM[EPWM_CH]).ETSEL.bit.INTEN      = ET_ENABLE;       // Enable EPWM1 Interrupt Generation
    (*ePWM[EPWM_CH]).ETSEL.bit.INTSEL     = ET_CTR_PRDZERO;  // Enable Event when CTR = ZRO or CTR = PRD
    // Event Trigger Pre-Scale Register
    (*ePWM[EPWM_CH]).ETPS.bit.INTPRD      = ET_1ST;          // Generate Interrupt on First Event
    // EDIS;

    EALLOW;                                        // Register Protection Off
    // Peripheral Clock Gating Registers
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = CLK_ENABLE; // Enable TBCLK within the ePWM
//    CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = CLK_ENABLE; // Enable CPU Timer 0 Clock
    EDIS;                                          // Register Protection On

    // Configure the DAC channels
    // DAC Initialization Current Reference from Master
    DAC_Initialization(DAC_DACA, (*Control[CURRENT]).REFERENCE); // Set DAC_REG = DAC_DACA, DAC_VAL = (*Control[CURRENT]).REFERENCE
}

// Initialization of EPWM Interrupt
void EPWM_Interrupt_Initialization(unsigned int EPWM_CH)
{
    // Enable global Interrupts and higher priority real-time debug events
    EALLOW;
    IER |= M_INT3; // Enable Group 3 Interrupt (for EPWM Interrupt)
    EDIS;

    EALLOW;
    switch(EPWM_CH)
    {
      // EPWM1 Interrupt Initializations
      case 1:  PieVectTable.EPWM1_INT        = &EPWM_Compare_ISR;  // Introduce EPWM1 Interrupt for EPWM1 Channel
               PieCtrlRegs.PIEIER3.bit.INTx1 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM2 Interrupt Initializations
      case 2:  PieVectTable.EPWM2_INT        = &EPWM_Compare_ISR;  // Introduce EPWM2 Interrupt for EPWM2 Channel
               PieCtrlRegs.PIEIER3.bit.INTx2 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM3 Interrupt Initializations
      case 3:  PieVectTable.EPWM3_INT        = &EPWM_Compare_ISR;  // Introduce EPWM3 Interrupt for EPWM3 Channel
               PieCtrlRegs.PIEIER3.bit.INTx3 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM4 Interrupt Initializations
      case 4:  PieVectTable.EPWM4_INT        = &EPWM_Compare_ISR;  // Introduce EPWM4 Interrupt for EPWM4 Channel
               PieCtrlRegs.PIEIER3.bit.INTx4 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM5 Interrupt Initializations
      case 5:  PieVectTable.EPWM5_INT        = &EPWM_Compare_ISR;  // Introduce EPWM5 Interrupt for EPWM5 Channel
               PieCtrlRegs.PIEIER3.bit.INTx5 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM6 Interrupt Initializations
      case 6:  PieVectTable.EPWM6_INT        = &EPWM_Compare_ISR;  // Introduce EPWM6 Interrupt for EPWM6 Channel
               PieCtrlRegs.PIEIER3.bit.INTx6 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM7 Interrupt Initializations
      case 7:  PieVectTable.EPWM7_INT        = &EPWM_Compare_ISR;  // Introduce EPWM7 Interrupt for EPWM7 Channel
               PieCtrlRegs.PIEIER3.bit.INTx7 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM8 Interrupt Initializations
      case 8:  PieVectTable.EPWM8_INT        = &EPWM_Compare_ISR;  // Introduce EPWM8 Interrupt for EPWM8 Channel
               PieCtrlRegs.PIEIER3.bit.INTx8 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM9 Interrupt Initializations
      case 9:  PieVectTable.EPWM9_INT        = &EPWM_Compare_ISR;  // Introduce EPWM9 Interrupt for EPWM9 Channel
               PieCtrlRegs.PIEIER3.bit.INTx9 = ET_ENABLE;          // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM10 Interrupt Initializations
      case 10: PieVectTable.EPWM10_INT        = &EPWM_Compare_ISR; // Introduce EPWM10 Interrupt for EPWM10 Channel
               PieCtrlRegs.PIEIER3.bit.INTx10 = ET_ENABLE;         // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM11 Interrupt Initializations
      case 11: PieVectTable.EPWM11_INT        = &EPWM_Compare_ISR; // Introduce EPWM11 Interrupt for EPWM11 Channel
               PieCtrlRegs.PIEIER3.bit.INTx11 = ET_ENABLE;         // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // EPWM12 Interrupt Initializations
      case 12: PieVectTable.EPWM12_INT        = &EPWM_Compare_ISR; // Introduce EPWM12 Interrupt for EPWM12 Channel
               PieCtrlRegs.PIEIER3.bit.INTx12 = ET_ENABLE;         // Enable PIE Group 3, Block 1
               break;                                              // Break to Get out of the Conditional Loop
      // Exceptional Case
      default: break;                                              // Break to Get out of the Conditional Loop
    }
    EDIS;
}

// Initialization of EPWM Trip Zone Interrupt
void EPWM_Trip_Zone_Interrupt_Initialization(unsigned int EPWM_CH)
{
    // Enable global Interrupts and higher priority real-time debug events
    EALLOW;
    IER |= M_INT2; // Enable Group 2 Interrupt (for EPWM Trip Zone Interrupt)
    EDIS;

    EALLOW;
    switch(EPWM_CH)
    {
      // EPWM1 Trip Zone Interrupt Initializations
      case 1:  PieVectTable.EPWM1_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM1 Trip Zone Interrupt for EPWM1 Channel
               PieCtrlRegs.PIEIER2.bit.INTx1 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM2 Trip Zone Interrupt Initializations
      case 2:  PieVectTable.EPWM2_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM2 Trip Zone Interrupt for EPWM2  Channel
               PieCtrlRegs.PIEIER2.bit.INTx2 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM3 Trip Zone Interrupt Initializations
      case 3:  PieVectTable.EPWM3_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM3 Trip Zone Interrupt for EPWM3 Channel
               PieCtrlRegs.PIEIER2.bit.INTx3 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM4 Trip Zone Interrupt Initializations
      case 4:  PieVectTable.EPWM4_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM4 Trip Zone Interrupt for EPWM4 Channel
               PieCtrlRegs.PIEIER2.bit.INTx4 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM5 Trip Zone Interrupt Initializations
      case 5:  PieVectTable.EPWM5_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM5 Trip Zone Interrupt for EPWM5 Channel
               PieCtrlRegs.PIEIER2.bit.INTx5 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM6 Trip Zone Interrupt Initializations
      case 6:  PieVectTable.EPWM6_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM6 Trip Zone Interrupt for EPWM6 Channel
               PieCtrlRegs.PIEIER2.bit.INTx6 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM7 Trip Zone Interrupt Initializations
      case 7:  PieVectTable.EPWM7_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM7 Trip Zone Interrupt for EPWM7 Channel
               PieCtrlRegs.PIEIER2.bit.INTx7 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM8 Trip Zone Interrupt Initializations
      case 8:  PieVectTable.EPWM8_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM8 Trip Zone Interrupt for EPWM8 Channel
               PieCtrlRegs.PIEIER2.bit.INTx8 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM9 Trip Zone Interrupt Initializations
      case 9:  PieVectTable.EPWM9_TZ_INT     = &EPWM_Trip_ISR;  // Introduce EPWM9 Trip Zone Interrupt for EPWM9 Channel
               PieCtrlRegs.PIEIER2.bit.INTx9 = ET_ENABLE;       // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM10 Trip Zone Interrupt Initializations
      case 10: PieVectTable.EPWM10_TZ_INT     = &EPWM_Trip_ISR; // Introduce EPWM10 Trip Zone Interrupt for EPWM10 Channel
               PieCtrlRegs.PIEIER2.bit.INTx10 = ET_ENABLE;      // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM11 Trip Zone Interrupt Initializations
      case 11: PieVectTable.EPWM11_TZ_INT     = &EPWM_Trip_ISR; // Introduce EPWM11 Trip Zone Interrupt for EPWM11 Channel
               PieCtrlRegs.PIEIER2.bit.INTx11 = ET_ENABLE;      // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // EPWM12 Trip Zone Interrupt Initializations
      case 12: PieVectTable.EPWM12_TZ_INT     = &EPWM_Trip_ISR; // Introduce EPWM12 Trip Zone Interrupt for EPWM12 Channel
               PieCtrlRegs.PIEIER2.bit.INTx12 = ET_ENABLE;      // Enable PIE Group 2, Block 1
               break;                                           // Break to Get out of the Conditional Loop
      // Exceptional Case
      default: break;                                           // Break to Get out of the Conditional Loop
    }
    EDIS;
}

// Define Control Loop Logic
void Close_Loop(void)
{
    // Determine Status of Operation of the System
    if(INITIAL_CONDITION == true)                                 // Check Condition for Soft Start
    {
        DUTY_VALUE = DUTY_VALUE + SOFT_START_TIME_PERIOD;         // Increment Duty Cycle of Top Switches
        DUTY_FIFTY = SMPS_PERIOD - DUTY_VALUE;                    // Increment Duty Cycle of Bottom Switches
        INITIAL_CONDITION = (DUTY_VALUE >= PWMxB) ? false : true; // Evaluate Initial Condition of the Module
    }
    else                                                          // Condition on Completion of Soft Start
    {
        (*Control[VOLTAGE]).REFERENCE = VOLTAGE_REFERENCE;        // Load the Voltage Reference for the Control Loop
        DUTY_FIFTY = PWMxB;                                       // Set the Duty Cycle of Bottom Switches to a Constant 50% of the Time period
    }

    // Determine the Priority (Master or Slave) Status of the System
    if(MASTER_STATUS == true)                                                                                  // Check Condition for Master Status of the Module
    {
        // Voltage Control Loop to Evaluate the Current Reference
        (*Control[VOLTAGE]).FEEDBACK  = Low_Pass_Filter(V_DC_OUT, (*ADC_Result[ADC_ADCD]).ADCRESULT2,          // Get the Voltage Feedback form the ADC
                                                        CURRENT_FEEDBACK_MAXIMUM, CURRENT_FEEDBACK_MINIMUM);   // Get the Voltage Feedback Upper Limit & Voltage Feedback Lower Limit
        (*Control[CURRENT]).REFERENCE = Control_Loop(VOLTAGE);                                                 // Call the Function Voltage Loop to Calculate the Current Reference
    }
    else                                                                                                       // Condition for Slave Status of the Module
    {
        // Acquire Current Reference form the Master Module via ADC
        (*Control[CURRENT]).REFERENCE = Low_Pass_Filter(I_REF_MT, (*ADC_Result[ADC_ADCD]).ADCRESULT3,          // Get the Current Reference form the Master Module via ADC
                                                        CURRENT_REFERENCE_MAXIMUM, CURRENT_REFERENCE_MINIMUM); // Get the Current Reference Upper Limit & Current Reference Lower Limit
    }

    // Current Control Loop to Evaluate Duty Cycle
    (*Control[CURRENT]).FEEDBACK = Low_Pass_Filter(I_DC_OUT, (*ADC_Result[ADC_ADCD]).ADCRESULT2,        // Get the Current Feedback form the ADC
                                                   CURRENT_FEEDBACK_MAXIMUM, CURRENT_FEEDBACK_MINIMUM); // Get the Current Feedback Upper Limit & Current Feedback Lower Limit
    DUTY_VALUE = Control_Loop(CURRENT);                                                                 // Call the Function Current Loop to Calculate the Duty Cycle of the Top Switches
}

// Define Function to Perform Low Pass Filtering Operation
float Low_Pass_Filter(unsigned int FLT_PARAM, int SIGNAL,  // Define Variables to Get Filter Parameter & Input Signal
                      int LPF_MAX           , int LPF_MIN) // Define Variables to Get Low Pass Filter Maximum Limit & Low Pass Filter Minimum Limit
{
    // Actual Low Pass Filter Execution
    (*LP_FILTER[FLT_PARAM]).LPF_PRESENT_OUTPUT  = (*LP_FILTER[FLT_PARAM]).LPF_COMP_SAMPLING_PERIOD * (*LP_FILTER[FLT_PARAM]).LPF_PREVIOUS_OUTPUT
                                                   + (*LP_FILTER[FLT_PARAM]).LPF_SAMPLING_PERIOD * SIGNAL;                                      // Calculate the Low Pass Filter Filter Output
    (*LP_FILTER[FLT_PARAM]).LPF_PREVIOUS_OUTPUT = (*LP_FILTER[FLT_PARAM]).LPF_PRESENT_OUTPUT;                                                    // Save Present Filter Output for Calculation of Next Cycle

    // Low Pass Filter Clamps
    (*LP_FILTER[FLT_PARAM]).LPF_RETURN_OUTPUT   = ((*LP_FILTER[FLT_PARAM]).LPF_PRESENT_OUTPUT > LPF_MAX)
                                                   ? LPF_MAX : (*LP_FILTER[FLT_PARAM]).LPF_PRESENT_OUTPUT;                                       // Limit LPF Output to Upper Value if it Rises Beyond the Upper Value
    (*LP_FILTER[FLT_PARAM]).LPF_RETURN_OUTPUT   = ((*LP_FILTER[FLT_PARAM]).LPF_PRESENT_OUTPUT < LPF_MAX)
                                                   ? LPF_MIN : (*LP_FILTER[FLT_PARAM]).LPF_PRESENT_OUTPUT;                                       // Limit LPF Output to Lower Value if it Falls Below the Lower Value

    // Return the Low Pass Filter Output
    return (*LP_FILTER[FLT_PARAM]).LPF_PRESENT_OUTPUT; // Return the Value of Filter Output
}

// Define Function to Perform Close Loop Operation
float Control_Loop(unsigned int LOOP_PARAM) // Define Variable to Get Control Loop Parameter
{
    // Generate Error Signal
    (*Control[LOOP_PARAM]).ERROR             = (*Control[LOOP_PARAM]).REFERENCE - (*Control[LOOP_PARAM]).FEEDBACK;                                      // Calculate Error Signal
    // Operate Proportional Path
    (*Control[LOOP_PARAM]).PROPORTIONAL      = (*Control[LOOP_PARAM]).Kp * (*Control[LOOP_PARAM]).ERROR;                                                // Calculate Proportional Path
    // Operate Integral Path
    (*Control[LOOP_PARAM]).INTEGRAL          = (*Control[LOOP_PARAM]).PREVIOUS_INTEGRAL + (*Control[LOOP_PARAM]).Ki * (*Control[LOOP_PARAM]).ERROR;     // Calculate Integral Path
    (*Control[LOOP_PARAM]).PREVIOUS_INTEGRAL = (*Control[LOOP_PARAM]).INTEGRAL;                                                                         // Save Present Integral Output for Integral Calculation of Next Cycle
    // Complete PI Loop
    (*Control[LOOP_PARAM]).PI_OUTPUT         = (*Control[LOOP_PARAM]).PROPORTIONAL + (*Control[LOOP_PARAM]).INTEGRAL;                                   // Sum of Proportional and Integral Path

    // PI Output Clamp
    (*Control[LOOP_PARAM]).PI_OUTPUT         = ((*Control[LOOP_PARAM]).PI_OUTPUT > (*Control[LOOP_PARAM]).PI_MAXIMUM)
                                                ? (unsigned int)((*Control[LOOP_PARAM]).PI_MAXIMUM) : (unsigned int)((*Control[LOOP_PARAM]).PI_OUTPUT); // Limit PI Output to Upper Value if it Rises Beyond the Upper Value
    (*Control[LOOP_PARAM]).PI_OUTPUT         = ((*Control[LOOP_PARAM]).PI_OUTPUT < (*Control[LOOP_PARAM]).PI_MINIMUM)
                                                ? (unsigned int)((*Control[LOOP_PARAM]).PI_MINIMUM) : (unsigned int)((*Control[LOOP_PARAM]).PI_OUTPUT); // Limit PI Output to Lower Value if it Falls Below the Lower Value

    // Return the PI Output
    return (*Control[LOOP_PARAM]).PI_OUTPUT; // Return the Value of PI Output
}

// Load and Reset Low-Pass Filter Parameters
void Low_Pass_Filter_Parameters_Initialization(unsigned int FLT_PARAM, float TS, float INV_TS) // Define Variables to Get Low-Pass Filter Parameter, Low-Pass Filter Sampling Time Period & (1 - Low-Pass Filter Sampling Time Period)
{
    // Load and Reset Respective Low Pass Filter Parameters
    (*LP_FILTER[FLT_PARAM]).LPF_SAMPLING_PERIOD      = TS;     // Load Low Pass Filter Sampling Time Period
    (*LP_FILTER[FLT_PARAM]).LPF_COMP_SAMPLING_PERIOD = INV_TS; // Load Low Pass Filter (Sampling Time Period - 1)
    (*LP_FILTER[FLT_PARAM]).LPF_PRESENT_OUTPUT       = RESET;  // Reset the Present Output of Low Pass Filter to Zero
    (*LP_FILTER[FLT_PARAM]).LPF_PREVIOUS_OUTPUT      = RESET;  // Reset the Previous Output of Low Pass Filter to Zero
    (*LP_FILTER[FLT_PARAM]).LPF_PRESENT_INPUT        = RESET;  // Reset the Present Input of Low Pass Filter to Zero
}

// Load and Reset Control Loop Parameters
void Control_Parameters_Initialization(unsigned int LOOP_PARAM, int REF,              // Define Variables to Get Control Loop Parameter
                                       unsigned int FBK_MAX   , unsigned int FBK_MIN, // Define Variables to Get Feedback Maximum Limit & Feedback Minimum Limit
                                       unsigned int REF_MAX   , unsigned int REF_MIN, // Define Variables to Get Reference Maximum Limit & Reference Minimum Limit
                                       unsigned int PI_MAX    , unsigned int PI_MIN,  // Define Variables to Get PI Maximum Limit & PI Minimum Limit
                                       float Kp_CNST          , float Ki_CNST)        // Define Variables to Get Kp Constant & Ki Constant
{
    // Load and Reset Respective Control Loop Parameters
    (*Control[LOOP_PARAM]).REFERENCE         = REF;     // Load the Reference Value to Evaluate Error Signal
    (*Control[LOOP_PARAM]).REFERENCE_MAXIMUM = REF_MAX; // Load the Maximum Limit for the Reference Signal
    (*Control[LOOP_PARAM]).REFERENCE_MINIMUM = REF_MIN; // Load the Minimum Limit for the Reference Signal
    (*Control[LOOP_PARAM]).FEEDBACK          = FBK_MIN; // Reset the Feedback Signal to Zero
    (*Control[LOOP_PARAM]).FEEDBACK_MAXIMUM  = FBK_MAX; // Load the Maximum Limit for the Feedback Signal
    (*Control[LOOP_PARAM]).FEEDBACK_MINIMUM  = FBK_MIN; // Load the Minimum Limit for the Feedback Signal
    (*Control[LOOP_PARAM]).ERROR             = RESET;   // Reset the Error Signal to Zero
    (*Control[LOOP_PARAM]).Kp                = Kp_CNST; // Load Kp Constant (Proportional Constant)
    (*Control[LOOP_PARAM]).Ki                = Ki_CNST; // Load Ki Constant (Integral Constant)
    (*Control[LOOP_PARAM]).PROPORTIONAL      = RESET;   // Reset the Proportional Output to Zero
    (*Control[LOOP_PARAM]).INTEGRAL          = RESET;   // Reset the Integral Output to Zero
    (*Control[LOOP_PARAM]).PREVIOUS_INTEGRAL = RESET;   // Reset the Previous Integral Output to Zero
    (*Control[LOOP_PARAM]).PI_OUTPUT         = RESET;   // Reset the PI Output to Zero
    (*Control[LOOP_PARAM]).PI_MAXIMUM        = PI_MAX;  // Load the Maximum Limit of the PI Output Signal
    (*Control[LOOP_PARAM]).PI_MINIMUM        = PI_MIN;  // Load the Minimum Limit of the PI Output Signal
}

// Initialization of DAC
void DAC_Initialization(unsigned int DAC_REG, int DAC_VAL) // Define Variables to Get DAC Register & DAC Value
{
    (*DAC_Define[DAC_REG]).DACCTL.bit.LOADMODE   = SYSCLK;     // Configure the DAC Channel to Emit Output at Every System Clock Cycle
    (*DAC_Define[DAC_REG]).DACCTL.bit.DACREFSEL  = VREF;       // Configure the Biasing of the DAC Channel
    (*DAC_Define[DAC_REG]).DACOUTEN.bit.DACOUTEN = DAC_ENABLE; // Enable the DAC Output
    (*DAC_Define[DAC_REG]).DACVALS.bit.DACVALS   = DAC_VAL;    // Load to DAC Value to be Transmitted
}

__interrupt void EPWM_Compare_ISR()
{
    // Counter Compare A
    (*ePWM[SMPS_ZERO]).CMPA.bit.CMPA = DUTY_VALUE;           // Load Counter Compare A
    (*ePWM[SMPS_HALF]).CMPA.bit.CMPA = DUTY_VALUE;           // Load Counter Compare A
    (*ePWM[SMPS_ZERO]).CMPB.bit.CMPB = DUTY_FIFTY;           // Load Counter Compare B
    (*ePWM[SMPS_HALF]).CMPB.bit.CMPB = DUTY_FIFTY;           // Load Counter Compare B
    // EPWM1 Interrupt Acknowledgment
    PieCtrlRegs.PIEACK.bit.ACK3      = AKNOWLEDGE_INTERRUPT; // Acknowledge EPWM1 Interrupt
    // Event Trigger Clear Register
    (*ePWM[SMPS_ZERO]).ETCLR.bit.INT = FLAG_CLEAR;           // Clear Flag Bit
    (*ePWM[SMPS_HALF]).ETCLR.bit.INT = FLAG_CLEAR;           // Clear Flag Bit
}

// End of File
