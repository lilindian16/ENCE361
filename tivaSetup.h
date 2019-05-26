/*
 * tivaSetup.h
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */
// CAN I GET A YEEEEEEEEEAAAAAAAAAAAA BOOOOOOOOIIIIIII for the 9.5/10???

#ifndef TIVASETUP_H_
#define TIVASETUP_H_

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

static uint32_t g_ulSampCnt;    // Counter for the interrupts

void initClock (void);

#endif /* TIVASETUP_H_ */
