	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-78
	sw	$ra,74($sp)
	sw	$fp,70($sp)
	move	$fp, $sp
	sw	$a0, 78($fp)	#storing param argument register
	sw	$a1, 82($fp)	#storing param argument register
	sw	$a2, 86($fp)	#storing param argument register
	sw	$a3, 90($fp)	#storing param argument register

#Current Scope Level = 1
#lock $t0
	 #varAddress = 0
	lw	$v0, 78($fp)		 #variable x is a function parameter
	nop
	 #varAddress = 0
	lw	$t0, 82($fp)		 #variable y is a function parameter
	nop
	and	$v0, $v0, $t0		 #logical or
#unlock $t0
	b f_function_end_1	 #return

#Current Scope Level = 0
f_function_end_1:
#DEALLOCATING STACK
	lw	$4, 78($fp)	#loading param argument register
	nop
	lw	$5, 82($fp)	#loading param argument register
	nop
	lw	$6, 86($fp)	#loading param argument register
	nop
	lw	$7, 90($fp)	#loading param argument register
	nop
	move	$sp, $fp
	lw	$ra,74($sp)
	nop
	lw	$fp,70($sp)
	nop
	addiu	$sp, $sp,78
	j	$ra
	nop

	.end	f

