/***************************************************************************\
	Copyright (c) 2004-2007 threewater@up-tech.com, All rights reserved.
	by threewter	2004.5.12
\***************************************************************************/
/***************************************************************************\
    #说明: C  main 函数，ucos-ii初始化等定义
	----------------------------------  Bug  --------------------------------------

	----------------------------------  TODO list  --------------------------------------

	----------------------------------修正--------------------------------------

	2004-5-12	移植

\***************************************************************************/

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "../ucos-ii/includes.h"
#include "../inc/sys/lib.h"
#include "../inc/macro.h"
#include "../inc/drv/reg2410.h"
#include "uhal.h"
#include "isr.h"

#ifdef UPTECH_ISR_DEBUG
#if 0
#define _TICK		900000
//void irq_Timer4(int vector, int ndev);
//Initialize timer that is used OS.
void InitTimer4(void)//use timer4
{
    //the following code is for s3c2410, use timer4 for tick
    //rTCFG0=0x0f0f;	//timer 0,1,2,3,4 16 prescaler
    rTCFG0=0x0f|100<<8;	
    rTCFG1=0x11111|(0x3<<16);
    //rTCFG1=(0x3)|(0x3<<4)|(0x3<<8)|(0x3<<12)|(0x3<<16);
    //rTCFG1=0x11111;	//timer 0,1,2,3,4 1/4 divider
    //rTCNTB4= PCLK/16/4/_TICK;
    rTCNTB4=0xffff;
    rTCON=TCON_4_UPDATE;	//update mode for TCNTB4 and TCMPB4.
    rTCON=TCON_4_AUTO;		//timer4 = auto reload, start

    //rCLKCON |= CLKCON_PWM;
}
#else
#define _TICK		OS_TICKS_PER_SEC

//Initialize timer that is used OS.
void InitTimer4(void)//use timer1
{
    //the following code is for s3c2410, use timer4 for tick
    rTCFG0=0x0f0f;	//timer 0,1,2,3,4 16 prescaler
    rTCFG1=0x11111;	//timer 0,1,2,3,4 1/4 divider
    rTCNTB4= PCLK/16/4/_TICK;
    rTCON=TCON_4_UPDATE;	//update mode for TCNTB4 and TCMPB4.
    rTCON=TCON_4_AUTO;		//timer4 = auto reload, start

    rCLKCON |= CLKCON_PWM;
}//uHALr_InitTimers


#endif
extern void irq_Timer4(int vector, int ndev);

void start_Timerinterrupt(void)
{
    SetISR_Interrupt(IRQ_TIMER4, (Interrupt_func_t)irq_Timer4, NULL);
    rTCON |=TCON_4_ONOFF;	//start timer4;
}
#else

#define _TICK		OS_TICKS_PER_SEC

//Initialize timer that is used OS.
void uHALr_InitTimers(void)//use timer1
{
	
  //the following code is for s3c2410, use timer4 for tick
    rTCFG0=0x0f0f;	//timer 0,1,2,3,4 16 prescaler
    rTCFG1=0x11111;	//timer 0,1,2,3,4 1/4 divider
    rTCNTB4= PCLK/16/4/_TICK;
    rTCON=TCON_4_UPDATE;	//update mode for TCNTB4 and TCMPB4.
    rTCON=TCON_4_AUTO;		//timer4 = auto reload, start

    rCLKCON |= CLKCON_PWM;
}//uHALr_InitTimers


//static int itimes = 0;
void TimerTickHandle(int vector, void* data)
{
 //      BSPprintf(0,"%s->%s: %d\n", __FILE__,__FUNCTION__, __LINE__);
 //     BSPprintf(0, "TimerTickHander %d times\n", ++itimes);
#if 0 
      BSPprintf(0, "%3d ", ++itimes);
      if(!(itimes %20) && itimes)  BSPprintf(0, "\n");
#endif	
      OSTimeTick();
#if 0      
      if (itimes >=300 )
      { 
        BSPprintf(0,"\ndebug itimes=%d\n",itimes);
      }
#endif      
}

//Start system timer & enable the interrupt.
void uHALr_InstallSystemTimer(void)
{		
//	rTCON |=TCON_4_ONOFF;	//start timer4;
    SetISR_Interrupt(IRQ_TIMER4, (Interrupt_func_t)TimerTickHandle, NULL);
    rTCON |=TCON_4_ONOFF;	//start timer4;

}//uHALr_InstallSystemTimer

#endif


void uHALr_InterruptRequestInit()
{
#if 0
	pISR_UNDEF= (unsigned) DebugUNDEF;
	pISR_SWI= (unsigned) DebugSWI;
	pISR_PABORT= (unsigned) DebugABORT;
	pISR_DABORT= (unsigned) DebugABORT;
//	pISR_RESERVED
	pISR_IRQ= (unsigned) IRQ_Handler;	//irq interrupt
	pISR_FIQ= (unsigned) DebugFIQ;

///////////////////////No use////////////////////////
	pISR_ADC= (unsigned) BreakPoint;
	pISR_RTC= (unsigned) BreakPoint;
	pISR_UTXD1= (unsigned) BreakPoint;
	pISR_UTXD0= (unsigned) BreakPoint;
	pISR_SIO= (unsigned) BreakPoint;
	pISR_IIC= (unsigned) BreakPoint;
	pISR_URXD1= (unsigned) BreakPoint;
	pISR_URXD0= (unsigned) BreakPoint;
	//remove by eric rong//	pISR_EINT67= (unsigned) BreakPoint;
	pISR_WDT= (unsigned) BreakPoint;
	pISR_TIMER3= (unsigned) BreakPoint;
	pISR_TIMER2= (unsigned) BreakPoint;
	pISR_TIMER1= (unsigned) BreakPoint;
	//	pISR_TIMER0= (unsigned) BreakPoint;
	//remove by eric rong//	pISR_UERR1= (unsigned) BreakPoint;
	//remove by eric rong//	pISR_UERR0= (unsigned) BreakPoint;
	pISR_BDMA1= (unsigned) BreakPoint;
	pISR_BDMA0= (unsigned) BreakPoint;
	pISR_ZDMA1= (unsigned) BreakPoint;
	pISR_ZDMA0= (unsigned) BreakPoint;
	//remove by eric rong//	pISR_EINT5= (unsigned) BreakPoint;
	//remove by eric rong//	pISR_EINT4= (unsigned) BreakPoint;
	pISR_EINT3= (unsigned) BreakPoint;
	pISR_EINT2= (unsigned) BreakPoint;
	pISR_EINT1= (unsigned) BreakPoint;
	pISR_EINT0= (unsigned) BreakPoint;
/////////////////////////////////////////////////////
#endif
}

void BreakPoint(void)
{
	BSPprintf(0,"!!!Enter break point.\n");

	for(;;);
}//BreakPoint

void OutDebug(unsigned int num)
{
	BSPprintf(0,"\n***STACK***:%d\r\n", num);
}//OutDebug

//Define pre & post-process routines for Interrupt.
void uHALir_DefineIRQ(void *is, void *iq, void *n)
{
}//uHALir_DefineIRQ

static int I_COUNT=0;

void Enter_UNDEF(void)
{
	BSPprintf(0,"!!!Enter UNDEFINED. %d\r\n", I_COUNT++);

	for(;;);
}//BreakPoint


void Enter_SWI(void)
{
	BSPprintf(0,"!!!Enter SWI. %d\r\n", I_COUNT++);	

	for(;;);
}

void Enter_PABORT(void)
{
	BSPprintf(0,"!!!Enter Prefetch ABORT %d\r\n", I_COUNT++);

	for(;;);
}

void Enter_DABORT(void)
{
	BSPprintf(0,"!!!Enter Data ABORT %d\r\n", I_COUNT++);

	for(;;);
}

void Enter_FIQ(void)
{
	BSPprintf(0,"!!!Enter FIQ. %d\r\n", I_COUNT++);

	for(;;);
}

unsigned int PCLK=0, HCLK=0, FCLK=0;

void ARMTargetInit(void)
{
	//MMU_Init();
	s3c2410_cpu_init();

	Uart_Init(0,115200);
	Uart_Init(1,115200);

	printk("\nBegin to run uC/OS-II!\n");

	hudelay(0);	//计算延时循环
		//init_SIO();
#if (USE_YAFFS==0)
	NandFlash_init();
#endif
	uHALr_InitInterrupts();

#ifndef UPTECH_ISR_DEBUG
	uHALr_InitTimers();
#endif
	uHALr_InterruptRequestInit();
}


