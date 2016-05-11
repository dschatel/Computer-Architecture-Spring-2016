.globl nCr
	.type	nCr, @function
nCr:
  # Your code for nCr should go here
  
	pushl	%ebp
	movl	%esp, %ebp
	subl	$32, %esp
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	Factorial
	movl	%eax, %ebx 			#Stores n!
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	Factorial
	movl	%eax, %esi 			#stores r!
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	subl	%edx, %eax
	movl	%eax, (%esp)
	call	Factorial
	jo .Overflow
	imull	%eax, %esi			#multiply r! by (n-r)!
	movl	%ebx, %eax
	sarl	$31, %edx
	idivl	%esi				#divide n! by r!(n-r)!
	leave
	ret
	


.globl Factorial
	.type	Factorial, @function
Factorial:
  # Your code for Factorial should go here
  
	pushl	%ebp
	movl	%esp, %ebp
	movl	$1, %eax
	jmp	.L2
.L3:
	imull	8(%ebp), %eax
	jo .Overflow
	decl	8(%ebp)
.L2:
	cmpl	$1, 8(%ebp)
	jae	.L3
	leave
	ret
	
 .Overflow:
	movl $0, %eax
	leave
	ret
