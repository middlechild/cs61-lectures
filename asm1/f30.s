	.file	"f30.cc"
	.text
	.globl	_Z1flllllll
	.type	_Z1flllllll, @function
_Z1flllllll:
.LFB0:
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	movq	%rdx, -24(%rsp)
	movq	%rcx, -32(%rsp)
	movq	%r8, -40(%rsp)
	movq	%r9, -48(%rsp)
	movq	-16(%rsp), %rax
	leaq	(%rax,%rax), %rdx
	movq	-8(%rsp), %rax
	leaq	(%rdx,%rax), %rcx
	movq	-24(%rsp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	leaq	(%rcx,%rax), %rdx
	movq	-32(%rsp), %rax
	salq	$2, %rax
	leaq	(%rdx,%rax), %rcx
	movq	-40(%rsp), %rdx
	movq	%rdx, %rax
	salq	$2, %rax
	addq	%rdx, %rax
	addq	%rax, %rcx
	movq	-48(%rsp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	addq	%rax, %rcx
	movq	8(%rsp), %rdx
	movq	%rdx, %rax
	salq	$3, %rax
	subq	%rdx, %rax
	addq	%rcx, %rax
	ret
.LFE0:
	.size	_Z1flllllll, .-_Z1flllllll
	.ident	"GCC: (Ubuntu 7.3.0-16ubuntu3) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
