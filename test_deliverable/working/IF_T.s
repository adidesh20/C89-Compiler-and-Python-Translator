	.text

#----------FUNCTION if_t----------
	.globl	if_t
	.ent	if_t
	.type	if_t, @function
if_t:
#ALLOCATING STACK
	addiu	$sp, $sp,-78
	sw	$ra,74($sp)
	sw	$fp,70($sp)
	move	$fp, $sp

#Current Scope Level = 1
#lock $t0
	li	$t0, 1
	beq	$zero, $t0, else_0
	nop	
#unlock $t0

#Current Scope Level = 2
	li	$v0, 2
	b if_t_function_end_1	 #return

#Current Scope Level = 1
	b	ifelse_end_0
	nop
else_0:
ifelse_end_0:

	li	$v0, 3
	b if_t_function_end_1	 #return

#Current Scope Level = 0
if_t_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,74($sp)
	nop
	lw	$fp,70($sp)
	nop
	addiu	$sp, $sp,78
	j	$ra
	nop

	.end	if_t

