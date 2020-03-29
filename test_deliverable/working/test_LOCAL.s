	.text

#----------FUNCTION ffff----------
	.globl	ffff
	.ent	ffff
	.type	ffff, @function
ffff:
#ALLOCATING STACK
	addiu	$sp, $sp,-82
	sw	$ra,78($sp)
	sw	$fp,74($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 0
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable x


	li	$v0, 10
	sw	$v0, 16($fp)		#assigning value 10 to local variable x

	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: x
	nop
	b ffff_function_end_1	 #return

#Current Scope Level = 0
ffff_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,78($sp)
	nop
	lw	$fp,74($sp)
	nop
	addiu	$sp, $sp,82
	j	$ra
	nop

	.end	ffff

