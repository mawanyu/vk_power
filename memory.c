/*******************************************/
/* Include Files */
/*******************************************/
#include "memory.h"

/******************************************/
/* Defines and Macros */
/******************************************/
/* These are pages in whole memory, related to memory address. */
/* Refer to MPU chapter of user manual. */
/* Note that they should change when memory address are modified. */
#define MPU_SEG1_PAGE       (0x10)
#define MPU_SEG2_PAGE       (0x14)

/******************************************/
/* Variables */
/******************************************/
//volatile char * mem_pdb = 0x0;
//volatile unsigned int * mem_pdw = 0x0;

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
    MPUSAM = MPUSEG1RE + MPUSEG1XE +
             MPUSEG2RE + MPUSEG2WE +
             MPUSEG3RE + MPUSEG3XE +
             MPUSEGIRE + MPUSEGIXE + MPUSEGIWE;

    /* Initialise byte data pointer and word data pointer. All to start address. */
    //mem_pdb = MEM_START_ADDR;
    //mem_pdw = MEM_START_ADDR;
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
int mem_read(char* mem_addr, int size, char* rdata)
{
    int rcnt = 0;
    char *src_addr = mem_addr;
    char *dst_addr = rdata;
    
    /* Check input validation */
    if((src_addr < (char*)MEM_START_ADDR) || (src_addr > (char*)MEM_END_ADDR)) {
        return MEM_PARAM_INVALID;
    }
    
    if((size == 0) || (size > ((char*)MEM_END_ADDR - src_addr + 1))) {
        return MEM_PARAM_INVALID;
    }

    /* Read data from memory */
    while(rcnt < size) {
        *(dst_addr++) = *(src_addr++);
        rcnt++;
    }
    return rcnt;
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
int mem_write(char* mem_addr, int size, char* wdata)
{
    int wcnt = 0;
    char *src_addr = wdata;
    char *dst_addr = mem_addr;
    
    /* Check input validation */
    if((dst_addr < (char*)MEM_START_ADDR) || (dst_addr > (char*)MEM_END_ADDR)) {
        return MEM_PARAM_INVALID;
    }
    
    if((size == 0) || (size > ((char*)MEM_END_ADDR - dst_addr + 1))) {
        return MEM_PARAM_INVALID;
    }

    /* Write data to memory */
    while(wcnt < size) {
        *(dst_addr++) = *(src_addr++);
        wcnt++;
    }
    return wcnt;
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
int mem_erase(char* mem_addr, int size)
{
    int ecnt = 0;
    char* ers_addr = mem_addr;

    /* Check input validation */
    if((ers_addr < (char*)MEM_START_ADDR) || (ers_addr > (char*)MEM_END_ADDR)) {
        return MEM_PARAM_INVALID;
    }
    
    if((size == 0) || (size > ((char*)MEM_END_ADDR - ers_addr + 1))) {
        return MEM_PARAM_INVALID;
    }
    
    /* Erase whole valid memory */
    while(ecnt < size) {
        *(ers_addr++) = 0x00;
        ecnt++;
    }
    return ecnt;
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
int mem_clean(void)
{
    return mem_erase((char*)MEM_START_ADDR, MEM_SIZE);
}


