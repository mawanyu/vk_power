/*******************************************/
/* Include Files */
/*******************************************/
#include "power.h"

/******************************************/
/* Defines and Macros */
/******************************************/
#define PWR_AC_IN_LO        (22000)
#define PWR_AC_IN_HI        (26000)
#define PWR_DC_IN_LO        (18000)
#define PWR_DC_IN_HI        (26000)
#define PWR_BACKUP_BAT_LO   (11000)
#define PWR_BACKUP_BAT_HI   (24000)
#define PWR_BACKUP_BAT_OVER (26000)
#define PWR_INTER_BAT_LO    (13000)
#define PWR_INTER_BAT_FULL  (16500)
#define PWR_INTER_BAT_OVER  (16800)
#define PWR_FAN_LO          (11000)
#define PWR_FAN_HI          (13000)
#define PWR_MC_OUT_LO       (22800)
#define PWR_MC_OUT_HI       (25200)
#define PWR_UI_OUT_LO       (4750)
#define PWR_UI_OUT_HI       (5250)
#define PWR_CHARGE_OVER     (3000)

#define PWR_BAT_CHARGE_PIN  (0x80)

#define VOLTAGE(x)  (x * 2500 * 11 / 0x03FF)

/******************************************/
/* Variables */
/******************************************/
unsigned int pwr_ac_in = 0;
unsigned int pwr_dc_in = 0;
unsigned int pwr_backup_bat = 0;
unsigned int pwr_inter_bat = 0;
unsigned int pwr_sys_in = 0;
unsigned int pwr_mc_out = 0;
unsigned int pwr_ui_out = 0;
unsigned int pwr_fan = 0;
unsigned int pwr_bat_charge = 0;

unsigned int pwr_warning = 0x0;
unsigned int pwr_info = 0x0;

static struct {
    unsigned int low;
    unsigned int high;
} pwr_sys_in_range;

static char pwr_bat_need_charge = 0x0;

/******************************************/
/* Functions */
/******************************************/


void pwr_bat_charge_initialise(void)
{
    /* Select battery charge related pins as GPIO pins(default) */
    P2SEL0 &= ~PWR_BAT_CHARGE_PIN;
    P2SEL1 &= ~PWR_BAT_CHARGE_PIN;

    /* Select battery charge related pins as output */
    P2DIR = PWR_BAT_CHARGE_PIN;

    /* Disable battery charge */
    P2OUT |= PWR_BAT_CHARGE_PIN;
}

#define pwr_bat_charge_enable   (P2OUT &= PWR_BAT_CHARGE_PIN)
#define pwr_bat_charge_disable  (P2OUT |= PWR_BAT_CHARGE_PIN)

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
void pwr_bat_charge_monitor(void)
{
    pwr_bat_charge = adcr_bat_charge * 2500 * 15 / 0x03FF / 10;

    if((pwr_info & PWR_INFO_CHARGE) != 0) {
        if((pwr_bat_charge > PWR_CHARGE_OVER) || 
            (pwr_warning & (PWR_WARNING_INTER_FULL + PWR_WARNING_INTER_OVER) != 0))
        {
            pwr_bat_charge_disable;
            pwr_info &= ~PWR_INFO_CHARGE;
        }
    }
    else if ((pwr_warning & PWR_WARNING_AC_IN) == 0) {

    }
}

void pwr_initialise(void)
{
    pwr_bat_charge_initialise();
    
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

    pwr_backup_bat = VOLTAGE(adcr_backup_bat);
    if(pwr_backup_bat < PWR_BACKUP_BAT_LO) {
        pwr_warning |= PWR_WARNING_BACKUP_LO;
    }
    else if(pwr_backup_bat > PWR_BACKUP_BAT_OVER) {
        pwr_warning |= PWR_WARNING_BACKUP_OVER;
    }
    else if(pwr_backup_bat > PWR_BACKUP_BAT_HI) {
        pwr_warning |= PWR_WARNING_BACKUP_HI;
    }
    else {}

    pwr_sys_in = VOLTAGE(adcr_sys_in);

    pwr_ac_in = VOLTAGE(adcr_ac_in);
    if((pwr_ac_in < PWR_AC_IN_LO) || (pwr_ac_in > PWR_AC_IN_HI)) {
        pwr_warning |= PWR_WARNING_AC_IN;
    }
    else {
        pwr_sys_in_range.low  = pwr_ac_in - 700;
        pwr_sys_in_range.high = pwr_ac_in - 100;
        if((pwr_sys_in < pwr_sys_in_range.low) || (pwr_sys_in > pwr_sys_in_range.high)) {
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
        pwr_sys_in_range.low  = pwr_dc_in - 700;
        pwr_sys_in_range.high = pwr_dc_in - 100;
        if((pwr_sys_in < pwr_sys_in_range.low) || (pwr_sys_in > pwr_sys_in_range.high)) {
            pwr_warning |= PWR_WARNING_SYS_DC;
        }
        else {
            pwr_info_new = PWR_INFO_SYS_DC;
        }
    }
    else {}

    pwr_inter_bat = VOLTAGE(adcr_inter_bat);
    if(pwr_inter_bat < PWR_INTER_BAT_LO) {
        pwr_warning |= PWR_WARNING_INTER_LO;
    }
    else if(pwr_inter_bat > PWR_INTER_BAT_OVER) {
        pwr_warning |= PWR_WARNING_INTER_OVER;
    }
    else {
        if(pwr_inter_bat > PWR_INTER_BAT_FULL) {
            pwr_warning |= PWR_WARNING_INTER_FULL;
        }
        if(pwr_info_new == 0) {
            pwr_sys_in_range.low  = pwr_inter_bat - 700;
            pwr_sys_in_range.high = pwr_inter_bat - 100;
            if((pwr_sys_in < pwr_sys_in_range.low) || (pwr_sys_in > pwr_sys_in_range.high)) {
                pwr_warning |= PWR_WARNING_SYS_INTER;
            }
            else {
                pwr_info_new = PWR_INFO_SYS_INTER;
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

            case PWR_INFO_SYS_INTER: {
                led_btled_on();
                led_acled_off();
                led_dcled_off();
                }
                break;

            default:
                break;
        }
    }
}

