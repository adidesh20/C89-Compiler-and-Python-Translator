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
	li	$v0, 0
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable x


	li	$v0, 0
#local var counter = 2
	sw	$v0, 20($fp)	#Storing variable y


	li	$v0, 1234
	sw	$v0, 16($fp)		#assigning value 1234 to local variable x

	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: x
	nop
	sw	$v0, 20($fp)		#assigning value 1234 to local variable y

	 #varAddress = 20
	lw	$v0, 20($fp)		 #loading local (scope) variable: y
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

