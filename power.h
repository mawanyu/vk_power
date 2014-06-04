/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"


/******************************************/
/* Defines and Macros */
/******************************************/
extern unsigned int pwr_warning;

#define PWR_WARNING_FAN             (0x0001)
#define PWR_WARNING_MC              (0x0002)
#define PWR_WARNING_UI              (0x0004)
#define PWR_WARNING_AC_IN           (0x0008)
#define PWR_WARNING_DC_IN           (0x0010)
#define PWR_WARNING_BKBAT_LO       (0x0020)
#define PWR_WARNING_BKBAT_HI       (0x0040)
#define PWR_WARNING_BKBAT_OVER     (0x0080)
#define PWR_WARNING_INBAT_LO        (0x0100)
#define PWR_WARNING_INBAT_FULL      (0x0200)
#define PWR_WARNING_INBAT_OVER      (0x0400)
#define PWR_WARNING_SYS_AC          (0x0800)
#define PWR_WARNING_SYS_DC          (0x1000)
#define PWR_WARNING_SYS_INBAT       (0x2000)

extern unsigned int pwr_info;

#define PWR_INFO_SYS_AC             (0x0001)
#define PWR_INFO_SYS_DC             (0x0002)
#define PWR_INFO_SYS_INBAT          (0x0004)
#define PWR_INFO_CHG             (0x0008)

/******************************************/
/* Variables */
/******************************************/


/******************************************/
/* Functions */
/******************************************/
extern void pwr_initialise(void);
extern void pwr_monitor(void);