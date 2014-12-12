;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; Copyright (c) 2004-2007 threewater@up-tech.com, All	rights reserved.
;;; 
;;; Startup Code for
;;;	   S3C2410 : Startup.s
;;;; by	threewater	2005.2.22
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "2410addr.s"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Some ARM920	CPSR bit discriptions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Pre-defined constants
USERMODE    EQU	0x10
FIQMODE	    EQU	0x11
IRQMODE	    EQU	0x12
SVCMODE	    EQU	0x13
ABORTMODE   EQU	0x17
UNDEFMODE   EQU	0x1b
MODEMASK    EQU	0x1f
NOINT	    EQU	0xc0

I_Bit       DEFINE  0x80
F_Bit       DEFINE  0x40

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; MMU	Register discription
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;p15	CP	15
;c0	CN	0
;c1	CN	1
;c2	CN	2
;c3	CN	3

CtrlMMU         DEFINE  1
CtrlAlign       DEFINE  2
CtrlCache       DEFINE  4
CtrlWBuff       DEFINE  8
CtrlBigEnd      DEFINE  128
CtrlSystem      DEFINE  256
CtrlROM         DEFINE  512

;initialization	L0 is MMU FULL_ACCESS, DOMAIN, SECTION
TLB_L0_INIT     DEFINE  0x0C02
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Start here
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        MODULE  ?CSTARTUP

        SECTION UND_STACK:DATA:NOROOT(3)
        SECTION ABT_STACK:DATA:NOROOT(3)
        SECTION IRQ_STACK:DATA:NOROOT(3)
        SECTION FIQ_STACK:DATA:NOROOT(3)
        SECTION SVC_STACK:DATA:NOROOT(3)
        SECTION .text:CODE:NOROOT(2)
        IMPORT  ?main
        ARM

;deal with IRQ interrupt
	PUBLIC  IRQ_Handler
IRQ_Handler
	IMPORT	ISR_IrqHandler
	STMFD	sp!, {r0-r12, lr}
	BL	ISR_IrqHandler
	LDMFD	sp!, {r0-r12, lr}
	SUBS	pc, lr,	#4
        LTORG

;=======
; ENTRY	 
;=======
        PUBLIC  ColdReset
ColdReset
	ldr	r0,=WTCON	;watch dog disable 
	ldr	r1,=0x0		
	str	r1,[r0]

	ldr	r0,=INTMSK
	ldr	r1,=0xffffffff	;all interrupt disable
	str	r1,[r0]

	ldr	r0,=INTSUBMSK
	ldr	r1,=0x7ff	;all sub interrupt disable, 2002/04/10
	str	r1,[r0]

;****************************************************
;*	Initialize stacks			    * 
;****************************************************
	bl	InitStacks	; Stack	Setup for each MODE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; start main function in C language
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	b       ?main
        b       .
        LTORG

;****************************************************
;*	The function for initializing stack	    *
;****************************************************
InitStacks
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#UNDEFMODE|NOINT
	msr	cpsr_cxsf,r1		;UndefMode
	ldr	sp,=SFE(UND_STACK)

	orr	r1,r0,#ABORTMODE|NOINT
	msr	cpsr_cxsf,r1		;AbortMode
	ldr	sp,=SFE(ABT_STACK)

	orr	r1,r0,#IRQMODE|NOINT
	msr	cpsr_cxsf,r1		;IRQMode
	ldr	sp,=SFE(IRQ_STACK)

	orr	r1,r0,#FIQMODE|NOINT
	msr	cpsr_cxsf,r1		;FIQMode
	ldr	sp,=SFE(FIQ_STACK)

	orr	r1,r0,#SVCMODE|NOINT
	msr	cpsr_cxsf,r1		;SVCMode
	ldr	sp,=SFE(SVC_STACK)

	;USER mode is not initialized.
	mov	pc,lr
        LTORG

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; End of Startup.s
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	END
