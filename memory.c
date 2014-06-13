/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"

/******************************************/
/* Defines and Macros */
/******************************************/
#define MEM_SIZE            (2048)
#define MEM_START_ADDR      (0xF000)
#define MEM_END_ADDR        (0xF3FF)

#define MPU_SEG1_PAGE       (0x10)
#define MPU_SEG2_PAGE       (0x14)

/******************************************/
/* Variables */
/******************************************/
volatile char * mem_pdb = 0x0;
volatile unsigned int * mem_pdw = 0x0;

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
void mem_initialise(void)
{
    /* Enable access MPU registers and enable MPU. */
    MPUCTL0 = MPUPW + MPUENA;

    /* Set segment1 and segment2 border. */
    MPUSEG = (MPU_SEG2_PAGE << 8) + MPU_SEG1_PAGE;

    /* Set segments access control. */
    /* Segment2 is used for store data. */
    /* Segment1 and segment3 are reserved for system. */
    
}
