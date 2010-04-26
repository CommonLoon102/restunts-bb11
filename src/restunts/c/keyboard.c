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

extern unsigned char kbinput[];
extern unsigned int kb_intr_data;
extern unsigned int kb_intr_data2;
extern unsigned int kb_intr_data3;
extern unsigned int kb_intr_data4;
extern unsigned int kb_intr_data_array[];
extern unsigned char keymap1[];
extern unsigned char keymap2[];
extern unsigned char keymap3[];
extern unsigned char keymap4[];
extern unsigned char keymap5[];
extern unsigned int kblastinput;
void kb_exit_handler();

void interrupt my_kb_intr_handler() {
	unsigned char kbc, kbp;
	unsigned int kbval, kbdata;

	kbc = inp(0x60);
	kbp = inp(0x61);
	outp(0x61, kbp | 0x80);
	outp(0x61, kbp);

	if ((kbc & 0x80) == 0) {
		if (kbc >= 0x5a) 
			kbc = 0;
		kblastinput = kbc;
		kbinput[kbc] = 1;
		
		if (kbinput[0x38] == 1) {
			kbval = keymap5[kbc];
		} else
		if (kbinput[0x1D] == 1) {
			kbval = keymap4[kbc];
		} else
		if (kbinput[0x2A] == 1) {
			kbval = keymap2[kbc];
		} else
		if (kbinput[0x36] == 1) {
			kbval = keymap2[kbc];
		} else
		if (kbinput[0x3A] == 1) {
			kbval = keymap3[kbc];
		} else {
			kbval = keymap1[kbc];
		}
		
		if ((kbval & 0x80) != 0) {
			if (kbval >= 0x85)
				kbval &= 0x7F;
			kbval <<= 8;
		}

		kbdata = kb_intr_data;
		disable();
		kb_intr_data_array[kbdata / 2] = kbval;
		kbdata+=2;
		if (kbdata >= kb_intr_data3) // data3 = kb_buffer_pos
			kbdata = 0;
		kb_intr_data = kbdata;
		
		kbdata = kb_intr_data4;
		kbdata+=2;
		if (kbdata > kb_intr_data3) {
			kbdata = kb_intr_data3;
			kb_intr_data2 = kb_intr_data;
		}
		kb_intr_data4 = kbdata;
		enable();
		
	} else {
		kbc &= 0x7F;
		if (kbc >= 0x5a) // 0x5a = 90, keymaps are 90 bytes?
			kbc = 0;
		kbinput[kbc] = 0;
	}
	
	outp(0x20, 0x20);
	
}

/*void interrupt my_kb_intr_bios_handler() {
	unsigned char bioscall;
	asm {
		mov bioscall, ah
	}
}*/

void kb_init_interrupt() {
	unsigned char irqmask;

	irqmask = inp(0x21);
	outp(0x21, irqmask | 0x3);

	old_kb_intr_handler = getvect(9);
	setvect(9, my_kb_intr_handler);

	old_kb_intr_bios_handler = getvect(22);
	setvect(22, kb_intr_bios_handler);

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
