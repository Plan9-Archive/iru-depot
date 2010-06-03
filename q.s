/* draw a square of l*l 'A's 
 * Iruata Souza
 * Public Domain
 */

.text
	.globl main
main:
	movl $0, %ecx
	movl $2, %edx
line:
	cmpl %ecx, %edx
	je end
	pushl $65 
	call putchar
	inc %ecx
	jmp line
end:
	push $0x0a
	call putchar

	pushl $0 
	call exit
