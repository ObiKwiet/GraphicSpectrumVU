/***********************************************************
edma_init contains the functions and data structures that handle
the DMA transfer of the audio samples.

A triple buffering system is used. Each new sample is transferred into
lower half of the "Lower" buffer, and the upper half of the "Upper". The "Active"
contained samples that will be processed next, and is not modified. When the
Upper buffer fills, and the Lower buffer fills halfway, and interrupt makes the
"Upper" buffer "Active", the "Lower" buffer "Upper", and the "Active" buffer is
overwritten as the new "Lower" buffer. This mean that the first half of each new
dataset contains the data from the upper half of the last data set.

************************************************************/

#include "fsl_edma_driver.h"
#include "fsl_device_registers.h"
#include "constants.h"

void config_DMA_Upper(void);

void config_DMA_Lower(void);

uint16_t bufSize(void);

void copyActiveToFloat(float fBuffer[]);

uint8_t bufferReady(void);

void bufferStatusReset(void);
