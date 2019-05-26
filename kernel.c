// *****************************************************************
//
//  Created on: 24/05/2019
//  Author: Zoren, ELlis and Jaime
//
// Pretty self explanatory don't you think???
// This kernel will schedule the foreground and background processes.
// This is all done using the designed state machine ;)
// ******************************************************************

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

#include "altitude.h"
#include "buttons.h"
#include "circBufT.h"
#include "peripheralAndSwitchControl.h"
#include "PWMController.h"
#include "PWMGains.h"
#include "UARTController.h"
#include "yaw.h"

#include "kernel.h"

// This is th starting sequence for the kernel and the program.
// It initialises everything
void startThisPuppyUp(void)
{
    resetPeripherals();
    initClock();
    initADC();
    initButtons();
    initialiseUSB_UART();
    initSysTick();
    initCircBuf(&g_inBuffer, BUF_SIZE);
    ButtonIntInit();
    ResetIntInit();
    ReferenceYawInit();
    QuadDecodeInit();
    initialiseMainPWM();
    initialiseTailPWM();
    SwitchIntInit();

    //Intitialise FSM
    current_state = 1;
    previous_state = 1;
    //Reset all the states as a safety precaution
    calibrated = 0;
    landing = 0;
    operational = 0;
    currently_landing = 0;
    reference_yaw_found = 0;

    IntMasterEnable();
}

// Function name translates roughly into run the kernel please
// Yet again it is pretty self explanatory what the function does
void runTheKernelPorFavor(void)
{
    // Continous read of the ADC value for altitude calculations
    Current_ADC_Value = meanHeightADC(); // Calculates the mean value of the ADC
    Current_Height = CalcHeight(Current_ADC_Value); // Determines the height of the Helicopter
    degrees = yawtoDegrees(yaw); // Determines the degrees of the Helicopter

    //*************************************************************************
    // Calibration Stage
    //*************************************************************************
    if (initialread == 0 && operational)
    {
        SysCtlDelay(SysCtlClockGet() / 2); // Adds a delay of 0.5s for reference ADC value to be calculated
        Reference_ADC_Value = meanHeightADC();
        initialread++;
    }

    if (initialread == 1 && !findYaw && operational)
    {
        desiredHeight = 10;
        Main_PIControl(Current_Height, 0.1);
        Tail_PIControl(degrees, 0.1);
        if (Current_Height >= 10)
        {
            findYaw = 1;
        }

    }

    if (findYaw == 1 && !calibrated && operational)
    {
        Main_PIControl(Current_Height, 0.1);
        tail_duty = main_duty * Hover;
        desiredYaw = 0;
    }

    //*************************************************************************
    // Flying Stage
    //*************************************************************************
    if (calibrated && operational)
    {
        Main_PIControl(Current_Height, 0.1);
        Tail_PIControl(degrees, 0.1);
    }

    //*************************************************************************
    // Landing Stage
    //*************************************************************************
    if (calibrated && !operational && !landing)
    {
        Main_PIControl(Current_Height, 0.1);
        tail_duty = main_duty * Hover;
        currently_landing = 1;
    }

    if (landing && Current_Height > 30)
    {

        desiredHeight = 30;

        desiredYaw = 0;
        Tail_PIControl(degrees, 0.1);
        Main_PIControl(Current_Height, 0.1);
    }

    if (landing && Current_Height > 5 && Current_Height <= 30)
    {

        desiredHeight = 5;

        desiredYaw = 0;
        Tail_PIControl(degrees, 0.1);
        Main_PIControl(Current_Height, 0.1);
    }

    if (landing && Current_Height <= 5)
    {
        desiredHeight = 0;
        SysCtlDelay(SysCtlClockGet() / 6);
        if (!operational)
        {
            PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
            PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
        }
    }

    if (Current_Height <= 0)
    {
        currently_landing = 0;
    }

    // Send the UART update if timing is correct?
    if (slowTick)
    {
        slowTick = false;
        // Form and send a status message to the console
        if (operational)
        {
            usprintf(
                    statusStr,
                    "Alt=%2d [%2d] Yaw=%2d [%2d] \n Main:%2d Tail:%2d \n Mode:Flying \r\n",
                    Current_Height, desiredHeight, degrees, desiredYaw,
                    main_duty, tail_duty); // * usprintf
        }
        else
        {
            usprintf(
                    statusStr,
                    "Alt=%2d [%2d] Yaw=%2d [%2d] \n Main:%2d Tail:%2d \n Mode:Landing \r\n",
                    Current_Height, desiredHeight, degrees, desiredYaw,
                    main_duty, tail_duty);
        }
        UARTSend(statusStr);
    }
    setMainPWM(PWM_START_RATE_HZ, main_duty);
    setTailPWM(PWM_START_RATE_HZ, tail_duty);
}

