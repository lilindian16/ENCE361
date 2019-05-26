/*
 * yaw.c
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

#include "yaw.h"


//*************************************************************************
// Quadrature Decoding for the helicopter project
//*************************************************************************
void QuadIntHandler(void)
{
    int32_t encoder_A, encoder_B;

    // Pin read on each Encoder values
    encoder_A = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
    encoder_B = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);

    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // The Finite state machine :D
    previous_state = current_state;
    if (!encoder_A)
    {
        if (encoder_B)
        {
            current_state = 2;
        }
        else
        {
            current_state = 1;
        }
    }
    else
    {
        if (encoder_B)
        {
            if (encoder_A)
            {
                current_state = 3;
            }
            else
            {
                current_state = 4;
            }
        }
    }
    yaw = currentYaw(previous_state, current_state);
}

//*************************************************************************
// Initialise Quadrature Decoding
//*************************************************************************
void QuadDecodeInit(void)
{
    // Enable and Initialise all the periperals for Quadrature Decoding
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1,
    GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);

    // Set interrupt for Quadrature Encoding
    GPIOIntRegister(GPIO_PORTB_BASE, QuadIntHandler);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    IntEnable(INT_GPIOB);

}

//*************************************************************************
// Scaling of the Quadrature reading of the Yaw to 360 degress
//*************************************************************************
int32_t yawtoDegrees(int32_t yaw)
{
    int32_t degrees;
    degrees = ((32143 * yaw) / 10000);
    displaydegrees = (displayYaw * 360) / 112;
    return degrees;
}

//*************************************************************************
// Quadrature Decoding calculations
//*************************************************************************
int32_t currentYaw(int previous_state, int current_state)
{
    if (current_state > previous_state)
    {
        yaw++;
        displayYaw++;
    }
    if (current_state < previous_state)
    {
        yaw--;
        displayYaw--;
    }
    return yaw;
}

void ReferenceYawIntHandler(void)
{
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);
    if (calibrated == 0)
    {
        yaw = 0;
    }
    if (!operational)
    {
        landing = 1;
        yaw = 0;
    }

    calibrated = 1;        // Avoid using "++" as signal is not debounced!!!!
    displayYaw = 0;
}

void ReferenceYawInit(void)
{
    // Enable and Initialise all the periperals for Quadrature Decoding
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOIntRegister(GPIO_PORTC_BASE, ReferenceYawIntHandler);

    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4,
    GPIO_STRENGTH_4MA,
                     GPIO_PIN_TYPE_STD_WPU);

    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);

    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);
    IntEnable(INT_GPIOC);
}

