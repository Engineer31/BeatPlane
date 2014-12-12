;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; Copyright (c) 2004-2007 threewater@up-tech.com, All rights reserved.
;;; 
;;; Startup Code for
;;;        S3C2410 : exception.s
;;;; by threewater	2005.2.23
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Start here
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	PROGRAM ?INTVEC

        SECTION .intvec:CODE(2)
        PUBLIC  __vector
        PUBLIC  __vector_0x14
        PUBLIC  __iar_program_start
	IMPORT  ColdReset
	IMPORT	Enter_UNDEF
	IMPORT	Enter_SWI
	IMPORT	Enter_PABORT
	IMPORT	Enter_DABORT
	IMPORT	IRQ_Handler
	IMPORT	Enter_FIQ
        ARM

__vector:
__iar_program_start:
	ldr	pc, =ColdReset		;reset
	ldr	pc, =Enter_UNDEF	;UndefinedInstruction
	ldr	pc, =Enter_SWI		;syscall_handler or SWI
	ldr	pc, =Enter_PABORT	;PrefetchAbort
	ldr	pc, =Enter_DABORT	;DataAbort
__vector_0x14:
	b	.			;ReservedHandler
	ldr	pc, =IRQ_Handler	;IRQHandler
	ldr	pc, =Enter_FIQ		;FIQHandler

	LTORG	;for save exception address

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; End of exception.s
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        END
