/*----------------------------------------------------------------------------	*/
/*         ATMEL Microcontroller Software Support  -  ROUSSET  -		*/
/*----------------------------------------------------------------------------	*/
/* The software is delivered "AS IS" without warranty or condition of any	*/
/* kind, either express, implied or statutory. This includes without		*/
/* limitation any warranty or condition with respect to merchantability or	*/
/* fitness for any particular purpose, or against the infringements of		*/
/* intellectual property rights of others.					*/
/*----------------------------------------------------------------------------	*/
/* File Name           : init.c							*/
/* Object              : Low level initialisations written in C			*/
/* Creation            : ODi   06/26/2002					*/
/*----------------------------------------------------------------------------	*/

#include "main.h"

#define AT91C_BASE_SDRAM	0x20000000
#define AT91C_MASTER_CLOCK	48000000
#define AT91C_BAUD_RATE		115200

/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_DBGU_Printk							*/
/* \brief This function is used to send a string through the DBGU channel (Very low level debugging) */
/*----------------------------------------------------------------------------	*/
void AT91F_DBGU_Printk(
	char *buffer) // \arg pointer to a string ending by \0
{
	while(*buffer != '\0') {
		while (!AT91F_US_TxReady((AT91PS_USART)AT91C_BASE_DBGU));
		AT91F_US_PutChar((AT91PS_USART)AT91C_BASE_DBGU, *buffer++);
	}
}

/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_DataAbort							*/
/* \brief This function reports an Abort					*/
/*----------------------------------------------------------------------------	*/
void AT91F_SpuriousHandler() 
{
	AT91F_DBGU_Printk("-F- Spurious Interrupt detected\n\r");
	while (1);
}


/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_DataAbort							*/
/* \brief This function reports an Abort					*/
/*----------------------------------------------------------------------------	*/
void AT91F_DataAbort() 
{
	AT91F_DBGU_Printk("-F- Data Abort detected\n\r");
	while (1);
}

/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_FetchAbort							*/
/* \brief This function reports an Abort					*/
/*----------------------------------------------------------------------------	*/
void AT91F_FetchAbort()
{
	AT91F_DBGU_Printk("-F- Prefetch Abort detected\n\r");
	while (1);
}

/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_Undef								*/
/* \brief This function reports an Abort					*/
/*----------------------------------------------------------------------------	*/
void AT91F_Undef() 
{
	AT91F_DBGU_Printk("-F- Undef detected\n\r");
	while (1);
}

/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_UndefHandler							*/
/* \brief This function reports that no handler have been set for current IT	*/
/*----------------------------------------------------------------------------	*/
void AT91F_UndefHandler() 
{
	AT91F_DBGU_Printk("-F- UndefH detected\n\r");
	while (1);
}


/*--------------------------------------------------------------------------------------*/
/* Function Name       : AT91F_InitSDRAM						*/
/* Object              : Initialize the SDRAM						*/
/*--------------------------------------------------------------------------------------*/
void AT91F_InitSDRAM()
{
	volatile int *pSDRAM = (int *)AT91C_BASE_SDRAM;
	
	/* Configure PIOC as peripheral (D16/D31) */	
	AT91F_PIO_CfgPeriph( AT91C_BASE_PIOC, 0xFFFF0000, 0);

	/*Setup MEMC to support all connected memories (CS0 = FLASH; CS1=SDRAM)*/
	AT91C_BASE_EBI->EBI_CSA = AT91C_EBI_CS1A;
	
	/*Init SDRAM*/
	AT91C_BASE_SDRC->SDRC_CR = 0x2188c159;  //was 0x2188c155; NR:NC NR=01b 12 row bits; NR=10b 13 row bits;
	AT91C_BASE_SDRC->SDRC_MR = 0x02;
	*pSDRAM = 0;
	AT91C_BASE_SDRC->SDRC_MR = 0x04;	
	*pSDRAM = 0; 
	*pSDRAM = 0; 
	*pSDRAM = 0; 
	*pSDRAM = 0; 
	*pSDRAM = 0; 
	*pSDRAM = 0; 
	*pSDRAM = 0; 
	*pSDRAM = 0; 

	AT91C_BASE_SDRC->SDRC_MR = 0x03;	
	*(pSDRAM + 0x80) = 0; 
	
	AT91C_BASE_SDRC->SDRC_TR= 0x2e0;
	*pSDRAM = 0; 
	
	AT91C_BASE_SDRC->SDRC_MR = 0;
	*pSDRAM = 0; 

}


/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_LowLevelInit							*/
/* \brief This function performs very low level HW initialization		*/
/*----------------------------------------------------------------------------	*/
void AT91F_LowLevelInit(void)
{
	/* Init Interrupt Controller */
	AT91F_AIC_Open(
		AT91C_BASE_AIC,          /* pointer to the AIC registers	*/
		AT91C_AIC_BRANCH_OPCODE, /* IRQ exception vector */
		AT91F_UndefHandler,      /* FIQ exception vector */
		AT91F_UndefHandler,      /* AIC default handler */
		AT91F_SpuriousHandler,   /* AIC spurious handler */
		0);                      /* Protect mode */

	/* Perform 8 End Of Interrupt Command to make sýre AIC will not Lock out nIRQ */
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);

	AT91F_AIC_SetExceptionVector((unsigned int *)0x0C, AT91F_FetchAbort);
	AT91F_AIC_SetExceptionVector((unsigned int *)0x10, AT91F_DataAbort);
	AT91F_AIC_SetExceptionVector((unsigned int *)0x4, AT91F_Undef);
	
	/* Initialize the SDRAM */
	AT91F_InitSDRAM();

	/* Open PIO for DBGU */
	AT91F_DBGU_CfgPIO();

	/* Configure DBGU */
	AT91F_US_Configure (
		(AT91PS_USART) AT91C_BASE_DBGU,  /* DBGU base address */
		AT91C_MASTER_CLOCK,              /* 48 MHz */
		AT91C_US_ASYNC_MODE,             /* mode Register to be programmed */
		AT91C_BAUD_RATE ,                /* baudrate to be programmed */
		0);                              /* timeguard to be programmed */

	/* Enable Transmitter */
	AT91F_US_EnableTx((AT91PS_USART)AT91C_BASE_DBGU);
	/* Enable Receiver */
	AT91F_US_EnableRx((AT91PS_USART)AT91C_BASE_DBGU);
	
	AT91F_DBGU_Printk("\n\r-I- AT91F_LowLevelInit(): Debug channel initialized\n\r");
}

