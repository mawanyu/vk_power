/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"

/******************************************/
/* Defines and Macros */
/******************************************/
#define PWR_AC_IN_LO        (22000)
#define PWR_AC_IN_HI        (26000)
#define PWR_DC_IN_LO        (18000)
#define PWR_DC_IN_HI        (26000)
#define PWR_BACKUP_BAT_LO   (11000)
#define PWR_BACKUP_BAT_HI0  (24000)
#define PWR_BACKUP_BAT_HI1  (26000)
#define PWR_INTER_BAT_LO    (13000)
#define PWR_INTER_BAT_FULL  (16500)
#define PWR_INTER_BAT_HI    (16800)
#define PWR_FAN_LO          (11000)
#define PWR_FAN_HI          (13000)
#define PWR_MC_OUT_LO       (22800)
#define PWR_MC_OUT_HI       (25200)
#define PWR_UI_OUT_LO       (4750)
#define PWR_UI_OUT_HI       (5250)
#define PWR_CHARGE          (3000)

/******************************************/
/* Variables */
/******************************************/
unsigned int pwr_ac_in;
unsigned int pwr_dc_in;
unsigned int pwr_backup_bat;
unsigned int pwr_inter_bat;
unsigned int pwr_sys_in;
unsigned int pwr_mc_out;
unsigned int pwr_ui_out;
unsigned int pwr_fan;
unsigned int pwr_bat_charge;

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

