	.text

#----------FUNCTION div_ass_fun----------
	.globl	div_ass_fun
	.ent	div_ass_fun
	.type	div_ass_fun, @function
div_ass_fun:
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
#lock $t1
	li	$t0, 5
	lw	$t1, 16($fp)		 #loading local variable a
	div	$t1, $t0		 #dividing by 5
	mflo	$v0
	sw	$v0, 16($fp)		 #storing a after increment
#unlock $t0
#unlock $t1

	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: a
	nop
	b div_ass_fun_function_end_1	 #return

#Current Scope Level = 0
div_ass_fun_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,78($sp)
	nop
	lw	$fp,74($sp)
	nop
	addiu	$sp, $sp,82
	j	$ra
	nop

	.end	div_ass_fun

