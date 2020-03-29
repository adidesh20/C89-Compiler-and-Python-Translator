	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-86
	sw	$ra,82($sp)
	sw	$fp,78($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 10
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable y


#lock $t0
#lock $t1
	li	$t0, 20
	 #varAddress = 16
	lw	$t1, 16($fp)		 #loading local (scope) variable: y
	nop
	addu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
#local var counter = 2
	sw	$v0, 20($fp)	#Storing variable x


	 #varAddress = 20
	lw	$v0, 20($fp)		 #loading local (scope) variable: x
	nop
	b f_function_end_1	 #return

#Current Scope Level = 0
f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,82($sp)
	nop
	lw	$fp,78($sp)
	nop
	addiu	$sp, $sp,86
	j	$ra
	nop

	.end	f

