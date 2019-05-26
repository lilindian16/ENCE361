/*
 * yaw.h
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */
// CAN I GET A YEEEEEEEEEAAAAAAAAAAAA BOOOOOOOOIIIIIII for the 9.5/10???

#ifndef YAW_H_
#define YAW_H_

bool calibrated, landing, operational, Reset, currently_landing;
int reference_yaw_found;
int32_t findYaw;

//*******************************************************************
// Yaw Calculations
//*******************************************************************
int32_t degrees, displaydegrees;
volatile int32_t previous_state, current_state;
volatile int32_t yaw, displayYaw;

//
int32_t
yawtoDegrees(int32_t yaw);

//
int32_t
currentYaw(int previous_state, int current_state);

//
void
QuadIntHandler(void);

//
void
QuadDecodeInit(void);

//
void
ReferenceYawIntHandler(void);

//
void
ReferenceYawInit(void);

//


#endif /* YAW_H_ */
