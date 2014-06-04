/*******************************************/
/* Include Files */
/*******************************************/
#include "power.h"
#include "led.h"
#include "adc.h"

/******************************************/
/* Defines and Macros */
/******************************************/
#define PWR_AC_IN_LO        (22000)
#define PWR_AC_IN_HI        (26000)
#define PWR_DC_IN_LO        (18000)
#define PWR_DC_IN_HI        (26000)
#define PWR_BKBAT_LO        (11000)
#define PWR_BKBAT_HI        (24000)
#define PWR_BKBAT_OVER      (26000)
#define PWR_INBAT_LO        (13000)
#define PWR_INBAT_FULL      (16500)
#define PWR_INBAT_OVER      (16800)
#define PWR_FAN_LO          (11000)
#define PWR_FAN_HI          (13000)
#define PWR_MC_OUT_LO       (22800)
#define PWR_MC_OUT_HI       (25200)
#define PWR_UI_OUT_LO       (4750)
#define PWR_UI_OUT_HI       (5250)

#define PWR_CHG_LO          (15100)
#define PWR_CHG_OVER        (3000)
#define PWR_CHG_NONEED      (0x00)
#define PWR_CHG_NEED        (0x01)

#define CHG_EN_PIN          (0x80)

#define VOLTAGE(x)  (x * 2500 * 11 / 0x03FF)

/******************************************/
/* Variables */
/******************************************/
unsigned int pwr_ac_in = 0;
unsigned int pwr_dc_in = 0;
unsigned int pwr_bkbat = 0;
unsigned int pwr_inbat = 0;
unsigned int pwr_sys = 0;
unsigned int pwr_mc_out = 0;
unsigned int pwr_ui_out = 0;
unsigned int pwr_fan = 0;

unsigned int pwr_charge = 0;

unsigned int pwr_warning = 0x0;
unsigned int pwr_info = 0x0;

static struct {
    unsigned int low;
    unsigned int high;
} pwr_sys_range;

static char pwr_need_charge = PWR_CHG_NONEED;

/******************************************/
/* Functions */
/******************************************/

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
    pwr_charge = adcr_charge * 2500 * 15 / 0x03FF / 10;

    if((pwr_info & PWR_INFO_CHG) != 0) {
        if((pwr_charge > PWR_CHG_OVER) || 
            ((pwr_warning & PWR_WARNING_INBAT_FULL) != 0) ||
            ((pwr_warning & PWR_WARNING_INBAT_OVER) != 0) ||
            ((pwr_warning & PWR_WARNING_AC_IN) != 0))
        {
            pwr_charge_disable;
            pwr_info &= ~PWR_INFO_CHG;
        }
    }
    else {
        if(((pwr_warning & PWR_WARNING_AC_IN) == 0) &&
            (pwr_need_charge == PWR_CHG_NEED))
        {
            pwr_charge_enable;
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

