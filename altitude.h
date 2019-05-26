/*
 * adcHandler.h
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */

#ifndef ALTITUDE_H_
#define ALTITUDE_H_

#define MAX_STR_LEN 60
#define BUF_SIZE 40

int32_t Current_Height, initialread;
circBuf_t g_inBuffer; // Buffer of size BUF_SIZE integers (sample values)

//*******************************************************************
// Height Calculations
//*******************************************************************
int32_t Current_ADC_Value;
int32_t Reference_ADC_Value;


//
void
ADCIntHandler (void);

//
void
initADC(void);

//
int32_t
Conversion(int32_t ADC_Value);

//
int32_t
meanHeightADC(void);

//
int32_t
CalcHeight(int32_t Current_ADC_Value);

#endif /* ALTITUDE_H_ */
