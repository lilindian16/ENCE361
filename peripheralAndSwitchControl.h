/*
 * tivaSetup.h
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */
// CAN I GET A YEEEEEEEEEAAAAAAAAAAAA BOOOOOOOOIIIIIII for the 9.5/10???

#ifndef PERIPHERALANDSWITCHCONTROL_H_
#define PERIPHERALANDSWITCHCONTROL_H_

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


#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4

#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF

#define SYSTICK_RATE_HZ 100
#define SAMPLE_RATE_HZ 400
#define SLOWTICK_RATE_HZ 4

uint8_t slowTick;

uint32_t g_ulSampCnt;    // Counter for the interrupts

//
void
SysTickIntHandler(void);

//
void
initSysTick(void);

//
void
initClock (void);

//
void
resetPeripherals (void);

//
void
ResetIntHandler(void);

//
void
ResetIntInit(void);

//
void
SwitchIntHandler(void);

//
void
SwitchIntInit(void);


#endif /* PERIPHERALANDSWITCHCONTROL_H_ */
