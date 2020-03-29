	.text

#----------FUNCTION mul_ass_fun----------
	.globl	mul_ass_fun
	.ent	mul_ass_fun
	.type	mul_ass_fun, @function
mul_ass_fun:
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
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: a
	nop
	lw	$t1, 16($fp)		 #loading local variable a
	mult	$t0, $t1		 #multiplying by 10
	mflo	$v0
	sw	$v0, 16($fp)		 #storing a after increment
#unlock $t0
#unlock $t1

	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: a
	nop
	b mul_ass_fun_function_end_1	 #return

#Current Scope Level = 0
mul_ass_fun_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,78($sp)
	nop
	lw	$fp,74($sp)
	nop
	addiu	$sp, $sp,82
	j	$ra
	nop

	.end	mul_ass_fun

