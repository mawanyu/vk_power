/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"

/******************************************/
/* Defines and Macros */
/******************************************/

/* Flags of power warnings and alarts */
extern unsigned int pwr_warning;
/* Following are definations of power warnings and alarts */
#define PWR_WARNING_FAN         (0x0001)    /* Fan voltage warning */
#define PWR_WARNING_MC          (0x0002)    /* Main Control Board output voltage warning */
#define PWR_WARNING_UI          (0x0004)    /* UI Control Board output voltage warning */
#define PWR_WARNING_AC_IN       (0x0008)    /* AC 24V voltage warning */
#define PWR_WARNING_DC_IN       (0x0010)    /* DC(backup battery) 20V voltage warning */
#define PWR_WARNING_BKBAT_LO    (0x0020)    /* Backup battery low voltage warning */
#define PWR_WARNING_BKBAT_HI    (0x0040)    /* Backup battery high voltage warning */
#define PWR_WARNING_BKBAT_OVER  (0x0080)    /* Backup battery over voltage warning */
#define PWR_WARNING_INBAT_LO    (0x0100)    /* Internal battery low voltage warning */
//#define PWR_WARNING_INBAT_FULL  (0x0200)    /* Internal battery full warning */
#define PWR_WARNING_INBAT_OVER  (0x0400)    /* Internal battery over voltage warning */
#define PWR_WARNING_SYS_AC      (0x0800)    /* AC to system channel warning */
#define PWR_WARNING_SYS_DC      (0x1000)    /* DC to system channel warning */
#define PWR_WARNING_SYS_INBAT   (0x2000)    /* Internal battery to system channel warning */

/* Flags of power informations */
extern unsigned int pwr_info;
/* Following are definations of power informations */
#define PWR_INFO_SYS_AC         (0x0001)    /* System power sourced by AC 24V */
#define PWR_INFO_SYS_DC         (0x0002)    /* System power sourced by DC(backup battery) 20V */
#define PWR_INFO_SYS_INBAT      (0x0004)    /* System power sourced by internal battery */
#define PWR_INFO_CHG            (0x0008)    /* Internal battery is charging */
#define PWR_INFO_INBAT_FULL     (0x0010)    /* Internal battery full */


/******************************************/
/* Variables */
/******************************************/


/******************************************/
/* Functions */
/******************************************/
extern void pwr_initialise(void);
extern void pwr_monitor(void);

