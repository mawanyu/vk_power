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
//#define PWR_CHG_NONEED      (0x00)
//#define PWR_CHG_NEED        (0x01)
/* Control pin to MAX1873. 1 to enable charge, 0 to disable.*/
#define CHG_EN_PIN          (BIT7)

/* Translate ADC result to real voltage(mV). */
#define VOLTAGE(x)  ((unsigned long)x * 2500 * 11 / 0x03FF)

/* Voltage value sum times */
#define VSUM_CNT        (10)

/******************************************/
/* Variables */
/******************************************/
/* Followings are to store real voltage(mV) of channels. */
/* They are the average value of 10 times summary. */
unsigned int pwr_ac_in  = 0;        /* AC/DC 24V output voltage */
unsigned int pwr_dc_in  = 0;        /* DC/DC 24V output voltage */
unsigned int pwr_bkbat  = 0;        /* backup battery voltage */
unsigned int pwr_inbat  = 0;        /* internal battery voltage */
unsigned int pwr_sys    = 0;        /* system input voltage */
unsigned int pwr_mc_out = 0;        /* output voltage for Main Control Board */
unsigned int pwr_ui_out = 0;        /* output voltage for UI Board */
unsigned int pwr_fan    = 0;        /* fan voltage */
/* These are temporary value to sum 10Hz sampling value. */
unsigned long pwr_ac_in_t  = 0;        /* AC/DC 24V output voltage */
unsigned long pwr_dc_in_t  = 0;        /* DC/DC 24V output voltage */
unsigned long pwr_bkbat_t  = 0;        /* backup battery voltage */
unsigned long pwr_inbat_t  = 0;        /* internal battery voltage */
unsigned long pwr_sys_t    = 0;        /* system input voltage */
unsigned long pwr_mc_out_t = 0;        /* output voltage for Main Control Board */
unsigned long pwr_ui_out_t = 0;        /* output voltage for UI Board */
unsigned long pwr_fan_t    = 0;        /* fan voltage */
/* Voltage value sum count. */
static char pwr_vcnt = 0;

/* Internal battery charge current(mA) */
unsigned int pwr_charge = 0;

/* Flags of power warnings and alarts */
unsigned int pwr_warning = 0x0;
/* Flags of power informations */
unsigned int pwr_info = 0x0;

/* Flag indicates if internal battery need charge or not */
//static char pwr_need_charge = PWR_CHG_NONEED;

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
    /* Clear related warning flags */
    pwr_warning &= ~PWR_WARNING_INBAT_OVER;

    /* Calculate internal battery charge current(mA). */ 
    pwr_charge = (unsigned int)((unsigned long)adcr_charge * 2500 * 15 / 0x03FF / 10);

    /* Internal battery is charging. If it is full, or over charge, */
    /* or AC input is incorrect, stop charge. */
    if((pwr_info & PWR_INFO_CHG) != 0) {
        /* over current, or AC input is incorrect */
        if((pwr_charge > PWR_CHG_OVER) || ((pwr_warning & PWR_WARNING_AC_IN) != 0))
        {
            pwr_charge_disable;
            /* update information flag */
            pwr_info &= ~PWR_INFO_CHG;
        }
        /* battery full */
        else if(pwr_inbat > PWR_INBAT_FULL) {
            pwr_charge_disable;
            /* update information flag */
            pwr_info &= ~PWR_INFO_CHG;
            pwr_info |= PWR_INFO_INBAT_FULL;
        }
        /* over voltage */
        else if(pwr_inbat > PWR_INBAT_OVER) {
            pwr_charge_disable;
            /* update information flag */
            pwr_info &= ~PWR_INFO_CHG;
            /* set warning flag */
            pwr_warning |= PWR_WARNING_INBAT_OVER;
        }
    }
    /* Internal battery is not charging. If it needs charge and AC input is */
    /* correct, start charge. */
    else {
        /* AC input is correct */
        if((pwr_warning & PWR_WARNING_AC_IN) == 0) {
            /* battery needs to be charged */
            if(pwr_inbat < PWR_CHG_LO) {
                pwr_charge_enable;
                /* update information flag */
                pwr_info |= PWR_INFO_CHG;
                pwr_info &= ~PWR_INFO_INBAT_FULL;
            }
        }
        /* AC input is incorrect, just update information */
        else {
            pwr_info &= ~PWR_INFO_INBAT_FULL;
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
static char pwr_translate(void)
{
    /* Sum the voltage values. */
    pwr_fan_t += VOLTAGE(adcr_fan);
    pwr_ui_out_t += VOLTAGE(adcr_ui_out);
    pwr_mc_out_t += VOLTAGE(adcr_mc_out);
    pwr_bkbat_t += VOLTAGE(adcr_bkbat);
    pwr_sys_t += VOLTAGE(adcr_sys_in);
    pwr_ac_in_t += VOLTAGE(adcr_ac_in);
    pwr_dc_in_t += VOLTAGE(adcr_dc_in);
    pwr_inbat_t += VOLTAGE(adcr_inbat);
    /* Increase count number. */
    pwr_vcnt++;

    /* Sum 10 times. */
    if(VSUM_CNT == pwr_vcnt) {
    //if(1 == pwr_vcnt) {
        /* Get the average value. */
        pwr_fan = (unsigned int)(pwr_fan_t / VSUM_CNT);
        pwr_ui_out = (unsigned int)(pwr_ui_out_t / VSUM_CNT);
        pwr_mc_out = (unsigned int)(pwr_mc_out_t / VSUM_CNT);
        pwr_bkbat = (unsigned int)(pwr_bkbat_t / VSUM_CNT);
        pwr_sys = (unsigned int)(pwr_sys_t / VSUM_CNT);
        pwr_ac_in = (unsigned int)(pwr_ac_in_t / VSUM_CNT);
        pwr_dc_in = (unsigned int)(pwr_dc_in_t / VSUM_CNT);
        pwr_inbat = (unsigned int)(pwr_inbat_t / VSUM_CNT);
        /* Clear temporary value. */
        pwr_ac_in_t  = 0;
        pwr_dc_in_t  = 0;
        pwr_bkbat_t  = 0;
        pwr_inbat_t  = 0;
        pwr_sys_t    = 0;
        pwr_mc_out_t = 0;
        pwr_ui_out_t = 0;
        pwr_fan_t    = 0;
        /* Clear count number. */
        pwr_vcnt = 0;

        /* Return OK. */
        return 0x1;
    }

    /* Return NOT OK. */
    return 0x0;
}


#define PRI_AC      (0x00)
#define PRI_DC      (0x01)
#define PRI_INBAT   (0x02)
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
void pwr_detect_source(void)
{
    /*The power sources priority. High voltage source has high priority. */
    struct {
        char first;     /* Highest priority */
        char second;
        char third;     /* Lowest priority */
    } source_pri;

    unsigned int diff_v_ac = 0;         /* AC channel drop voltage */
    unsigned int diff_v_dc = 0;         /* DC(backup battery) channel drop voltage */
    unsigned int diff_v_inbat = 0;      /* Internal battery channel drop voltage */
    
    unsigned int pwr_info_new = 0x0;    /* Temp save of power information flags */

    /* Clear power channel warning flags. */
    pwr_warning &= ~(PWR_WARNING_SYS_AC + PWR_WARNING_SYS_DC + PWR_WARNING_SYS_INBAT);

    /* Judge power sources priority. */
    if((pwr_ac_in > pwr_dc_in) && (pwr_ac_in > pwr_inbat)) {
        source_pri.first = PRI_AC;
        if(pwr_dc_in > pwr_inbat) {
            source_pri.second = PRI_DC;
            source_pri.third = PRI_INBAT;
        }
        else {
            source_pri.second = PRI_INBAT;
            source_pri.third = PRI_DC;
        }
    }
    else if ((pwr_ac_in < pwr_dc_in) && (pwr_ac_in > pwr_inbat)) {
        source_pri.second = PRI_AC;
        source_pri.first = PRI_DC;
        source_pri.third = PRI_INBAT;
    }
    else if ((pwr_ac_in > pwr_dc_in) && (pwr_ac_in < pwr_inbat)) {
        source_pri.second = PRI_AC;
        source_pri.first = PRI_INBAT;
        source_pri.third = PRI_DC;
    }
    else {
        source_pri.third = PRI_AC;
        if(pwr_dc_in > pwr_inbat) {
            source_pri.first = PRI_DC;
            source_pri.second = PRI_INBAT;
        }
        else {
            source_pri.first = PRI_INBAT;
            source_pri.second = PRI_DC;
        }
    }

    /* Get drop voltage of channels. */
    diff_v_ac = pwr_ac_in - pwr_sys;
    diff_v_dc = pwr_dc_in - pwr_sys;
    diff_v_inbat = pwr_inbat - pwr_sys;

    /* Judge which source supplies the system voltage. */
    /* If drop voltage is in correct range(0mV-700mV), this source supplies */
    /* the system. Then if this source is not the highest priority, the higher */
    /* priority source channels are incorrect, the warning flags should be set. */
    /* System is supplied by AC */
    if((diff_v_ac > 0) && (diff_v_ac < 700)) {
        pwr_info_new |= PWR_INFO_SYS_AC;
        /* If DC is highest priority, it is incorrect. */
        if(PRI_DC == source_pri.first) {
            pwr_warning |= PWR_WARNING_SYS_DC;
            /* If internal battery is second priority, it is incorrect. */
            if(PRI_INBAT == source_pri.second) {
                pwr_warning |= PWR_WARNING_SYS_INBAT;
            }
        }
        /* If internal battery is highest priority, it is incorrect. */
        else if(PRI_INBAT == source_pri.first) {
            pwr_warning |= PWR_WARNING_SYS_INBAT;
            /* If DC is second priority, it is incorrect. */
            if(PRI_DC == source_pri.second) {
                pwr_warning |= PWR_WARNING_SYS_DC;
            }
        }
    }
    /* System is supplied by DC */
    else if((diff_v_dc > 0) && (diff_v_dc < 700)) {
        pwr_info_new |= PWR_INFO_SYS_DC;
        /* If AC is highest priority, it is incorrect. */
        if(PRI_AC == source_pri.first) {
            pwr_warning |= PWR_WARNING_SYS_AC;
            /* If internal battery is second priority, it is incorrect. */
            if(PRI_INBAT == source_pri.second) {
                pwr_warning |= PWR_WARNING_SYS_INBAT;
            }
        }
        /* If internal battery is highest priority, it is incorrect. */
        else if(PRI_INBAT == source_pri.first) {
            pwr_warning |= PWR_WARNING_SYS_INBAT;
            /* If AC is second priority, it is incorrect. */
            if(PRI_AC == source_pri.second) {
                pwr_warning |= PWR_WARNING_SYS_AC;
            }
        }
    }
    /* System is supplied by internal battery */
    else if((diff_v_inbat > 0) && (diff_v_inbat < 700)) {
        pwr_info_new |= PWR_INFO_SYS_INBAT;
        /* If AC is highest priority, it is incorrect. */
        if(PRI_AC == source_pri.first) {
            pwr_warning |= PWR_WARNING_SYS_AC;
            /* If DC is second priority, it is incorrect. */
            if(PRI_DC == source_pri.second) {
                pwr_warning |= PWR_WARNING_SYS_DC;
            }
        }
        /* If DC is highest priority, it is incorrect. */
        else if(PRI_DC == source_pri.first) {
            pwr_warning |= PWR_WARNING_SYS_DC;
            /* If AC is second priority, it is incorrect. */
            if(PRI_AC == source_pri.second) {
                pwr_warning |= PWR_WARNING_SYS_AC;
            }
        }
    }
    else {
        int dummy;
        dummy = 1;
    }

    /* If power information of source is changed, then refresh the information flags, */
    /* and change the LED. */
    if(pwr_info_new != 
       (pwr_info & (PWR_INFO_SYS_AC + PWR_INFO_SYS_DC + PWR_INFO_SYS_INBAT)))
    {
        pwr_info &= ~(PWR_INFO_SYS_AC + PWR_INFO_SYS_DC + PWR_INFO_SYS_INBAT);
        pwr_info |= pwr_info_new;
        switch(pwr_info_new) {
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

            default: {
                    led_acled_off();
                    led_dcled_off();
                    led_btled_off();
            }
            break;
        }
    }
    
#if 0
    /* If AC is highest priority */
    if(source_pri.first == PRI_AC) {
        /* Get drop voltage */
        diff_v = pwr_ac_in - pwr_sys;
        /* If it is valid, the system voltage is sourced by AC. */
        if((diff_v > 100) && (diff_v < 700)) {
            pwr_info_new |= PWR_INFO_SYS_AC;
        }
        /* If it isn't valid, mark the source as error. */
        /* Then check the next priority. */
        else {
            pwr_warning |= PWR_WARNING_SYS_AC;
            /* If DC is second priority */
            if(source_pri.second == PRI_DC) {
                /* Get drop voltage */
                diff_v = pwr_dc_in - pwr_sys;
                /* If it is valid, the system voltage is sourced by DC(backup battery). */
                if((diff_v > 100) && (diff_v < 700)) {
                    pwr_info_new |= PWR_INFO_SYS_DC;
                }
                /* If it isn't valid, mark the source as error. */
                /* Then check the next priority. */
                else {
                    pwr_warning |= PWR_WARNING_DC_IN;
                    /* Get drop voltage */
                    diff_v = pwr_inbat - pwr_sys;
                    /* If it is valid, the system voltage is sourced by internal battery. */
                    if((diff_v > 100) && (diff_v < 700)) {
                        pwr_info_new |= PWR_INFO_SYS_INBAT;
                    }
                    /* If it isn't valid, mark the source as error. */
                    else {
                        pwr_warning |= PWR_WARNING_SYS_INBAT;
                    }
                }
            }
            /* If internal battery is second priority */
            else {
            }
        }
    }
#endif
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
    /* Get real voltages. It is average of 10 times summary. */
    /* When it is not ready, just do nothing. */
    if(0x0 == pwr_translate()) {
        return;
    }

    /* Detect which source supplies the system. */
    pwr_detect_source();

    /* Clear warning flags. */
    pwr_warning &= ~(PWR_WARNING_FAN + PWR_WARNING_MC + PWR_WARNING_UI +
                     PWR_WARNING_AC_IN + PWR_WARNING_DC_IN + PWR_WARNING_BKBAT_LO +
                     PWR_WARNING_BKBAT_HI + PWR_WARNING_BKBAT_OVER +
                     PWR_WARNING_INBAT_LO);

    /* Check if fan voltage is correct. */
    if((pwr_fan < PWR_FAN_LO) || (pwr_fan > PWR_FAN_HI)) {
        pwr_warning |= PWR_WARNING_FAN;
    }

    /* Check if output voltage for UI Board is correct. */
    if((pwr_ui_out < PWR_UI_OUT_LO) || (pwr_ui_out > PWR_UI_OUT_HI)) {
        pwr_warning |= PWR_WARNING_UI;
    }

    /* Check if output voltage for Main Control Board is correct. */
    if((pwr_mc_out < PWR_MC_OUT_LO) || (pwr_mc_out > PWR_MC_OUT_HI)) {
        pwr_warning |= PWR_WARNING_MC;
    }

    /* Check if backup battery voltage is correct. */
    if(pwr_bkbat < PWR_BKBAT_LO) {
        pwr_warning |= PWR_WARNING_BKBAT_LO;
    }
    else if(pwr_bkbat > PWR_BKBAT_OVER) {
        pwr_warning |= PWR_WARNING_BKBAT_OVER;
    }
    else if(pwr_bkbat > PWR_BKBAT_HI) {
        pwr_warning |= PWR_WARNING_BKBAT_HI;
    }

    /* Check if AC 24V input voltage is correct. */
    if((pwr_ac_in < PWR_AC_IN_LO) || (pwr_ac_in > PWR_AC_IN_HI)) {
        pwr_warning |= PWR_WARNING_AC_IN;
    }

    /* Check if DC(backup battery) 20V input voltage is correct. */
    if((pwr_dc_in < PWR_DC_IN_LO) || (pwr_dc_in > PWR_DC_IN_HI)) {
        pwr_warning |= PWR_WARNING_DC_IN;
    }

    /* Check if internal battery voltage is correct. */
    if(pwr_inbat < PWR_INBAT_LO) {
        pwr_warning |= PWR_WARNING_INBAT_LO;
    }

    /* Monitor internal battery charging. */
    pwr_charge_monitor();
}

