#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.  
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR			char
#define portFLOAT			float
#define portDOUBLE		double
#define portLONG			long
#define portSHORT			short
#define portSTACK_TYPE	unsigned portLONG
#define portBASE_TYPE	portLONG

#if( configUSE_16_BIT_TICKS == 1 )
	typedef unsigned portSHORT portTickType;
	#define portMAX_DELAY ( portTickType ) 0xffff
#else
	typedef unsigned portLONG portTickType;
	#define portMAX_DELAY ( portTickType ) 0xffffffff
#endif
/*-----------------------------------------------------------*/	

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_RATE_MS			( ( portTickType ) 1000 / configTICK_RATE_HZ )		
#define portBYTE_ALIGNMENT			8
#define portNOP()						__asm volatile ( "NOP" );
/*-----------------------------------------------------------*/	


/* Scheduler utilities. */

/*
 * portSAVE_CONTEXT, portRESTORE_CONTEXT, portENTER_SWITCHING_ISR
 * and portEXIT_SWITCHING_ISR can only be called from ARM mode, but
 * are included here for efficiency.  An attempt to call one from
 * THUMB mode code will result in a compile time error.
 */

#define portRESTORE_CONTEXT()															\
{																						\
	extern volatile void * volatile pxCurrentTCB;										\
	extern volatile unsigned portLONG ulCriticalNesting;								\
																						\
	/* Set the LR to the task stack.												*/	\
	__asm volatile (																	\
	/* Put the address of the current TCB into R1.									*/	\
	"LDR		R0, =pxCurrentTCB											\n\t"		\
	/* Load the 32-bit value stored at the address in R1.							*/	\
	/* First item in the TCB is the top of the stack for the current TCB.			*/	\
	"LDR		R0, [R0]													\n\t"		\
																		\
	/* Move the value into the Link Register!										*/	\
	"LDR		LR, [R0]														\n\t"	\
																							\
	/* The critical nesting depth is the first item on the stack. */	\
	/* Load it into the ulCriticalNesting variable. */						\
	"LDR		R0, =ulCriticalNesting										\n\t"	\
	"LDMFD		LR!, {R1}													\n\t"	\
	"STR		R1, [R0]													\n\t"	\
																							\
	/* Get the SPSR from the stack. */											\
	"LDMFD		LR!, {R0}														\n\t"	\
	"MSR		SPSR_cxsf, R0															\n\t"	\
																							\
	/* Restore all system mode registers for the task. */					\
	"LDMFD	LR, {R0-R14}^													\n\t"	\
	"NOP																			\n\t"	\
																							\
	/* Restore the return address. */											\
	"LDR		LR, [LR, #+60]													\n\t" \
																							\
	/* And return - correcting the offset in the LR to obtain the */	\
	/* correct address. */															\
	"SUBS		PC, LR, #4														\n\t"  \
	"NOP														\n\t"	\
	"NOP														\n\t"	\
	);																						\
	( void ) ulCriticalNesting;													\
	( void ) pxCurrentTCB;															\
}
/*-----------------------------------------------------------*/

#define portSAVE_CONTEXT()													\
{																			\
	extern volatile void * volatile pxCurrentTCB;							\
	extern volatile unsigned portLONG ulCriticalNesting;					\
																			\
	/* Push R0 as we are going to use the register. */						\
	__asm volatile (														\
	"STMDB	SP!, {R0}												\n\t"	\
																			\
	/* Set R0 to point to the task stack pointer. */						\
	"STMDB	SP,{SP}^	\n\t"	/* ^ means get the user mode SP value. */	\
	/*"NOP															\n\t" */ \
	"SUB	SP, SP, #4												\n\t"	\
	"LDMIA	SP!,{R0}												\n\t"	\
																			\
	/* Push the return address onto the stack. */							\
	"STMDB	R0!, {LR}												\n\t"	\
																			\
	/* Now we have saved LR we can use it instead of R0. */					\
	"MOV	LR, R0													\n\t"	\
																			\
	/* Pop R0 so we can save it onto the system mode stack. */				\
	"LDMIA	SP!, {R0}												\n\t"	\
																			\
	/* Push all the system mode registers onto the task stack. */			\
	"STMDB	LR,{R0-LR}^												\n\t"	\
	"NOP															\n\t"	\
	"SUB	LR, LR, #60												\n\t"	\
																			\
	/* Push the SPSR onto the task stack. */								\
	"MRS	R0, SPSR												\n\t"	\
	"STMDB	LR!, {R0}												\n\t"	\
																			\
	"LDR	R0, =ulCriticalNesting									\n\t"	\
	"LDR	R0, [R0]												\n\t"	\
	"STMDB	LR!, {R0}												\n\t"	\
																			\
	/* Store the new top of stack for the task. */							\
	"LDR	R0, =pxCurrentTCB										\n\t"	\
	"LDR	R0, [R0]												\n\t"	\
	"STR	LR, [R0]												\n\t"	\
	);																		\
	( void ) ulCriticalNesting;												\
	( void ) pxCurrentTCB;													\
}

extern void vTaskSwitchContext( void );
#define portYIELD_FROM_ISR()		vTaskSwitchContext()
#define portYIELD()					__asm volatile ( "SWI 0" )
/*-----------------------------------------------------------*/


/* Critical section management. */

/*
 * The interrupt management utilities can only be called from ARM mode.	 When
 * THUMB_INTERWORK is defined the utilities are defined as functions in 
 * portISR.c to ensure a switch to ARM mode.  When THUMB_INTERWORK is not 
 * defined then the utilities are defined as macros here - as per other ports.
 */

#ifdef THUMB_INTERWORK

	extern void vPortDisableInterruptsFromThumb( void ) __attribute__ ((naked));
	extern void vPortEnableInterruptsFromThumb( void ) __attribute__ ((naked));

	#define portDISABLE_INTERRUPTS()	vPortDisableInterruptsFromThumb()
	#define portENABLE_INTERRUPTS()	vPortEnableInterruptsFromThumb()
	
#else

	#define portDISABLE_INTERRUPTS()														\
		__asm volatile (																		\
			"STMDB	SP!, {R0}		\n\t"		/* Push R0.							*/	\
			"MRS		R0, CPSR			\n\t"		/* Get CPSR.						*/	\
			"ORR		R0, R0, #0xC0	\n\t"		/* Disable IRQ, FIQ.				*/	\
			"MSR		CPSR, R0			\n\t"		/* Write back modified value.	*/	\
			"LDMIA	SP!, {R0}			 " )	/* Pop R0.							*/
			
	#define portENABLE_INTERRUPTS()														\
		__asm volatile (																		\
			"STMDB	SP!, {R0}		\n\t"		/* Push R0.							*/	\
			"MRS		R0, CPSR			\n\t"		/* Get CPSR.						*/	\
			"BIC		R0, R0, #0xC0	\n\t"		/* Enable IRQ, FIQ.				*/	\
			"MSR		CPSR, R0			\n\t"		/* Write back modified value.	*/	\
			"LDMIA	SP!, {R0}			 " )	/* Pop R0.							*/

#endif /* THUMB_INTERWORK */

extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portENTER_CRITICAL()		vPortEnterCritical();
#define portEXIT_CRITICAL()		vPortExitCritical();
/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
