/*******************************************/
/* Include Files */
/*******************************************/
#include "led.h"
#include "timer.h"

/******************************************/
/* Defines and Macros */
/******************************************/
/* ACLED_G_PIN/DCLED_W_PIN/BTLED_B_PIN/ALED_R_PIN are in port3 */
#define ACLED_G_PIN     (0x80u)
#define DCLED_W_PIN     (0x40u)
#define BTLED_B_PIN     (0x20u)
#define ALED_R_PIN      (0x10u)
/* ALED_Y_PIN is in port2 */
#define ALED_Y_PIN      (0x40u)

/******************************************/
/* Variables */
/******************************************/


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
void led_initialise(void)
{
    /* Select LED related pins as GPIO pins(default) */
    P3SEL0 &= ~(ACLED_G_PIN + DCLED_W_PIN + BTLED_B_PIN + ALED_R_PIN);
    P3SEL1 &= ~(ACLED_G_PIN + DCLED_W_PIN + BTLED_B_PIN + ALED_R_PIN);
    P2SEL0 &= ~ALED_Y_PIN;
    P2SEL1 &= ~ALED_Y_PIN;

    /* Select LED related pins as output */
    P3DIR = ACLED_G_PIN + DCLED_W_PIN + BTLED_B_PIN + ALED_R_PIN;
    P2DIR = ALED_Y_PIN;

    /* Turn on all LEDs */
    P3OUT |= ACLED_G_PIN + DCLED_W_PIN + BTLED_B_PIN + ALED_R_PIN;
    P2OUT |= ALED_Y_PIN;
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
void led_acled_on(void)
{
    P3OUT |= ACLED_G_PIN;
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
void led_acled_off(void)
{
    P3OUT &= ~ACLED_G_PIN;
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
void led_dcled_on(void)
{
    P3OUT |= DCLED_W_PIN;
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
void led_dcled_off(void)
{
    P3OUT &= ~DCLED_W_PIN;
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
void led_btled_on(void)
{
    P3OUT |= BTLED_B_PIN;
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
void led_btled_off(void)
{
    P3OUT &= ~BTLED_B_PIN;
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
void led_aled_r_on(void)
{
    P3OUT |= ALED_R_PIN;
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
void led_aled_r_off(void)
{
    P3OUT &= ~ALED_R_PIN;
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
void led_toggle_aled_r(void)
{
    P3OUT ^= ALED_R_PIN;
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
void led_flash_aled_r(unsigned int switcher)
{
    aled_r_flash_flag  = switcher;
    
    if(switcher == LED_FLASH_ON) {
        timer_a1_start();
    }
    else {
        if(aled_y_flash_flag == LED_FLASH_OFF) {
            timer_a1_stop();
        }
    }
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
void led_aled_y_on(void)
{
    P2OUT |= ALED_Y_PIN;
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
void led_aled_y_off(void)
{
    P2OUT &= ~ALED_Y_PIN;
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
void led_toggle_aled_y(void)
{
    P2OUT ^= ALED_Y_PIN;
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
void led_flash_aled_y(unsigned int switcher)
{
    aled_y_flash_flag = switcher;

    if(switcher == LED_FLASH_ON) {
        timer_a1_start();
    }
    else {
        if(aled_r_flash_flag == LED_FLASH_OFF) {
            timer_a1_stop();
        }
    }
}

