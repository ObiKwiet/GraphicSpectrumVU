#include "perf_init.h"


// This configures channel 0 on adc(instance).
// The instance refers to the actual ADC hardware module, either ADC0 or ADC1
// on the K64F.
// The channel is the pin that is connected to the ADC.
void init_adc(uint32_t instance) {

	/******** Initialize Data Structures *********/

    //Config
	adc16_converter_config_t userConvConfig;
	adc16_chn_config_t userChnConfig;

    //Calibration
    adc16_calibration_param_t userCalConfig;

    /********* Configure ADC **********/
    // Maximum conversion clock frequency is 18MHz for 12bit, 12MHz for 16bit. Bus frequency is 60.
    // With a clock divider of 4, the ADCK is 15MHz.

    // Set Converter Configuration Values
    userConvConfig.lowPowerEnable = false;
    userConvConfig.clkDividerMode = kAdc16ClkDividerOf4; //
    userConvConfig.longSampleTimeEnable = false;
    userConvConfig.resolution = kAdc16ResolutionBitOf12or13; //12bit for Single Ended Conversion
    userConvConfig.clkSrc = kAdc16ClkSrcOfBusClk;
    userConvConfig.asyncClkEnable = false;
    userConvConfig.highSpeedEnable = true;
    userConvConfig.longSampleCycleMode = kAdc16LongSampleCycleOf24;
    userConvConfig.hwTriggerEnable = true;
    userConvConfig.refVoltSrc = kAdc16RefVoltSrcOfVref;
    userConvConfig.continuousConvEnable = false;
    userConvConfig.dmaEnable = true;

    //Initialize ADC
    ADC16_DRV_Init(instance, &userConvConfig);

    //Calibrate ADC - This are not the optimal settings. To improve, see AN3949
    ADC16_DRV_GetAutoCalibrationParam(instance, &userCalConfig);
    ADC16_DRV_SetCalibrationParam(instance, &userCalConfig);

    //Set Channel Configuration Values
    userChnConfig.chnIdx = kAdc16Chn1d; // DAD1 - ADC0_DP1
    userChnConfig.convCompletedIntEnable = false; // Enable Interrupt on Conversion
    userChnConfig.diffConvEnable = false;

    // Apply Channel Configuration ADC(instance)_SC1A. The 0 selects A.
    ADC16_DRV_ConfigConvChn(instance, 0, &userChnConfig);
}


void config_PDB(void) {

    uint32_t busClock, modValue, countRate;
    pdb_timer_config_t PdbTimerConfig;
    pdb_adc_pretrigger_config_t pdbAdcTriggerConfig;

    //Basic PDB configuration
    PdbTimerConfig.loadValueMode = kPdbLoadValueImmediately;
    PdbTimerConfig.seqErrIntEnable = false;
    PdbTimerConfig.clkPreDiv = kPdbClkPreDivBy1;  //Adjust these multipliers to accommodate modValue
    PdbTimerConfig.clkPreMultFactor = kPdbClkPreMultFactorAs1;
    PdbTimerConfig.triggerInput = kPdbSoftTrigger;
    PdbTimerConfig.continuousModeEnable = true;
    PdbTimerConfig.dmaEnable = false;
    PdbTimerConfig.intEnable = false;
    PdbTimerConfig.seqErrIntEnable = true;
    PDB_DRV_Init(0U, &PdbTimerConfig);

    pdbAdcTriggerConfig.adcPreTriggerIdx = 0U;
    pdbAdcTriggerConfig.preTriggerEnable = true;
    pdbAdcTriggerConfig.preTriggerOutputEnable = true;
    pdbAdcTriggerConfig.preTriggerBackToBackEnable = false;
    PDB_DRV_ConfigAdcPreTrigger(0U, 0U, &pdbAdcTriggerConfig);

    // CLOCK_SYS_GetBusClockFreq(); would be nice here, but it's broken
    // under this clock configuration and it returns 0. I asked about it
    // on the community forum, but it was ignored.
    busClock = 60000000;

    // Get count rate in Hz:
    // countRate = busClock/PdbTimerConfig.clkPreDiv/PdbTimerConfig.clkPreMultFactor;
    // Except you have to manually interpret the values .clkProDiv and clkPreMultFactor because they are coded
    // to a 4 bit word. In this cause busClock/1/1 = busClock.
    countRate = busClock;

    // Get Modulus value for sampling frequency (Must fit in 16bit field)
    modValue = countRate/SAMPLEFREQ;

    PDB_DRV_SetTimerModulusValue(0U, modValue);
    PDB_DRV_SetAdcPreTriggerDelayValue(0U, 0U, 0U, modValue/2);
    PDB_DRV_LoadValuesCmd(0U);
    // Hardware should default to this, but it is added for completeness.
    SIM_HAL_SetAdcAlternativeTriggerCmd(SIM_BASE, 0U, false);

    //Start
    PDB_DRV_SoftTriggerCmd(0);
}


/*******************************************************************************
initPIT - Start the PIT timer. This is to measure DSP performance and verify
timing.
PIT Clock - Bus Clock = 60MHz
1 uS = 60 Clock Cycles.
Clock 1 Count Clocks, Clock 2 counts Clock 1
*******************************************************************************/
void initPIT(void) {

	CLOCK_SYS_EnablePitClock(0);
    // Timer 2 - Counts down uS
	PIT_MCR = 0x00;
    PIT_LDVAL2 = 0xFFFFFFFF; // setup Timer 2 for 2^32 counts
    PIT_TCTRL2 |= PIT_TCTRL_CHN(1); // chain Timer 2 to Timer 1
    PIT_TCTRL2 |= PIT_MCR_FRZ(1); // Stop on debug
    PIT_TCTRL2 |= PIT_TCTRL_TEN(1); // start Timer 2
    // Timer 1 - Counts clock cycles at 60MHz
    PIT_LDVAL1 = 59; // setup Timer 1 for 60 cycles
    PIT_TCTRL1 |= PIT_MCR_FRZ(1); // Stop on debug
    PIT_TCTRL1 = PIT_TCTRL_TEN(1); // start Timer 1
    // Turn on PIT

}




