/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"

/******************************************/
/* Defines and Macros */
/******************************************/


/******************************************/
/* Variables */
/******************************************/
/* Indicate Timer_A0 interrupt occured. */
extern char timer_a0_int_flag;

/******************************************/
/* Functions */
/******************************************/

/********************************************************************
* Funcion Name
*     
* Input Param
*     
* Output Param
*     
* Return Code
*     
* Description
*     
********************************************************************/
/* Timers initialise */
extern void timer_initialise(void);
/* Timer_A0 control functions */
extern void timer_a0_start(void);
extern void timer_a0_stop(void);
/* Timer_A1 control functions */
extern void timer_a1_start(void);
extern void timer_a1_stop(void);

