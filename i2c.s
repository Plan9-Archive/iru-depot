/* reads argv[0] (16(%ebp)) and prints it one char at a time */

	.text
	.globl main
	.type main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp

	movl	12(%ebp), %esi	# esi = argv[0]
	addl	$4, %esi	# esi = argv[n/4]
	movl	(%esi), %esi
	movl	$0, %ebx	# string index	

loop:
	pushl (%esi)
	call putchar	

	cmpb $0, (%esi)
	je end

	inc %ebx
	inc %esi

	jmp loop

end:
	pushl $0
	call exit
