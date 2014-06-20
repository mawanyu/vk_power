/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"

/******************************************/
/* Defines and Macros */
/******************************************/
/* Memory size are calculated as byte. It should be multiple of 512. */
#define MEM_SIZE            (1024)
/* Memory adress align by 512. */
#define MEM_START_ADDR      (0xF000)
#define MEM_END_ADDR        (0xF3FF)

#define MEM_PARAM_INVALID   (-1)

/******************************************/
/* Variables */
/******************************************/


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
extern void mem_initialise(void);
extern int mem_read(char * mem_addr,int size,char * rdata);
extern int mem_write(char * mem_addr,int size,char * wdata);
extern int mem_erase(char * mem_addr,int size);
extern int mem_clean(void);

