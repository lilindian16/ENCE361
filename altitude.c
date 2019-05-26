/*
 * adcHandler.c
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "circBufT.h"
#include "stdlib.h"

#include "altitude.h"

void ADCIntHandler(void)
{
    uint32_t ulValue;
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);
    writeCircBuf(&g_inBuffer, ulValue);
    ADCIntClear(ADC0_BASE, 3);
}

void initADC(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntRegister(ADC0_BASE, 3, ADCIntHandler);
    ADCIntEnable(ADC0_BASE, 3);
}

//***********************************************************************
// Converts the ADC value to a linear increase
//***********************************************************************
int32_t Conversion(int32_t ADC_Value)
{
    int32_t Conversion = (((ADC_Value) * Reference_ADC_Value - 1000)
            / Reference_ADC_Value);
    return Conversion;
}

//***********************************************************************
// Takes the mean ADC value that is read from the circular buffer
//***********************************************************************
int32_t meanHeightADC(void)
{
    int32_t sum = 0;
    int32_t i = 0;
    for (i = 0; i < BUF_SIZE; i++)
    {
        sum = sum + readCircBuf(&g_inBuffer);
    }
    Current_ADC_Value = sum / BUF_SIZE;
    return Current_ADC_Value;
}

//************************************************************************
// Calculates the altitude of the helicopter as a percentage
//************************************************************************
int32_t CalcHeight(int32_t Current_ADC_Value)
{
    int32_t height, Ref_ADC_Ratio, Current_ADC_Ratio;

    /* Does Conversion of the reference and current ADC value into their respective ratios */
    Ref_ADC_Ratio = Conversion(Reference_ADC_Value);
    Current_ADC_Ratio = Conversion(Current_ADC_Value);

    /* Calculates the factor as a percentage and if Current ADC Value > Reference ADC Value
     * a Negative Flag would be set to 1 to calculate negative integers */
    height = (((Ref_ADC_Ratio - Current_ADC_Ratio) * 100) / 1000);
    return height;
}
