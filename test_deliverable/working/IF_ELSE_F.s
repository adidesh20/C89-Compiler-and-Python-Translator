	.text

#----------FUNCTION if_e_f----------
	.globl	if_e_f
	.ent	if_e_f
	.type	if_e_f, @function
if_e_f:
#ALLOCATING STACK
	addiu	$sp, $sp,-82
	sw	$ra,78($sp)
	sw	$fp,74($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 10
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable a


#lock $t0
	li	$t0, 0
	beq	$zero, $t0, else_0
	nop	
#unlock $t0

#Current Scope Level = 2
	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: a
	nop
	b if_e_f_function_end_1	 #return

#Current Scope Level = 1
	b	ifelse_end_0
	nop
else_0:

#Current Scope Level = 2
#lock $t0
#lock $t1
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: a
	nop
	li	$t1, 10
	addu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	b if_e_f_function_end_1	 #return

#Current Scope Level = 1
ifelse_end_0:

#Current Scope Level = 0
if_e_f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,78($sp)
	nop
	lw	$fp,74($sp)
	nop
	addiu	$sp, $sp,82
	j	$ra
	nop

	.end	if_e_f

