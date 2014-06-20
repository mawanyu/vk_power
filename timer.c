/*******************************************/
/* Include Files */
/*******************************************/
#include "timer.h"
#include "led.h"
//#include "adc.h"

/******************************************/
/* Defines and Macros */
/******************************************/
/* Following are compare value of Timer_A0 and Timer_A1. These mean how many */
/* timer clocks when timer occur an interrupt. */
/* They can be not over 65535(0xFFFF). */
#define TIMER_A0_VALUE  (2500u)     //10Hz
#define TIMER_A1_VALUE  (50000u)    //0.5Hz

/******************************************/
/* Variables */
/******************************************/
/* Indicate Timer_A0 interrupt occured. */
char timer_a0_int_flag = 0x0;

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
void timer_initialise(void)
{
    /* Timer_A0 initialise */
    /* Set source to SMCLK(1MHz), divided by 8*5 to 25KHz; up mode; */
    /* enable interrupt; timer stop. */
    TA0CTL = TASSEL__SMCLK + ID__8 + TAIE;
    TA0EX0 = TAIDEX_4;
    /* Set Timer_A0 interrupt to 10Hz. */
    TA0CCR0 = TIMER_A0_VALUE;
    /* Clear Timer_A0 counter. */
    TA0R = 0x00;

    /* Timer_A1 initialise */
    /* Set source to SMCLK(1MHz), divided by 8*5 to 25KHz; up mode; */
    /* enable interrupt; timer stop. */
    TA1CTL = TASSEL__SMCLK + ID__8 + TAIE;
    TA1EX0 = TAIDEX_4;
    /* Set Timer_A0 interrupt to 0.5Hz. */
    TA1CCR0 = TIMER_A1_VALUE;
    /* Clear Timer_A0 counter. */
    TA1R = 0x00;
}

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
void timer_a0_start(void)
{
    /* If Timer_A0 is halted, start it by up mode. */
    if((TA0CTL & (MC0 + MC1)) == MC__STOP)
        TA0CTL |= MC__UP;
}

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
void timer_a0_stop(void)
{
    /* Halt Timer_A0 */
    TA0CTL &= ~(MC0 + MC1);
    /* Clear Timer_A0 counter */
    TA0R = 0x0;
}

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
void timer_a1_start(void)
{
    /* If Timer_A1 is halted, start it by up mode. */
    if((TA1CTL & (MC0 + MC1)) == MC__STOP)
        TA1CTL |= MC__UP;
}

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
void timer_a1_stop(void)
{
    /* Halt Timer_A1 */
    TA1CTL &= ~(MC0 + MC1);
    /* Clear Timer_A1 counter */
    TA1R = 0x0;
}

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
#pragma vector = TIMER0_A1_VECTOR
__interrupt void timer_a0_isr(void)
{
    /* Clear interrupt flag */
    TA0CTL &= ~TAIFG;
    
    /* Reset Timer_A0 counter */
    //TA0R = 0x0;

    /* Set complete flag */
    timer_a0_int_flag = 0x1;
}

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
#pragma vector = TIMER1_A1_VECTOR
__interrupt void timer_a1_isr(void)
{
    /* Clear interrupt flag */
    TA1CTL &= ~TAIFG;

    /* Reset Timer_A1 counter */
    //TA1R = 0x0;

    /* Toggle alart red LED */
    if(aled_r_flash_flag == LED_FLASH_ON)
        led_toggle_aled_r();

    /* Toggle alart yellow LED */
    if(aled_y_flash_flag == LED_FLASH_ON)
        led_toggle_aled_y();
}

