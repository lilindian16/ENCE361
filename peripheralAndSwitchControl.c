/*
 * tivaSetup.c
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */
// CAN I GET A YEEEEEEEEEAAAAAAAAAAAA BOOOOOOOOIIIIIII for the 9.5/10???

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pwm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "driverlib/adc.h"


#include "buttons.h"
#include "PWMController.h"
#include "peripheralAndSwitchControl.h"


//*******************************************************************
//
// The interrupt handler for the SysTick interrupt.
//
//*******************************************************************
void SysTickIntHandler(void)
{
    ADCProcessorTrigger(ADC0_BASE, 3);
    g_ulSampCnt++;    // Counter for the interrupts
    static uint8_t tickCount = 0;
    const uint8_t ticksPerSlow = SYSTICK_RATE_HZ / SLOWTICK_RATE_HZ;

    updateButtons();       // Poll the buttons
    if (++tickCount >= ticksPerSlow)
    {                       // Signal a slow tick
        tickCount = 0;
        slowTick = true;
    }
}

//*******************************************************************
void initSysTick(void)
{
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}


//********************************************************
// Initialisation functions: clock, SysTick, display, UART
//********************************************************
void initClock(void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
    SYSCTL_XTAL_16MHZ);

    // Set the PWM clock rate (using the prescaler)
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
}

void resetPeripherals (void)
{
    SysCtlPeripheralReset(PWM_MAIN_PERIPH_GPIO); // Used for PWM output
    SysCtlPeripheralReset(PWM_MAIN_PERIPH_PWM);  // Main Rotor PWM
    SysCtlPeripheralReset(PWM_TAIL_PERIPH_GPIO); // Used for PWM output
    SysCtlPeripheralReset(PWM_TAIL_PERIPH_PWM);  // Tail Rotor PWM
}

void ResetIntHandler(void)
{
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_6);
    SysCtlReset();
}

void ResetIntInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOIntRegister(GPIO_PORTA_BASE, ResetIntHandler);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPD);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_6);
}

void SwitchIntHandler(void)
{
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_7);

    if (!currently_landing)
    {
        if (operational == 0)
        {
            landing = 0;
            PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
            PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
            operational = 1;

        }
        else
        {
            operational = 0;
        }
    }
}

void SwitchIntInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOIntRegister(GPIO_PORTA_BASE, SwitchIntHandler);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_7);

}



