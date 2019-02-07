#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_edma_driver.h"
#include "fsl_adc16_driver.h"
#include "fsl_clock_manager.h"
#include "arm_const_structs.h"
#include <math.h>
#include "board.h"

#include "edma_init.h"
#include "perf_init.h"
#include "constants.h"

extern void UART_DRV_IRQHandler(uint32_t instance);

void UART3_RX_TX_IRQHandler(void)
{
    UART_DRV_IRQHandler(3);
}

struct Metrics {
    uint32_t avgTimeFFT; //FFT Processing time in uS
    uint32_t avgTimeFormatSend; // Time to format and send Data
    uint32_t avgTimeWait; // Time left over until next buffer is ready.
    uint32_t timeCount;
    uint32_t loopCounter;
    uint8_t time_over_run;
};

int main(void)
{
    uint32_t i;
    uint32_t txBytes;
	float FFTArray0[BUF_SIZE];
    float FFTArray1[BUF_SIZE];
    uint16_t LEDArray[LED_COLUMNS];
    uint8_t *ptrLEDArray;
    ptrLEDArray = (uint8_t*)LEDArray;
    float LEDFloat;

    float xAxisLinearValue, xAxisLinearValue_Last;
    uint16_t xAxisLinearIndex, xAxisLinearIndex_Last;
    float kXaxis = 38/logf((float)(BUF_SIZE/2-1));

    //float kXaxis = sqrtf(LED_COLUMNS*logf((float)(BUF_SIZE/2-1)));
    // Calculate constants for semi-log graph scaling.

    struct Metrics metric = {0};

    // Initialize CFFT structures
    arm_rfft_fast_instance_f32 S;
    arm_rfft_fast_init_f32(&S, BUF_SIZE);

    // Initialize Clocks
    BOARD_ClockInit();
    // Initialize peripherals
    config_DMA_Upper();
    config_DMA_Lower();
    config_PDB();
    init_adc(0);

    /***** INIT UART *****************************/
    // Fill in uart config data
    uart_state_t uartState;
    uart_user_config_t uartConfig = {
        .bitCountPerChar = kUart8BitsPerChar,
        .parityMode      = kUartParityDisabled,
        .stopBitCount    = kUartOneStopBit,
        .baudRate        = 115200
    };

    CLOCK_SYS_EnablePortClock(PORTC_IDX);
    PORT_HAL_SetMuxMode(PORTC,16u,3);
    PORT_HAL_SetMuxMode(PORTC,17u,3);

    // Initialize the uart module with base address and config structure
    UART_DRV_Init(3, &uartState, &uartConfig);
    /********************************************/

    initPIT();

    // Main Loop
    while(1) {

        metric.loopCounter++;
        metric.timeCount = PIT_CVAL2;
        if(bufferReady()) //Make sure we finished the last loop in time.
            metric.time_over_run = 1;

        // Wait for new sample buffer to be ready
        while(!bufferReady()) {}
        bufferStatusReset();

        metric.timeCount = metric.timeCount - PIT_CVAL2;
        metric.avgTimeWait = (metric.timeCount + metric.loopCounter*metric.avgTimeWait) / (metric.loopCounter + 1);
        metric.timeCount = PIT_CVAL2;

        // Convert new sample buffer to float array
        copyActiveToFloat(FFTArray0);

        // (&S, input buffer, output buffer, 0 - Don't do the inverse FFT.)
        arm_rfft_fast_f32(&S, FFTArray0, FFTArray1, 0);

        // The first two values of the RFFT are bins 0 and BUF_SIZE/2, which are
        // purely real. To save memory, the output is the first half of FFTArray0. The send half of the array is left as sample data.
        // &S, input buffer, output buffer, 0 - Don't do the inverse FFT.
        // (input buffer, output buffer, buffer size)
        arm_cmplx_mag_f32(&FFTArray1[2], &FFTArray0[1], (uint32_t)(BUF_SIZE/2-1));
        // The rfft seems to scale the first and last pack bins twice as much as the others. For some reason it will return negative values.
        // The documentation says it can't do this. But it also says the forward rfft doesn't scale, so it's full of crap.
        FFTArray0[0] = fabs(FFTArray1[0]/2.0);
        FFTArray0[BUF_SIZE/2] = fabs(FFTArray1[1]/2.0);

        metric.timeCount = metric.timeCount - PIT_CVAL2;
        metric.avgTimeFFT = (metric.timeCount + metric.loopCounter*metric.avgTimeFFT) / (metric.loopCounter + 1);
        metric.timeCount = PIT_CVAL2;

        //Format Data
        xAxisLinearValue_Last = expf((float)(7)/kXaxis);
        for(i = 0; i < LED_COLUMNS; i++) {

            xAxisLinearValue = expf((float)(i+7)/kXaxis);

            // If log step less than 1, do linear interpolation of closest bins
            if(xAxisLinearValue - xAxisLinearValue_Last <= 1.0f) {
                xAxisLinearIndex = (uint16_t)(xAxisLinearValue); //truncate actual value, and interpolate between two closest bins.
                // y = y0 + (y1 - y0)(x - x0)/(x1 - x0), where x1 - x0 = 1
                LEDFloat = FFTArray0[xAxisLinearIndex] + (FFTArray0[xAxisLinearIndex + 1] - FFTArray0[xAxisLinearIndex]) * (xAxisLinearValue - (float)xAxisLinearIndex);
            }

            //If log step greater than 1, get mean of bins.
            else {
                xAxisLinearIndex = roundf(xAxisLinearValue);
                xAxisLinearIndex_Last = roundf(xAxisLinearValue_Last);
                arm_mean_f32(&FFTArray0[xAxisLinearIndex_Last], xAxisLinearIndex - xAxisLinearIndex_Last, &LEDFloat);
                LEDFloat = LEDFloat*(xAxisLinearIndex - xAxisLinearIndex_Last);
            }

            // Normalize LEDFloat to LED Column height. (LED brightness used for fractional steps)
            // This line also corrects for FFT scaling.
            LEDArray[i] = (uint16_t)(LED_ROWS * 256 * (LEDFloat/(BUF_SIZE/8)));

            xAxisLinearValue_Last = xAxisLinearValue;
        }

        //Send Data
        txBytes = sizeof(LEDArray);
            UART_DRV_SendData(3, ptrLEDArray, txBytes);
            while (kStatus_UART_TxBusy == UART_DRV_GetTransmitStatus(3, NULL)){}

        metric.timeCount = metric.timeCount - PIT_CVAL2;
        metric.avgTimeFormatSend = (metric.timeCount + metric.loopCounter*metric.avgTimeFormatSend) / (metric.loopCounter + 1);
        metric.timeCount = PIT_CVAL2;



    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
