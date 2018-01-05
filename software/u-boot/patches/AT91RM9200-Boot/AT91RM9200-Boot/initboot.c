/*---------------------------------------------------------------------------- */
/*         ATMEL Microcontroller Software Support  -  ROUSSET  - 		*/
/*----------------------------------------------------------------------------	*/
/* The software is delivered "AS IS" without warranty or condition of any	*/
/* kind, either express, implied or statutory. This includes without		*/
/* limitation any warranty or condition with respect to merchantability or	*/
/* fitness for any particular purpose, or against the infringements of		*/
/* intellectual property rights of others.					*/
/*----------------------------------------------------------------------------	*/
/* File Name           : init.c							*/
/* Object              : Low level initialisations written in C			*/
/* Creation            : FB   23/10/2002					*/
/*										*/
/*----------------------------------------------------------------------------*/
#include "AT91RM9200.h"

/* //////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////// */

#define	FALSE			0
#define	TRUE			1
#define	DELAY_PLL		100
#define DELAY_MAIN_FREQ		100
#define INPUT_FREQ_MIN		900000
#define INPUT_FREQ_MAX		32000000
#define MASTER_CLOCK 		59904000
// #define MASTER_CLOCK 	45000000

#define QUARTZ_18432		1

#if QUARTZ_18432
#define	PLLAR       0x20263E04	/* 180 MHz for PLLA */
#define	PLLBR 	    0x10483E0E	/* 48,054857 MHz (divider by 2 for USB) */
#else /* 4.608MHZ */
#define	PLLAR 	    0x20263E01	/* 179,712000 MHz for PCK  for 4.608 */
#define	PLLBR 	    0x10673E05	/* 48,054857 MHz (divider by 2 for USB) */
#endif

#define	MCKR 	0x0000202	/* PLLA=180 , == CPU = 180MHZ, MASTER_CLOCK = 60MHZ */
//#define	MCKR  	0x0000302	/* PLLA=180 , == CPU = 180MHZ, MASTER_CLOCK = 45MHZ */

#define BASE_EBI_CS0_ADDRESS	0x10000000	/* base address to access memory on CS0 */
#define BASE_EBI_CS1_ADDRESS	0x20000000	/* base address to access memory on CS1 */

#define OUTPUT_FREQ_MIN		80000000
#define OUTPUT_FREQ_MAX		240000000

#define C1_IDC		(1<<2)	/* icache and/or dcache off/on */

/* //////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_WaitForMainClockFrequency					*/
/* \brief This function performs very low level HW initialization		*/
/*----------------------------------------------------------------------------*/
unsigned char AT91F_WaitForMainClockFrequency()
{
	volatile char	tmp	= 0;

/*----------------------------------------------------------------------------	*/
/* Step 2.									*/
/* Checking the Main Oscillator Frequency (Optional)				*/
/*----------------------------------------------------------------------------	*/
	
	/* Determine the main clock frequency */
	while(!(AT91C_BASE_CKGR->CKGR_MCFR & AT91C_CKGR_MAINRDY) && (tmp++ < DELAY_MAIN_FREQ));

	if (tmp >= DELAY_MAIN_FREQ)
		return FALSE;

	return TRUE;
}

/*----------------------------------------------------------------------------*/
/* \fn    AT91F_CheckPLL_FrequencyRange					      */
/* \brief This function performs very low level HW initialiszation	      */
/*----------------------------------------------------------------------------*/
unsigned char AT91F_CheckPLL_FrequencyRange(unsigned int MainClock,unsigned int pllDivider , unsigned int pllMultiplier)
{
	if(pllDivider == 0)
		return FALSE;

	/* Check Input Frequency */
	if( ((MainClock/pllDivider) < INPUT_FREQ_MIN)
	 || ((MainClock/pllDivider) > INPUT_FREQ_MAX) )
		return FALSE;

	/* Check Output Frequency */
	if( ((MainClock/pllDivider*pllMultiplier) < OUTPUT_FREQ_MIN)
	 || ((MainClock/pllDivider*pllMultiplier) > OUTPUT_FREQ_MAX) )
		return FALSE;

	return TRUE;
}

/*----------------------------------------------------------------------------*/
/* \fn    AT91F_InitClocks						      */
/* \brief This function performs very low level HW initialization	      */
/*----------------------------------------------------------------------------*/
unsigned char AT91F_InitClocks(int PLLAR_Register,int PLLBR_Register ,int MCKR_Register)
{
	volatile char 	tmp = 0;
	unsigned int	MainClock;
	unsigned int 	pllDivider,pllMultiplier;

/*----------------------------------------------------------------------------	*/
/* Optionnal									*/
/*----------------------------------------------------------------------------	*/

	/* Check if Input & Output Frequencies are in the correct range */

	/* Get Main Clock */
	MainClock 	= (((AT91C_BASE_CKGR->CKGR_MCFR) & AT91C_CKGR_MAINF) * 32768) >> 4;

	pllDivider    	= (PLLAR_Register  & AT91C_CKGR_DIVA);
	pllMultiplier 	= ((PLLAR_Register  & AT91C_CKGR_MULA) >> 16) + 1;
	if(AT91F_CheckPLL_FrequencyRange(MainClock, pllDivider , pllMultiplier) == FALSE)
		return FALSE;

	pllDivider    	= (PLLBR_Register  & AT91C_CKGR_DIVB);
	pllMultiplier 	= ((PLLBR_Register  & AT91C_CKGR_MULB) >> 16) + 1;
	if(AT91F_CheckPLL_FrequencyRange(MainClock, pllDivider , pllMultiplier) == FALSE)
		return FALSE;

/*----------------------------------------------------------------------------	*/
/* Step 3.									*/
/* Setting PLLA and Divider A							*/
/*----------------------------------------------------------------------------	*/

	AT91C_BASE_CKGR->CKGR_PLLAR = PLLAR_Register;
	/* Wait for PLLA stabilization LOCKA bit in PMC_SR */
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA) && (tmp++ < DELAY_PLL) ) ;

/*----------------------------------------------------------------------------	*/
/* Step 4.									*/
/* Setting PLLB and Divider B							*/
/*----------------------------------------------------------------------------	*/

	AT91C_BASE_CKGR->CKGR_PLLBR = PLLBR_Register;
	/* Wait for PLLB stabilization LOCKB bit in PMC_SR */
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKB) && (tmp++ < DELAY_PLL) ) ;

/*----------------------------------------------------------------------------	*/
/* Step 5.									*/
/* Selection of Master Clock MCK (and Processor Clock PCK)			*/
/*----------------------------------------------------------------------------	*/

	/* Constraints of the Master Clock selection sequence */
	/* Write in the MCKR dirty value concerning the clock selection CSS then overwrite it in a second sequence */
	AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK;
	/* Wait until the master clock is established */
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (tmp++ < DELAY_MAIN_FREQ) );

	/* Second sequence */
	AT91C_BASE_PMC->PMC_MCKR = MCKR_Register;
	/* Wait until the master clock is established */
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (tmp++ < DELAY_MAIN_FREQ) );

	return TRUE;
}


void AT91F_InitSDRAM()
{
	volatile int *pRegister;
	AT91PS_PIO pPio = AT91C_BASE_PIOC;
	
	/* Configure PIOC as peripheral (D16/D31) */
	pPio->PIO_ASR = 0xFFFF0000;
	pPio->PIO_BSR = 0x0;
	pPio->PIO_PDR = 0xFFFF0000;
	
	/* Setup MEMC to support all connected memories (CS0 = FLASH; CS1=SDRAM) */
	pRegister = (int *)0xFFFFFF60;
	*pRegister = 0x02; 
	
	/* Init SDRAM */
	pRegister = (int *)0xFFFFFF98;
	*pRegister = 0x2188c159;  //was 0x2188c155; NR:NC NR=01b 12 row bits; NR=10b 13 row bits;
	pRegister = (int *)0xFFFFFF90;
	*pRegister = 0x2; 
	pRegister = (int *)0x20000000;
	*pRegister = 0; 
	pRegister = (int *)0xFFFFFF90;
	*pRegister = 0x4; 
	pRegister = (int *)0x20000000;
	*pRegister = 0; 
	*pRegister = 0; 
	*pRegister = 0; 
	*pRegister = 0; 
	*pRegister = 0; 
	*pRegister = 0; 
	*pRegister = 0; 
	*pRegister = 0; 
	pRegister = (int *)0xFFFFFF90;
	*pRegister = 0x3; 
	pRegister = (int *)0x20000080;
	*pRegister = 0; 
	pRegister = (int *)0xFFFFFF94;
	*pRegister = 0x2e0; 
	pRegister = (int *)0x20000000;
	*pRegister = 0; 
	pRegister = (int *)0xFFFFFF90;
	*pRegister = 0x00; 
	pRegister = (int *)0x20000000;
	*pRegister = 0; 
}

/*----------------------------------------------------------------------------*/
/* \fn    AT91F_Read_p15_c1							*/
/* \brief This function reads co-processor 15, register 1 (control register)	*/
/*----------------------------------------------------------------------------*/
unsigned int AT91F_Read_p15_c1(void)
{
    unsigned int value;

    __asm__ __volatile__(
	"mrc     p15, 0, %0, c1, c0, 0   @ read control reg\n"
	: "=r" (value)
	:
	: "memory");
 
    return value;
}

/*----------------------------------------------------------------------------*/
/* \fn    AT91F_Write_p15_c1							*/
/* \brief This function writes to co-processor 15, register 1 (control register) */
/*----------------------------------------------------------------------------*/
void AT91F_Write_p15_c1(unsigned int value)
{
    __asm__ __volatile__(
        "mcr     p15, 0, %0, c1, c0, 0   @ write it back\n"
	: "=r" (value)
	:
	: "memory");

    AT91F_Read_p15_c1();
}

/*----------------------------------------------------------------------------*/
/* \fn    AT91F_Enable_ICache							*/
/* \brief This function enables Instruction Cache				*/
/*----------------------------------------------------------------------------*/
void AT91F_Enable_ICache(void)
{
    unsigned int i, reg;
    
    reg = AT91F_Read_p15_c1();
    for (i=0; i<100; i++);
    AT91F_Write_p15_c1(reg | C1_IDC);
}


/*----------------------------------------------------------------------------*/
/* \fn    AT91F_InitFlash							*/
/* \brief This function performs very low level HW initialization		*/
/*----------------------------------------------------------------------------*/
void AT91F_InitFlash()
{
#if 0
	AT91C_BASE_MC->MC_PUIA[0] = AT91C_MC_PROT_PNAUNA;
	AT91C_BASE_MC->MC_PUP = 0;
	AT91C_BASE_MC->MC_PUER = 0;	/* Memory controller protection unit disable */
	AT91C_BASE_MC->MC_ASR = 0;  /* read only! */
	AT91C_BASE_MC->MC_AASR = 0; /* read only! */

	/* Setup MEMC to support CS0=Flash */
	AT91C_BASE_EBI->EBI_CSA |= AT91C_EBI_CS0A_SMC;
	AT91C_BASE_EBI->EBI_CFGR = (AT91C_EBI_DBPUC & 0x00) | (AT91C_EBI_EBSEN & 0x00);
#endif	

	/* Setup Flash */
	AT91C_BASE_SMC2->SMC2_CSR[0] = (AT91C_SMC2_NWS & 0x4) | AT91C_SMC2_WSEN
									| (AT91C_SMC2_TDF & 0x200) | AT91C_SMC2_BAT | AT91C_SMC2_DBW_16;
}

/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_DBGU_Printk							*/
/* \brief This function is used to send a string through the DBGU channel (Very low level debugging) */
/*----------------------------------------------------------------------------*/
void AT91F_DBGU_Printk(
	char *buffer) /* \arg pointer to a string ending by \0 */
{
	while(*buffer != '\0') {
		while (!(((AT91PS_USART)AT91C_BASE_DBGU)->US_CSR & AT91C_US_TXRDY));
		((AT91PS_USART)AT91C_BASE_DBGU)->US_THR = (*buffer++ & 0x1FF);
	}
}

/*----------------------------------------------------------------------------	*/
/* \fn    AT91F_LowLevelInit							*/
/* \brief This function performs very low level HW initialization		*/
/*----------------------------------------------------------------------------*/
void AT91F_LowLevelInit()
{
	AT91PS_USART 	pUSART = (AT91PS_USART)AT91C_BASE_DBGU;
	AT91PS_PDC	pPdc;
	unsigned char 	status;

/*----------------------------------------------------------------------------	*/
/* Step 2.									*/
/* Checking the Main Oscillator Frequency (Optional)				*/
/*----------------------------------------------------------------------------	*/

	status = AT91F_WaitForMainClockFrequency();

/*----------------------------------------------------------------------------	*/
/* Step 3. to 5.								*/
/*----------------------------------------------------------------------------	*/

	AT91F_InitFlash();

	status = AT91F_InitClocks(PLLAR,PLLBR, MCKR);

/*----------------------------------------------------------------------------	*/
/* SDRAM & Flash AT49BV1614 Initialisations					*/
/*----------------------------------------------------------------------------	*/

	AT91F_InitSDRAM();
		
/*----------------------------------------------------------------------------	*/
/* Test PMC Initialization.							*/
/*----------------------------------------------------------------------------	*/

	/* Open PIO for DBGU */
	AT91C_BASE_PIOA->PIO_ASR = AT91C_PA31_DTXD | AT91C_PA30_DRXD;
	AT91C_BASE_PIOA->PIO_BSR = 0x0;
	AT91C_BASE_PIOA->PIO_PDR = AT91C_PA31_DTXD | AT91C_PA30_DRXD;

	/******************/
	/* Configure DBGU */
	/******************/
	
	/* Disable interrupts */
	pUSART->US_IDR = (unsigned int) -1;
	
	/* Reset receiver and transmitter */
	pUSART->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS | AT91C_US_TXDIS;
	
	/* Define the baudrate divisor register */
	pUSART->US_BRGR = MASTER_CLOCK/(115200*16); /* 33 For 60 MHz and 24 for 45 MHz */
	
	/* Write the timeguard register */
	pUSART->US_TTGR = 0;
	
	/* Clear Transmit and Receive counters */
	pPdc = (AT91PS_PDC) &pUSART->US_RPR;
	
	pPdc->PDC_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;
	pPdc->PDC_TNPR = 0;
	pPdc->PDC_TNCR = 0;
	pPdc->PDC_RNPR = 0;
	pPdc->PDC_RNCR = 0;
	pPdc->PDC_TPR = 0;
	pPdc->PDC_TCR = 0;
	pPdc->PDC_TPR = 0;
	pPdc->PDC_TCR = 0;
	pPdc->PDC_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;	/* Enable Rx and Tx */
	
	/* Define the USART mode */ 
	pUSART->US_MR = AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE;
	
	/* Enable Transmitter */
	pUSART->US_CR = AT91C_US_TXEN;

	/* Enable I-Cache */
	AT91F_Enable_ICache();

#ifdef DEBUG
	AT91F_DBGU_Printk("\n\rLow Level Init performed\n\r");
#endif

}
