/*******************************************/
/* Include Files */
/*******************************************/
#include "power.h"
#include "led.h"
#include "adc.h"

/******************************************/
/* Defines and Macros */
/******************************************/
/* These definations are to determine if there is a voltage error. */
/* They are defined by mV. */
/* Following is valid range of AC/DC 24V output voltage */
#define PWR_AC_IN_LO        (22000)     /* AC input low threshold */
#define PWR_AC_IN_HI        (26000)     /* AC input high threshold */
/* Following is valid range of DC/DC 24V output voltage */
#define PWR_DC_IN_LO        (18000)     /* DC input low threshold */
#define PWR_DC_IN_HI        (26000)     /* DC input high threshold */
/* Following is valid range of backup battery voltage */
#define PWR_BKBAT_LO        (11000)     /* backup battery low threshold */
#define PWR_BKBAT_HI        (24000)     /* backup battery high threshold */
#define PWR_BKBAT_OVER      (26000)     /* backup battery over voltage threshold */
/* Following is valid range of internal battery voltage */
#define PWR_INBAT_LO        (13000)     /* internal battery low threshold */
#define PWR_INBAT_FULL      (16500)     /* internal battery high threshold */
#define PWR_INBAT_OVER      (17000)     /* internal battery over voltage threshold */
/* Following is valid range of fan voltage */
#define PWR_FAN_LO          (11000)     /* fan low threshold */
#define PWR_FAN_HI          (13000)     /* fan high threshold */
/* Following is valid range of output voltage for Main Control Board */
#define PWR_MC_OUT_LO       (22800)     /* MC output low threshold */
#define PWR_MC_OUT_HI       (25200)     /* MC output high threshold */
/* Following is valid range of output voltage for UI Board */
#define PWR_UI_OUT_LO       (4750)     /* UI output low threshold */
#define PWR_UI_OUT_HI       (5250)     /* UI output high threshold */

/* If internal battery voltage is less than this threshold(mV), it needs to be charged. */
#define PWR_CHG_LO          (15100)
/* This is max charge current(mA) for internal battery. */
#define PWR_CHG_OVER        (3000)
/* Definations of internal battery charge flag. */
#define PWR_CHG_NONEED      (0x00)
#define PWR_CHG_NEED        (0x01)
/* Control pin to MAX1873. 1 to enable charge, 0 to disable.*/
#define CHG_EN_PIN          (BIT7)

/* Translate ADC result to real voltage(mV). */
#define VOLTAGE(x)  (unsigned int)((unsigned long)x * 2500 * 11 / 0x03FF)

/******************************************/
/* Variables */
/******************************************/
/* Followings are to store real voltage(mV) of channels. */
unsigned int pwr_ac_in  = 0;        /* AC/DC 24V output voltage */
unsigned int pwr_dc_in  = 0;        /* DC/DC 24V output voltage */
unsigned int pwr_bkbat  = 0;        /* backup battery voltage */
unsigned int pwr_inbat  = 0;        /* internal battery voltage */
unsigned int pwr_sys    = 0;        /* system input voltage */
unsigned int pwr_mc_out = 0;        /* output voltage for Main Control Board */
unsigned int pwr_ui_out = 0;        /* output voltage for UI Board */
unsigned int pwr_fan    = 0;        /* fan voltage */

/* Internal battery charge current(mA) */
unsigned int pwr_charge = 0;

/* Flags of power warnings and alarts */
unsigned int pwr_warning = 0x0;
/* Flags of power informations */
unsigned int pwr_info = 0x0;

static struct {
    unsigned int low;
    unsigned int high;
} pwr_sys_range;

/* Flag indicates if internal battery need charge or not */
static char pwr_need_charge = PWR_CHG_NONEED;

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
void pwr_charge_initialise(void)
{
    /* Select battery charge related pins as GPIO pins(default) */
    P2SEL0 &= ~CHG_EN_PIN;
    P2SEL1 &= ~CHG_EN_PIN;

    /* Select battery charge related pins as output */
    P2DIR = CHG_EN_PIN;

    /* Disable battery charge */
    P2OUT |= CHG_EN_PIN;
}

#define pwr_charge_enable   (P2OUT &= CHG_EN_PIN)
#define pwr_charge_disable  (P2OUT |= CHG_EN_PIN)

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
void pwr_charge_monitor(void)
{
    /* Calculate internal battery charge current(mA). */ 
    pwr_charge = (unsigned int)((unsigned long)adcr_charge * 2500 * 15 / 0x03FF / 10);

    /* Internal battery is charging. If it is full, or over charge, */
    /* or AC input is incorrect, stop charge. */
    if((pwr_info & PWR_INFO_CHG) != 0) {
        if((pwr_charge > PWR_CHG_OVER) || 
            ((pwr_warning & PWR_WARNING_INBAT_FULL) != 0) ||
            ((pwr_warning & PWR_WARNING_INBAT_OVER) != 0) ||
            ((pwr_warning & PWR_WARNING_AC_IN) != 0))
        {
            pwr_charge_disable;
            /* update information flag */
            pwr_info &= ~PWR_INFO_CHG;
        }
    }
    /* Internal battery is not charging. If it needs charge and AC input is */
    /* correct, start charge. */
    else {
        if(((pwr_warning & PWR_WARNING_AC_IN) == 0) &&
            (pwr_need_charge == PWR_CHG_NEED))
        {
            pwr_charge_enable;
            /* update information flag */
            pwr_info |= PWR_INFO_CHG;
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
void pwr_initialise(void)
{
    pwr_charge_initialise();
    
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
void pwr_monitor(void)
{
    unsigned int pwr_info_new = 0x0;

    pwr_warning = 0x0;

    pwr_fan = VOLTAGE(adcr_fan);
    if((pwr_fan < PWR_FAN_LO) || (pwr_fan > PWR_FAN_HI)) {
        pwr_warning |= PWR_WARNING_FAN;
    }

    pwr_ui_out = VOLTAGE(adcr_ui_out);
    if((pwr_ui_out < PWR_UI_OUT_LO) || (pwr_ui_out > PWR_UI_OUT_HI)) {
        pwr_warning |= PWR_WARNING_UI;
    }

    pwr_mc_out = VOLTAGE(adcr_mc_out);
    if((pwr_mc_out < PWR_MC_OUT_LO) || (pwr_mc_out > PWR_MC_OUT_HI)) {
        pwr_warning |= PWR_WARNING_MC;
    }

    pwr_bkbat = VOLTAGE(adcr_bkbat);
    if(pwr_bkbat < PWR_BKBAT_LO) {
        pwr_warning |= PWR_WARNING_BKBAT_LO;
    }
    else if(pwr_bkbat > PWR_BKBAT_OVER) {
        pwr_warning |= PWR_WARNING_BKBAT_OVER;
    }
    else if(pwr_bkbat > PWR_BKBAT_HI) {
        pwr_warning |= PWR_WARNING_BKBAT_HI;
    }
    else {}

    pwr_sys = VOLTAGE(adcr_sys_in);

    pwr_ac_in = VOLTAGE(adcr_ac_in);
    if((pwr_ac_in < PWR_AC_IN_LO) || (pwr_ac_in > PWR_AC_IN_HI)) {
        pwr_warning |= PWR_WARNING_AC_IN;
    }
    else {
        pwr_sys_range.low  = pwr_ac_in - 700;
        pwr_sys_range.high = pwr_ac_in - 100;
        if((pwr_sys < pwr_sys_range.low) || (pwr_sys > pwr_sys_range.high)) {
            pwr_warning |= PWR_WARNING_SYS_AC;
        }
        else {
            pwr_info_new = PWR_INFO_SYS_AC;
        }
    }

    pwr_dc_in = VOLTAGE(adcr_dc_in);
    if((pwr_dc_in < PWR_DC_IN_LO) || (pwr_dc_in > PWR_DC_IN_HI)) {
        pwr_warning |= PWR_WARNING_DC_IN;
    }
    else if(pwr_info_new == 0) {
        pwr_sys_range.low  = pwr_dc_in - 700;
        pwr_sys_range.high = pwr_dc_in - 100;
        if((pwr_sys < pwr_sys_range.low) || (pwr_sys > pwr_sys_range.high)) {
            pwr_warning |= PWR_WARNING_SYS_DC;
        }
        else {
            pwr_info_new = PWR_INFO_SYS_DC;
        }
    }
    else {}

    pwr_inbat = VOLTAGE(adcr_inbat);
    if(pwr_inbat < PWR_INBAT_LO) {
        pwr_warning |= PWR_WARNING_INBAT_LO;
        pwr_need_charge = PWR_CHG_NEED;
    }
    else if(pwr_inbat < PWR_CHG_LO) {
        pwr_need_charge = PWR_CHG_NEED;
    }
    else if(pwr_inbat > PWR_INBAT_OVER) {
        pwr_warning |= PWR_WARNING_INBAT_OVER;
    }
    else {
        if(pwr_inbat > PWR_INBAT_FULL) {
            pwr_warning |= PWR_WARNING_INBAT_FULL;
        }
        if(pwr_info_new == 0) {
            pwr_sys_range.low  = pwr_inbat - 700;
            pwr_sys_range.high = pwr_inbat - 100;
            if((pwr_sys < pwr_sys_range.low) || (pwr_sys > pwr_sys_range.high)) {
                pwr_warning |= PWR_WARNING_SYS_INBAT;
            }
            else {
                pwr_info_new = PWR_INFO_SYS_INBAT;
            }
        }
    }

    if(pwr_info != pwr_info_new) {
        pwr_info = pwr_info_new;
        switch(pwr_info) {
            case PWR_INFO_SYS_AC: {
                led_acled_on();
                led_dcled_off();
                led_btled_off();
                }
                break;

            case PWR_INFO_SYS_DC: {
                led_dcled_on();
                led_acled_off();
                led_btled_off();
                }
                break;

            case PWR_INFO_SYS_INBAT: {
                led_btled_on();
                led_acled_off();
                led_dcled_off();
                }
                break;

            default:
                break;
        }
    }

    pwr_charge_monitor();
}

