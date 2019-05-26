/*
 * kernel.h
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */

#ifndef KERNEL_H_
#define KERNEL_H_

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


#include "buttons.h"
#include "PWMGains.h"
#include "PWMController.h"
#include "UARTController.h"
#include "altitude.h"
#include "yaw.h"
#include "peripheralAndSwitchControl.h"

char statusStr[MAX_STR_LEN + 1];

void
startThisPuppyUp (void);

void
runTheKernelPorFavor (void);

#endif /* KERNEL_H_ */
