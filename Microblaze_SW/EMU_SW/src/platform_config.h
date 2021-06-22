#ifndef __PLATFORM_CONFIG_H_
#define __PLATFORM_CONFIG_H_

#include "xgpio.h"

#define SET_ENABLE() 	XGpio_DiscreteWrite(&Gpio, 1, 0x01)

XGpio Gpio; /* The Instance of the GPIO Driver */

#endif
