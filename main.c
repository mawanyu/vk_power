/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"
#include "adc.h"
#include "led.h"
#include "timer.h"
#include "power.h"

/******************************************/
/* Defines and Macros */
/******************************************/


/******************************************/
/* Variables */
/******************************************/


/******************************************/
/* Functions */
/******************************************/
void sysclock_initialise(void);

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
void main(void)
{
    /* Stop watchdog timer to prevent time out reset */
    WDTCTL = WDTPW + WDTHOLD;

    /* Enable markable interrupts */
    _BIS_SR(GIE);

    /* Initialise system */
    sysclock_initialise();
    timer_initialise();
    adc_initialise();
    led_initialise();
    pwr_initialise();

/*
    while(1) {
        adc_start(ADC_CH_AC_IN, &adcr_ac_in, 500);
        adc_start(ADC_CH_DC_IN, &adcr_dc_in, 50);
        adc_start(ADC_CH_BKBAT, &adcr_bkbat, 50);
        adc_start(ADC_CH_INBAT, &adcr_inbat, 50);
        adc_start(ADC_CH_SYS, &adcr_sys_in, 50);
        adc_start(ADC_CH_MC_OUT, &adcr_mc_out, 50);
        adc_start(ADC_CH_UI_OUT, &adcr_ui_out, 50);
        adc_start(ADC_CH_FAN, &adcr_fan, 50);
        adc_start(ADC_CH_CHG, &adcr_charge, 50);
    }
*/    
    /* Start Timer A0 (10Hz) for ADC */
    timer_a0_start();

    while(1) {
        while(adc_complete_flag != ADC_COMPLETE_ALL);

        adc_complete_flag = ADC_COMPLETE_NONE;
        
        pwr_monitor();
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
void sysclock_initialise(void)
{
    /* Write password to enable CS register access */
    CSCTL0 = CSKEY;

    /* Select DCO to 8MHz(default) */
    CSCTL1 = DCOFSEL_3;

    /* Set MCLK/ACLK/SMCLK sources to DCOCLK */
    CSCTL2 = SELM__DCOCLK + SELA__DCOCLK + SELS__DCOCLK;

    /* Set MCLK/SMCLK to 1MHz(divider is 8) */
    /* Set ACLK to 4MHz(divider is 2, for AD10 */
    CSCTL3 = DIVM__8 + DIVA__2 + DIVS__8;

    /* Turn off XT1 and XT2 */
    CSCTL4 = XT1OFF + XT2OFF;

    /* Disable CS register access*/
    CSCTL0_H = 0x00;
}
