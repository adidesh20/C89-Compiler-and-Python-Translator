	.text

#----------FUNCTION if_f----------
	.globl	if_f
	.ent	if_f
	.type	if_f, @function
if_f:
#ALLOCATING STACK
	addiu	$sp, $sp,-78
	sw	$ra,74($sp)
	sw	$fp,70($sp)
	move	$fp, $sp

#Current Scope Level = 1
#lock $t0
	li	$t0, 0
	beq	$zero, $t0, else_0
	nop	
#unlock $t0

#Current Scope Level = 2
	li	$v0, 2
	b if_f_function_end_1	 #return

#Current Scope Level = 1
	b	ifelse_end_0
	nop
else_0:
ifelse_end_0:

	li	$v0, 3
	b if_f_function_end_1	 #return

#Current Scope Level = 0
if_f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,74($sp)
	nop
	lw	$fp,70($sp)
	nop
	addiu	$sp, $sp,78
	j	$ra
	nop

	.end	if_f

