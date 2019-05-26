/*
 * PWMGains.h
 *
 *  Created on: 24/05/2019
 *      Author: lilindian14
 */
// CAN I GET A YEEEEEEEEEAAAAAAAAAAAA BOOOOOOOOIIIIIII for the 9.5/10???

#ifndef PWMGAINS_H_
#define PWMGAINS_H_

//********************************************************
// The Controls Global variables
//********************************************************

#define MAIN_KP_H1     0.4
#define MAIN_KI_H1     0.0005
#define TAIL_KP_H1     1.05              // Previously 1.4
#define TAIL_KI_H1     0.0001

#define MAIN_KP_H2     0.4
#define MAIN_KI_H2     0.0005
#define TAIL_KP_H2     1.3              // Previously 1.4
#define TAIL_KI_H2     0.0001

#define MAIN_KP_H3     0.4
#define MAIN_KI_H3     0.0005
#define TAIL_KP_H3     1.2              //Compensation for undershoot by 3-6 degrees
#define TAIL_KI_H3     0.0001

#define MAIN_KP_H4     0.42              //Calibration takes too long hence the bump
#define MAIN_KI_H4     0.0005
#define TAIL_KP_H4     1.3               //Compensation for undershoot by 3-6 degrees
#define TAIL_KI_H4     0.0001

#define MAIN_KP_H5     0.37
#define MAIN_KI_H5     0.0005
#define TAIL_KP_H5     0.7
#define TAIL_KI_H5     0.0001

#define Hover_H1       0.95
#define Hover_H2       1.25
#define Hover_H3       1.4              // Pretty good control
#define Hover_H4       1.2
#define Hover_H5       0.95

// Set the helicopter used for demonstration
#define Hover          Hover_H3
#define MAIN_KP_HELI   MAIN_KP_H3
#define MAIN_KI_HELI   MAIN_KI_H3
#define TAIL_KP_HELI   TAIL_KP_H3
#define TAIL_KI_HELI   TAIL_KI_H3

#endif /* PWMGAINS_H_ */
