	.equ	O_RDONLY, 00000000
	#.equ	O_WRONLY, 00000001
	#.equ	O_RDWR,   00000002
	#.equ	O_CREAT,  00000100
	.equ	O_WRONLY, 00000101
	.equ	O_RDWR,   00000102

	.equ	SYS_EXIT,  1
	.equ	SYS_READ,  3
	.equ	SYS_WRITE, 4
	.equ	SYS_OPEN,  5
	.equ	SYS_CLOSE, 6

	.section .text

	.globl	os_open_rdwr
	.type	os_open_rdwr, @function
os_open_rdwr:
	pushl	%ebp
	movl	%esp, %ebp

	push	$0666
	pushl	$O_RDWR
	movl	8(%ebp), %eax
	pushl	%eax
	call	os_open

	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	os_open_rd
	.type	os_open_rd, @function
os_open_rd:
	pushl	%ebp
	movl	%esp, %ebp

	push	$0666
	pushl	$O_RDONLY
	movl	8(%ebp), %eax
	pushl	%eax
	call	os_open

	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	os_open_wr
	.type	os_open_wr, @function
os_open_wr:
	pushl	%ebp
	movl	%esp, %ebp

	push	$0666
	pushl	$O_WRONLY
	movl	8(%ebp), %eax
	pushl	%eax
	call	os_open

	movl	%ebp, %esp
	popl	%ebp
	ret

	.type	os_open_wr, @function
os_open:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx

	movl	$SYS_OPEN, %eax
	movl	8(%ebp), %ebx
	movl	12(%ebp), %ecx
	movl	16(%ebp), %edx
	int	$0x80

	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	os_close
	.type	os_close, @function
os_close:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx

	movl	$SYS_CLOSE, %eax
	movl	8(%ebp), %ebx
	int	$0x80

	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	os_read
	.type	os_read, @function
os_read:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx

	movl	$SYS_READ, %eax
	movl	8(%ebp), %ebx
	movl	12(%ebp), %ecx
	movl	16(%ebp), %edx
	int	$0x80

	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	os_write
	.type	os_write, @function
os_write:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx

	movl	$SYS_WRITE, %eax
	movl	8(%ebp), %ebx
	movl	12(%ebp), %ecx
	movl	16(%ebp), %edx
	int	$0x80

	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	os_exit
	.type	os_exit, @function
os_exit:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx

	movl	$SYS_EXIT, %eax
	movl	8(%ebp), %ebx
	int	$0x80

	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret

