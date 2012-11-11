/**
 *	Integrated Interrupt Controller for RaspberryPi.
 *	@author	James Walmsley <james@fullfat-fs.co.uk>
 **/

#include "interrupts.h"
#include "bcm2835_intc.h"

static INTERRUPT_VECTOR g_VectorTable[BCM2835_INTC_TOTAL_IRQ];


typedef struct {
	unsigned long	IRQBasic;	// Pending 0
	unsigned long	Pending1;
	unsigned long	Pending2;
	unsigned long	FIQCtrl;
	unsigned long	Enable1;
	unsigned long	Enable2;
	unsigned long	EnableBasic;
	unsigned long	Disable1;
	unsigned long	Disable2;
	unsigned long	DisableBasic;
} BCM2835_INTC_REGS;

static volatile BCM2835_INTC_REGS * const pRegs = (BCM2835_INTC_REGS *) (BCM2835_BASE_INTC);

/**
 *	Enables all IRQ's in the CPU's CPSR register.
 **/
static void irqEnable() {
	__asm volatile("mrs 	r0,cpsr");		// Read in the cpsr register.
	__asm volatile("bic		r0,r0,#0x80");	// Clear bit 8, (0x80) -- Causes IRQs to be enabled.
	__asm volatile("msr		cpsr_c, r0");	// Write it back to the CPSR register
}

static void irqDisable() {
	__asm volatile("mrs		r0,cpsr");		// Read in the cpsr register.
	__asm volatile("orr		r0,r0,#0x80");	// Set bit 8, (0x80) -- Causes IRQs to be disabled.
	__asm volatile("msr		cpsr_c, r0");	// Write it back to the CPSR register.

}

#define clz(a) \
 ({ unsigned long __value, __arg = (a); \
     asm ("clz\t%0, %1": "=r" (__value): "r" (__arg)); \
     __value; })


/**
 *	This is the global IRQ handler  on this platform!
 *	It is based on the assembler code found in the Broadcom datasheet.
 *
 **/
void irqHandler() {	

	/**
	 *	WARNING:
	 *		Compiler optimisations will likely prevent this code from working!
	 *		
	 *	This will be fixed later!
	 **/

	register unsigned long ulMaskedStatus;
	register unsigned long irqNumber;
	register unsigned long tmp;

	ulMaskedStatus = pRegs->IRQBasic;
	tmp = ulMaskedStatus & 0x00000300;			// Check if anything pending in pr1/pr2.   

	if(ulMaskedStatus & ~0xFFFFF300) {			// Note how we mask out the GPU interrupt Aliases.
		irqNumber = 64 + 31;						// Shifting the basic ARM IRQs to be IRQ# 64 +
		goto emit_interrupt;
	}

	if(tmp & 0x100) {
		ulMaskedStatus = pRegs->Pending1;
		irqNumber = 0 + 31;
		// Clear the interrupts also available in basic IRQ pending reg.
		//ulMaskedStatus &= ~((1 << 7) | (1 << 9) | (1 << 10) | (1 << 18) | (1 << 19));
		if(ulMaskedStatus) {
			goto emit_interrupt;
		}
	}

	if(tmp & 0x200) {
		ulMaskedStatus + pRegs->Pending2;		
		irqNumber = 32 + 31;
		// Don't clear the interrupts in the basic pending, simply allow them to processed here!
		if(ulMaskedStatus) {
			goto emit_interrupt;
		}				
	}

	return;

emit_interrupt:

	tmp = ulMaskedStatus - 1;
	ulMaskedStatus = ulMaskedStatus ^ tmp;

	unsigned long lz = clz(ulMaskedStatus);

	//irqNumber = irqNumber - 

	//__asm volatile("clz	r7,r5");				// r5 is the ulMaskedStatus register. Leaving result in r6!
	//__asm volatile("sub r6,r7");

	if(g_VectorTable[irqNumber-lz].pfnHandler) {
		g_VectorTable[irqNumber-lz].pfnHandler(irqNumber, g_VectorTable[irqNumber].pParam);
	}
}


static void stubHandler(int nIRQ, void *pParam) {
	/**
	 *	Actually if we get here, we should probably disable the IRQ,
	 *	otherwise we could lock up this system, as there is nothing to 
	 *	ackknowledge the interrupt.
	 **/   
}

int InitInterruptController() {
	int i;
	for(i = 0; i < BCM2835_INTC_TOTAL_IRQ; i++) {
		g_VectorTable[i].pfnHandler 	= stubHandler;
		g_VectorTable[i].pParam			= (void *) 0;
	}
	return 0;
}



int RegisterInterrupt(int nIRQ, FN_INTERRUPT_HANDLER pfnHandler, void *pParam) {

	irqDisable();
	{
		g_VectorTable[nIRQ].pfnHandler = pfnHandler;
		g_VectorTable[nIRQ].pParam		= pParam;
	}
	irqEnable();
	return 0;
}

int EnableInterrupt(int nIRQ) {

	unsigned long	ulTMP;

	ulTMP = pRegs->EnableBasic;

	if(nIRQ >= 64 && nIRQ <= 72) {	// Basic IRQ enables
		pRegs->EnableBasic = 1 << (nIRQ - 64);
	}

	ulTMP = pRegs->EnableBasic;

	// Otherwise its a GPU interrupt, and we're not supporting those...yet!

	return 0;
}

int DisableInterrupt(int nIRQ) {
	if(nIRQ >= 64 && nIRQ <= 72) {
		pRegs->DisableBasic = 1 << (nIRQ - 64);
	}

	// I'm currently only supporting the basic IRQs.

	return 0;
}

int EnableInterrupts() {
	irqEnable();
	return 0;
}

int DisableInterrupts() {
	irqDisable();
	return 0;
}
