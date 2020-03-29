	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-82
	sw	$ra,78($sp)
	sw	$fp,74($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 0
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable x


	li	$v0, 1
	sw	$v0, 16($fp)		#assigning value 1 to local variable x

#lock $t0
#lock $t1
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: x
	nop
	 #varAddress = 16
	lw	$t1, 16($fp)		 #loading local (scope) variable: x
	nop
	addu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	sw	$v0, 16($fp)		#assigning value 4 to local variable x

	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: x
	nop
	b f_function_end_1	 #return

#Current Scope Level = 0
f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,78($sp)
	nop
	lw	$fp,74($sp)
	nop
	addiu	$sp, $sp,82
	j	$ra
	nop

	.end	f

