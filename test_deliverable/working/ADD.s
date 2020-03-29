	.text

#----------FUNCTION add_fun----------
	.globl	add_fun
	.ent	add_fun
	.type	add_fun, @function
add_fun:
#ALLOCATING STACK
	addiu	$sp, $sp,-86
	sw	$ra,82($sp)
	sw	$fp,78($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 10
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable a


	li	$v0, 5
#local var counter = 2
	sw	$v0, 20($fp)	#Storing variable b


#lock $t0
#lock $t1
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: a
	nop
	 #varAddress = 20
	lw	$t1, 20($fp)		 #loading local (scope) variable: b
	nop
	addu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	b add_fun_function_end_1	 #return

#Current Scope Level = 0
add_fun_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,82($sp)
	nop
	lw	$fp,78($sp)
	nop
	addiu	$sp, $sp,86
	j	$ra
	nop

	.end	add_fun

