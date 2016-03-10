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

// Remember which interrupts have been enabled:
static unsigned long enabled[3];

/**
 *	Enables all IRQ's in the CPU's CPSR register.
 **/
static void irqEnable() {
	__asm volatile("cpsie i" : : : "memory");
}

static void irqDisable() {
	__asm volatile("cpsid i" : : : "memory");
}

static void handleRange (unsigned long pending, const unsigned int base)
{
	while (pending)
	{
		// Get index of first set bit:
		unsigned int bit = 31 - __builtin_clz(pending);

		// Map to IRQ number:
		unsigned int irq = base + bit;

		// Call interrupt handler:
		g_VectorTable[irq].pfnHandler(irq, g_VectorTable[irq].pParam);

		// Clear bit in bitfield:
		pending &= ~(1UL << bit);
	}
}

/**
 *	This is the global IRQ handler on this platform!
 *	It is based on the assembler code found in the Broadcom datasheet.
 *
 **/
void irqHandler (void)
{
	register unsigned long ulMaskedStatus = pRegs->IRQBasic;

	// Bit 8 in IRQBasic indicates interrupts in Pending1 (interrupts 31-0):
	if (ulMaskedStatus & (1UL << 8))
		handleRange(pRegs->Pending1 & enabled[0], 0);

	// Bit 9 in IRQBasic indicates interrupts in Pending2 (interrupts 63-32):
	if (ulMaskedStatus & (1UL << 9))
		handleRange(pRegs->Pending2 & enabled[1], 32);

	// Bits 7 through 0 in IRQBasic represent interrupts 64-71:
	if (ulMaskedStatus & 0xFF)
		handleRange(ulMaskedStatus & 0xFF & enabled[2], 64);
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
	if(nIRQ<0 || nIRQ>71)
		return -1;

	irqDisable();
	{
		g_VectorTable[nIRQ].pfnHandler = pfnHandler;
		g_VectorTable[nIRQ].pParam		= pParam;
	}
	irqEnable();
	return 0;
}

int EnableInterrupt(int nIRQ) {
	/* Datasheet says "All other bits are unaffected", and I'm counting on that. */
	unsigned int mask=1<<(nIRQ%32);

	if(nIRQ >=0 && nIRQ <=31) {
		pRegs->Enable1 = mask;
		enabled[0] |= mask;
	} else
	if(nIRQ >=32 && nIRQ <=63){
		pRegs->Enable2 = mask;
		enabled[1] |= mask;
	} else
	if(nIRQ >= 64 && nIRQ <= 71) {	// Basic IRQ enables
		pRegs->EnableBasic = mask;
		enabled[2] |= mask;
	} else
		return -1;

	return 0;
}

int DisableInterrupt(int nIRQ) {
	/* Datasheet says "All other bits are unaffected", and I'm counting on that. */
	unsigned int mask=1<<(nIRQ%32);

	if(nIRQ >=0 && nIRQ <=31) {
		pRegs->Disable1 = mask;
		enabled[0] &= ~mask;
	} else
	if(nIRQ >=32 && nIRQ <=63){
		pRegs->Disable2 = mask;
		enabled[1] &= ~mask;
	} else
	if(nIRQ >= 64 && nIRQ <= 71) {
		pRegs->DisableBasic = mask;
		enabled[2] &= ~mask;
	} else
		return -1;

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
