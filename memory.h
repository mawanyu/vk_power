/*******************************************/
/* Include Files */
/*******************************************/
#include "general.h"

/******************************************/
/* Defines and Macros */
/******************************************/
/* Memory size are calculated as byte. */
#define MEM_SIZE            (2048)
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
void mem_initialise(void);
int mem_read(char * mem_addr,int size,char * rdata);
int mem_write(char * mem_addr,int size,char * wdata);
int mem_erase(char * mem_addr,int size);
int mem_clean(void);

