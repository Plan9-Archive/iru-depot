/* 
 * Print 32bit value in SI
 * warning: 32bit call/return
 * we cannot use LOOP in the loops because we want CX as zero too
 */
TEXT printhex(SB), $0
	LWI(3, rCX)
_phex:
	LWI(0xFF, rAX)

	/* setup the mask to be ANDed */
	OPSIZE; PUSHL CX
_mask:
	OPSIZE; CMPL CX, $0
	OPSIZE; JE _maskok
	OPSIZE; SHLL $8, AX
	OPSIZE; DECL CX
	OPSIZE; JMP _mask

_maskok:
	OPSIZE; POPL CX

	OPSIZE; MOVL SI, BX
	OPSIZE; ANDL AX, BX

	/* shift right to get a byte */
	OPSIZE; PUSHL CX
_shift:
	OPSIZE; CMPL CX, $0
	OPSIZE; JE _shiftok
	OPSIZE; SHRL $8, BX
	OPSIZE; DECL CX
	OPSIZE; JMP _shift

_shiftok:
	OPSIZE; POPL CX
	
	/* a byte makes two hex chars */
	OPSIZE; PUSHL BX
	OPSIZE; ANDL $0xF0, BX
	OPSIZE; SHRL $4, BX
	OPSIZE; ADDL $hex(SB), BX			/* find char index */
	OPSIZE; ADSIZE; MOVB (BX), BX		/* load char */
	OPSIZE; ANDL $0xFF, BX
	OPSIZE; CALL putc(SB)
	OPSIZE; POPL BX

	OPSIZE; ANDL $0xF, BX
	OPSIZE; ADDL $hex(SB), BX			/* find char index */
	OPSIZE; ADSIZE; MOVB (BX), BX		/* load char */
	OPSIZE; ANDL $0xFF, BX
	OPSIZE; CALL putc(SB)
	
	OPSIZE; DECL CX
	OPSIZE; CMPL CX, $-1
	OPSIZE; JNE _phex
	
	OPSIZE; RET

TEXT hex(SB), $0
	BYTE $'0'; BYTE $'1'; BYTE $'2'; BYTE $'3'; 
	BYTE $'4'; BYTE $'5'; BYTE $'6'; BYTE $'7'; 
	BYTE $'8'; BYTE $'9'; BYTE $'A'; BYTE $'B'; 
	BYTE $'C'; BYTE $'D'; BYTE $'E'; BYTE $'F'; 
