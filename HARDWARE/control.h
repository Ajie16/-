#ifndef   __CONTROL_H
#define   __CONTROL_H

#include "stm32f10x.h"
#include "imu.h"
extern int alt_target;
void Control(FLOAT_ANGLE *att_in,FLOAT_XYZ *gyr_in);

#endif

