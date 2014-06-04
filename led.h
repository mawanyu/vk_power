/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"

/******************************************/
/* Defines and Macros */
/******************************************/
#define LED_FLASH_ON    (0x1u)
#define LED_FLASH_OFF   (0x0u)

/******************************************/
/* Variables */
/******************************************/
/* indicate the red alart LED is flashing or not */
extern char aled_r_flash_flag;
/* indicate the yellow alart LED is flashing or not */
extern char aled_y_flash_flag;

/******************************************/
/* Functions */
/******************************************/
/* LEDs initialise */
extern void led_initialise(void);
/* AC indication LED control functions */
extern void led_acled_on(void);
extern void led_acled_off(void);
/* DC indication LED control functions */
extern void led_dcled_on(void);
extern void led_dcled_off(void);
/* Battery indication LED control functions */
extern void led_btled_on(void);
extern void led_btled_off(void);
/* Alart red LED control functions */
extern void led_aled_r_on(void);
extern void led_aled_r_off(void);
extern void led_toggle_aled_r(void);
extern void led_flash_aled_r(unsigned int switcher);
/* Alart yellow LED control functions */
extern void led_aled_y_on(void);
extern void led_aled_y_off(void);
extern void led_toggle_aled_y(void);
extern void led_flash_aled_y(unsigned int switcher);

