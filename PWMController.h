/*
 * PWMController.h
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */
// CAN I GET A YEEEEEEEEEAAAAAAAAAAAA BOOOOOOOOIIIIIII for the 9.5/10???

#ifndef PWMCONTROLLER_H_
#define PWMCONTROLLER_H_

//  PWM Hardware Details M0PWM7 (gen 3)
//  ---Main Rotor PWM: PC5, J4-05
#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5

// PWM Hardware Details M1PWM5
// ---Tail Rotor PWM: PF1, J3-10
#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1


//********************************************************
// The PWM Global variables
//********************************************************
// PWM configuration
#define PWM_START_RATE_HZ  250
#define PWM_RATE_STEP_HZ   50
#define PWM_RATE_MIN_HZ    50
#define PWM_RATE_MAX_HZ    400
#define PWM_FIXED_DUTY     67
#define PWM_MAX_DUTY       95
#define PWM_MIN_DUTY       5
#define STEP_DUTY          5
#define PWM_START_DUTY     50

#define PWM_DIVIDER        4
#define PWM_START_DUTY_TAIL 10
#define PWM_START_RATE_HZ_TAIL 200

#define MAIN_PWM_MAX 85
#define MAIN_PWM_MIN 15

#define TAIL_PWM_MAX 98
#define TAIL_PWM_MIN 2

#define MAIN_CONSTANT 15
#define TAIL_CONSTANT 10

#define TO_PERCENT 100

int32_t desiredHeight;
int32_t desiredYaw;
double errHeight_integrated, previous_errHeight, errYaw_integrated, previous_errYaw, errYaw_int;
int main_duty, tail_duty;


//
void
Main_PIControl(int Current_Height, double delta_t);

//
void
Tail_PIControl(int32_t degrees, double delta_t);

//
void
setMainPWM(uint32_t ui32Freq, uint32_t ui32Duty);

//
void
setTailPWM(uint32_t ui32TailFreq, uint32_t ui32TailDuty);

//
void
initialiseMainPWM(void);

//
void
initialiseTailPWM(void);


#endif /* PWMCONTROLLER_H_ */
