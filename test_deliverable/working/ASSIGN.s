	.text

#----------FUNCTION ass_fun----------
	.globl	ass_fun
	.ent	ass_fun
	.type	ass_fun, @function
ass_fun:
#ALLOCATING STACK
	addiu	$sp, $sp,-82
	sw	$ra,78($sp)
	sw	$fp,74($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 10
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable a


	li	$v0, 11
	sw	$v0, 16($fp)		#assigning value 11 to local variable a

	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: a
	nop
	b ass_fun_function_end_1	 #return

#Current Scope Level = 0
ass_fun_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,78($sp)
	nop
	lw	$fp,74($sp)
	nop
	addiu	$sp, $sp,82
	j	$ra
	nop

	.end	ass_fun

