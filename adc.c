/*******************************************/
/* Include Files */
/*******************************************/
#include "adc.h"

/******************************************/
/* Defines and Macros */
/******************************************/
/* Following ADC pins are in Port1 */
#define ADC_PIN_CHARGE      (0x02)
#define ADC_PIN_FAN         (0x04)
#define ADC_PIN_INBAT       (0x08)
#define ADC_PIN_MC_OUT      (0x10)
#define ADC_PIN_AC_IN       (0x20)
/* Following ADC pins are in Port3 */
#define ADC_PIN_DC_IN       (0x01)
#define ADC_PIN_SYS_IN      (0x02)
#define ADC_PIN_BKBAT       (0x04)
#define ADC_PIN_UI_OUT      (0x08)

/******************************************/
/* Variables */
/******************************************/
/* Following are ADC result of certain signals */
unsigned int adcr_charge = 0;       /* battery charge I */
unsigned int adcr_fan    = 0;       /* fan V */
unsigned int adcr_inbat  = 0;       /* internal battery V */
unsigned int adcr_bkbat  = 0;       /* backup battery V */
unsigned int adcr_ac_in  = 0;       /* 24V AC/DC input V */
unsigned int adcr_dc_in  = 0;       /* 20V DC/DC(backup battery) input V*/
unsigned int adcr_sys_in = 0;       /* system DC input V(one of AD/DC, DC/DC, internal battery) */
unsigned int adcr_mc_out = 0;       /* output V for Main Control Board */
unsigned int adcr_ui_out = 0;       /* output V for UI Board */

/* This flag indicates all ADC channel have been converted */
char adc_complete_flag = ADC_COMPLETE_NONE;

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
void adc_initialise(void)
{
    /* Setup ADC module related pins */
    P1SEL0 |= ADC_PIN_CHARGE + ADC_PIN_FAN + ADC_PIN_INBAT + ADC_PIN_MC_OUT + ADC_PIN_AC_IN;
    P1SEL1 |= ADC_PIN_CHARGE + ADC_PIN_FAN + ADC_PIN_INBAT + ADC_PIN_MC_OUT + ADC_PIN_AC_IN;
    P3SEL0 |= ADC_PIN_DC_IN + ADC_PIN_SYS_IN + ADC_PIN_BKBAT + ADC_PIN_UI_OUT;
    P3SEL1 |= ADC_PIN_DC_IN + ADC_PIN_SYS_IN + ADC_PIN_BKBAT + ADC_PIN_UI_OUT;

    /* Clear ADC10ENC to unlock some configure bits */
    ADC10CTL0 &= ~ADC10ENC;
    
    /* Set ADC clock(4MHz). Source by ACLK, divided by 1. */
    /* Set ADC to single-channel-single-convertion mode, trigger by ADC10SC bit. */
    ADC10CTL1 = ADC10SHS_0 + ADC10DIV_0 + ADC10SSEL_1 + ADC10CONSEQ_0;

    /* Set ADC to 10 bits result. */
    ADC10CTL2 = ADC10PDIV__1 + ADC10RES;

    /* Initialise REF module to set ADC reference voltage to 2.5V. */
    /* It should automatically be on when ADC10 requests. */
    REFCTL0 |= REFVSEL_2 + REFON;

    /* Enable ADC_10 interrupt. */
    //ADC10IE = ADC10IE0;

    /* Enable ADC_10 module. */
    /* Enable convertion and lock some configure bits. */
    ADC10CTL0 = ADC10ON + ADC10ENC;
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
unsigned int adc_start(unsigned int adc_signal, unsigned int *result,
                        unsigned int timeout)
{
    /* Check ADC_10 is busy or not, if necessary. */
    //while(((ADC10CTL1 & ADC10BUSY) != 0x0) && (timeout > 0)) {
    //    timeout--;
    //}
    //if(0x0 == timeout) return ADC_RESULT_BUSY;

    /* Clear ADC10ENC to unlock configure bits. */
    ADC10CTL0 &= ~ADC10ENC;
    
    /* Set ADC reference and input channel. */
    /* Use internal reference for all channel. */
    ADC10MCTL0 = ADC10SREF_1 + adc_signal;
    
    /* Start sample and convert. */
    ADC10CTL0 |= ADC10SC + ADC10ENC;

    /* Wait for complete. */
    while(((ADC10IFG & ADC10IFG0) != 0x0) && (timeout > 0)) {
        timeout--;
    }

    /* If goes wrong, return error code. */
    if(0x0 == timeout) return ADC_RESULT_TIMEOUT;

    /* Store the result. */
    *result = ADC10MEM0;

    return ADC_RESULT_OK;
}

