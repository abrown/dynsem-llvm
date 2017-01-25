	.file	"dynsem.ll"
	.text
	.globl	match
	.align	16, 0x90
	.type	match,@function
match:                                  # @match
	.cfi_startproc
# BB#0:                                 # %entry
	xorl	%eax, %eax
	ret
.Ltmp0:
	.size	match, .Ltmp0-match
	.cfi_endproc

	.globl	clone
	.align	16, 0x90
	.type	clone,@function
clone:                                  # @clone
	.cfi_startproc
# BB#0:                                 # %entry
	xorl	%eax, %eax
	ret
.Ltmp1:
	.size	clone, .Ltmp1-clone
	.cfi_endproc

	.globl	replace
	.align	16, 0x90
	.type	replace,@function
replace:                                # @replace
	.cfi_startproc
# BB#0:                                 # %entry
	xorl	%eax, %eax
	ret
.Ltmp2:
	.size	replace, .Ltmp2-replace
	.cfi_endproc

	.globl	clean
	.align	16, 0x90
	.type	clean,@function
clean:                                  # @clean
	.cfi_startproc
# BB#0:                                 # %entry
	ret
.Ltmp3:
	.size	clean, .Ltmp3-clean
	.cfi_endproc

	.globl	binSearch
	.align	16, 0x90
	.type	binSearch,@function
binSearch:                              # @binSearch
	.cfi_startproc
# BB#0:                                 # %checkeq
	pushl	%ebx
.Ltmp8:
	.cfi_def_cfa_offset 8
	pushl	%edi
.Ltmp9:
	.cfi_def_cfa_offset 12
	pushl	%esi
.Ltmp10:
	.cfi_def_cfa_offset 16
	subl	$16, %esp
.Ltmp11:
	.cfi_def_cfa_offset 32
.Ltmp12:
	.cfi_offset %esi, -16
.Ltmp13:
	.cfi_offset %edi, -12
.Ltmp14:
	.cfi_offset %ebx, -8
	movl	40(%esp), %ecx
	movl	36(%esp), %edx
	movl	32(%esp), %edi
	movl	%edx, %esi
	shrl	%esi
	movl	(%edi,%esi,4), %ebx
	cmpl	%ecx, %ebx
	jne	.LBB4_2
# BB#1:
	movl	%esi, %eax
	jmp	.LBB4_7
.LBB4_2:                                # %checkn
	movl	$-1, %eax
	cmpl	$1, %edx
	je	.LBB4_7
# BB#3:                                 # %checkgtlt
	cmpl	%ebx, %ecx
	jbe	.LBB4_6
# BB#4:                                 # %gtcheck
	leal	4(%edi,%esi,4), %eax
	leal	-1(%esi), %edi
	subl	%edi, %edx
	movl	%ecx, 8(%esp)
	movl	%eax, (%esp)
	movl	%edx, 4(%esp)
	calll	binSearch
	cmpl	$-1, %eax
	je	.LBB4_7
# BB#5:                                 # %gtyes
	leal	1(%esi,%eax), %eax
	jmp	.LBB4_7
.LBB4_6:                                # %ltcheck
	decl	%esi
	movl	%ecx, 8(%esp)
	movl	%esi, 4(%esp)
	movl	%edi, (%esp)
	calll	binSearch
.LBB4_7:                                # %return
	addl	$16, %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	ret
.Ltmp15:
	.size	binSearch, .Ltmp15-binSearch
	.cfi_endproc

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	movl	$42, %eax
	ret
.Ltmp16:
	.size	main, .Ltmp16-main
	.cfi_endproc

	.type	enum.symbol,@object     # @enum.symbol
	.section	.rodata,"a",@progbits
	.globl	enum.symbol
enum.symbol:
	.byte	0                       # 0x0
	.size	enum.symbol, 1

	.type	enum.constant,@object   # @enum.constant
	.globl	enum.constant
enum.constant:
	.byte	1                       # 0x1
	.size	enum.constant, 1

	.type	enum.variable,@object   # @enum.variable
	.globl	enum.variable
enum.variable:
	.byte	2                       # 0x2
	.size	enum.variable, 1

	.type	enum.wildcard,@object   # @enum.wildcard
	.globl	enum.wildcard
enum.wildcard:
	.byte	3                       # 0x3
	.size	enum.wildcard, 1

	.type	enum.constructor,@object # @enum.constructor
	.globl	enum.constructor
enum.constructor:
	.byte	4                       # 0x4
	.size	enum.constructor, 1

	.type	enum.tuple,@object      # @enum.tuple
	.globl	enum.tuple
enum.tuple:
	.byte	5                       # 0x5
	.size	enum.tuple, 1


	.section	".note.GNU-stack","",@progbits
