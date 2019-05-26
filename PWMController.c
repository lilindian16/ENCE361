/*
 * PWMController.c
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */

#include <peripheralAndSwitchControl.h>
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
#include "driverlib/pwm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "circBufT.h"
#include "stdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "PWMController.h"
#include "PWMGains.h"

//*************************************************************************
// The Proportional Integral Controller for the Helicopter
//*************************************************************************
void Main_PIControl(int Current_Height, double delta_t)
{
    double errHeight;

    // The PI controller gains for the main rotor
    double kpHeight = MAIN_KP_HELI;                         // Proportional Gain
    double kiHeight = MAIN_KI_HELI;                            // Integral Gains

    // The height error calculations
    errHeight = desiredHeight - Current_Height;           // Height Error
    errHeight_integrated += errHeight * delta_t;        // Integral Height Error

    // Calculation of the main rotor's duty cycle
    main_duty = MAIN_CONSTANT + kpHeight * errHeight + kiHeight * errHeight_integrated;

    // Specified limits given from the lecture
    if (main_duty > MAIN_PWM_MAX)
    {
        main_duty = MAIN_PWM_MAX;                                   // Maximum duty cycle
    }
    if (main_duty < MAIN_PWM_MIN)
    {
        main_duty = MAIN_PWM_MIN;                                    // Minimum duty cycle
    }

    // Recored previous height error for the integral error
    previous_errHeight = errHeight;
}

void Tail_PIControl(int32_t degrees, double delta_t)
{
    double errYaw;

    // The PI Control gains for the tail rotor
    double kpYaw = TAIL_KP_HELI;                            // Proportional Gain
    double kiYaw = TAIL_KI_HELI;                               // Integral Gains

    // The yaw error calculations
    errYaw = desiredYaw - degrees;                         // Yaw Error
    errYaw_integrated += errYaw * delta_t;             // Integral Yaw Error

    // Calculation of the tail rotor's duty cycle

    tail_duty = TAIL_CONSTANT + kpYaw * errYaw + kiYaw * errYaw_integrated;

    // Specified limits given from the lecture
    if (tail_duty > TAIL_PWM_MAX)
    {
        tail_duty = TAIL_PWM_MAX;                                 // Maximum Duty Cycle
    }
    if (tail_duty < TAIL_PWM_MIN)
    {
        tail_duty = TAIL_PWM_MIN;                                  // Minimum Duty Cycle
    }

    // Record Previous yaw error for the integral
    previous_errYaw = errYaw;
}

/********************************************************
 * Function to set the freq, duty cycle of M0PWM7
 ********************************************************/
void setMainPWM(uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / ui32Freq;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
                     ui32Period * ui32Duty / TO_PERCENT);
}

/********************************************************
 * Function to set the freq, duty cycle of M0PWM7
 ********************************************************/
void setTailPWM(uint32_t ui32TailFreq, uint32_t ui32TailDuty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32TailPeriod = SysCtlClockGet() / PWM_DIVIDER / ui32TailFreq;

    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32TailPeriod);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,
                     ui32TailPeriod * ui32TailDuty / TO_PERCENT);
}

/*********************************************************
 * initialisePWM
 * M0PWM7 (J4-05, PC5) is used for the main rotor motor
 *********************************************************/
void initialiseMainPWM(void)
{
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    setMainPWM(PWM_START_RATE_HZ, PWM_FIXED_DUTY);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}


/*********************************************************
 * initialiseTailPWM
 * M1PWM5 (J3-10, PC1) is used for the tail rotor motor
 *********************************************************/
void initialiseTailPWM(void)
{
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);

    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN,
    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    setTailPWM(PWM_START_RATE_HZ_TAIL, PWM_START_DUTY_TAIL);

    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}




