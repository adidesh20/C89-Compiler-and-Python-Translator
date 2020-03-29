	.text

#----------FUNCTION log_and_fun----------
	.globl	log_and_fun
	.ent	log_and_fun
	.type	log_and_fun, @function
log_and_fun:
#ALLOCATING STACK
	addiu	$sp, $sp,-90
	sw	$ra,86($sp)
	sw	$fp,82($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 10
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable a


	li	$v0, 10
#local var counter = 2
	sw	$v0, 20($fp)	#Storing variable b


	li	$v0, 15
#local var counter = 3
	sw	$v0, 24($fp)	#Storing variable c


#lock $t0
#lock $t1
	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: a
	nop
	 #varAddress = 20
	lw	$t1, 20($fp)		 #loading local (scope) variable: b
	nop
	xor	$v0, $v0, $t1		 #comparison (==)
	sltiu	$v0, $v0, 1
#unlock $t1
#lock $t1
	 #varAddress = 24
	lw	$t0, 24($fp)		 #loading local (scope) variable: c
	nop
	 #varAddress = 20
	lw	$t1, 20($fp)		 #loading local (scope) variable: b
	nop
	slt	$t0, $t0, $t1		 #comparison (<)
#unlock $t1
	and	$v0, $v0, $t0		 #logical or
#unlock $t0
	b log_and_fun_function_end_1	 #return

#Current Scope Level = 0
log_and_fun_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,86($sp)
	nop
	lw	$fp,82($sp)
	nop
	addiu	$sp, $sp,90
	j	$ra
	nop

	.end	log_and_fun

