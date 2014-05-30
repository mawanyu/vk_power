/*******************************************/
/* Include Files */
/*******************************************/
#include "msp430afe253.h"
#include "intrinsics.h"

/******************************************/
/* Defines and Macros */
/******************************************/
//Folling is some definations for time calculation,
//Should be modified when main clock is changed.
//Note that they must be integers.
//this defines clock ticks per 1us, it will be MCLK(MHz)
#define TICKS_PER_USEC (1)
//this defines clock ticks per 1ms, it will be MCLK(KHz)
#define TICKS_PER_MSEC (1000)

//delay some time, x must be constant integer
#define DELAY_USEC(x) (__delay_cycles(TICKS_PER_USEC * x))
#define DELAY_MSEC(x) (__delay_cycles(TICKS_PER_MSEC * x))

/******************************************/
/* Variables */
/******************************************/

/******************************************/
/* Functions */
/******************************************/

