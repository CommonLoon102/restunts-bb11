#include <dos.h>

// need these since we are referncing external symbols without an underscore
#define getvect _getvect
#define setvect _setvect
extern void _CType _setvect( int __interruptno, void interrupt( far *__isr )( ) );
extern void interrupt( far * _CType _getvect( int __interruptno ))( );

typedef void interrupt (far* voidinterruptfunctype)();
typedef void (far* voidfunctype)();

voidinterruptfunctype old_kb_intr_handler;
voidinterruptfunctype old_kb_intr_bios_handler;

extern void interrupt kb_intr_handler();
extern void interrupt kb_intr_bios_handler();
extern void add_exit_handler(voidfunctype exitfunc);

void interrupt my_kb_intr_handler() {
	kb_intr_handler(); // TODO
}

void interrupt my_kb_intr_bios_handler() {
	kb_intr_bios_handler();  // TODO
}

void kb_init_interrupt() {
	unsigned char irqmask;

	irqmask = inp(0x21);
	outp(0x21, irqmask | 0x3);

	old_kb_intr_handler = getvect(9);
	setvect(9, my_kb_intr_handler);

	old_kb_intr_bios_handler = getvect(22);
	setvect(22, my_kb_intr_bios_handler);

	outp(0x21, irqmask);
	add_exit_handler(kb_exit_handler);
}

void kb_exit_handler() {
	unsigned char irqmask;

	irqmask = inp(0x21);
	outp(0x21, irqmask | 0x3);
	setvect(9, old_kb_intr_handler);
	setvect(22, old_kb_intr_bios_handler);
	pokeb(0, 0x417, peekb(0, 0x417) & 0xf0);
	outp(0x21, irqmask);
}
