/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"

/******************************************/
/* Defines and Macros */
/******************************************/
/* Reflect ADC input signal to channel */
#define ADC_CH_CHG          (ADC10INCH_1)   /* Battery charge I channel */
#define ADC_CH_FAN          (ADC10INCH_2)   /* Fan V channel */
#define ADC_CH_INBAT        (ADC10INCH_3)   /* Internal battery V channel */
#define ADC_CH_MC_OUT       (ADC10INCH_4)   /* Output V channel for Main Control Board */
#define ADC_CH_AC_IN        (ADC10INCH_5)   /* Input V channel from 24V AC/DC */
#define ADC_CH_DC_IN        (ADC10INCH_12)  /* Input V channel from 20V DC/DC(backup battery) */
#define ADC_CH_SYS          (ADC10INCH_13)  /* Input V channel of system, one of AC/DC, DC/DC and internel battery */
#define ADC_CH_BKBAT        (ADC10INCH_14)  /* Backup battery V channel */
#define ADC_CH_UI_OUT       (ADC10INCH_15)  /* Output V channel for UI Board */

/* Definations of return code */
#define ADC_RESULT_OK       (0x00)
#define ADC_RESULT_TIMEOUT  (0x01)

/* Definations of ADC complete flag */
#define ADC_COMPLETE_NONE   (0x00)
#define ADC_COMPLETE_ALL    (0x01)

/******************************************/
/* Variables */
/******************************************/
/* Following are ADC result of certain signals */
extern unsigned int adcr_charge;    /* battery charge I */
extern unsigned int adcr_fan;           /* fan V */
extern unsigned int adcr_inbat;         /* internal battery V */
extern unsigned int adcr_bkbat;         /* backup battery V */
extern unsigned int adcr_ac_in;         /* 24V AC/DC input V */
extern unsigned int adcr_dc_in;         /* 20V DC/DC(backup battery) input V*/
extern unsigned int adcr_sys_in;        /* system DC input V(one of AD/DC, DC/DC, internal battery) */
extern unsigned int adcr_mc_out;        /* output V for Main Control Board */
extern unsigned int adcr_ui_out;        /* output V for UI Board */

/* This flag indicates all ADC channel have been converted */
extern char adc_complete_flag;

/******************************************/
/* Functions */
/******************************************/
extern void adc_initialise(void);
extern unsigned int adc_start(unsigned int adc_signal, unsigned int * result,
                                unsigned int timeout);
