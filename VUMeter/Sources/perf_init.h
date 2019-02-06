/***********************************************************
edma_init contains the functions that initialize the following
peripheral devices.

ADC
PDB
PIT - For performance measurement.
************************************************************/

#include "fsl_device_registers.h"
#include "fsl_adc16_driver.h"
#include "fsl_pdb_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_port_hal.h"
#include "fsl_smc_hal.h"
#include "fsl_uart_driver.h"

#include "constants.h"

void init_adc(uint32_t instance);

void config_PDB(void);

void initPIT(void);

//void initUART(void);

extern void UART_DRV_IRQHandler(uint32_t instance);
