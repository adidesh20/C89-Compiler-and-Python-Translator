	.text

#----------FUNCTION all_ass_fun----------
	.globl	all_ass_fun
	.ent	all_ass_fun
	.type	all_ass_fun, @function
all_ass_fun:
#ALLOCATING STACK
	addiu	$sp, $sp,-94
	sw	$ra,90($sp)
	sw	$fp,86($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 10
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable a


	li	$v0, 15
#local var counter = 2
	sw	$v0, 20($fp)	#Storing variable b


	li	$v0, 3
#local var counter = 3
	sw	$v0, 24($fp)	#Storing variable c


	li	$v0, 100
#local var counter = 4
	sw	$v0, 28($fp)	#Storing variable d


#lock $t0
#lock $t1
	 #varAddress = 24
	lw	$t0, 24($fp)		 #loading local (scope) variable: c
	nop
	lw	$t1, 16($fp)		 #loading local variable a
	sub	$t0, $t1, $t0		 #decrementing by 3
	sw	$t0, 16($fp)		 #storing a after increment
#unlock $t0
#unlock $t1

#lock $t0
#lock $t1
	 #varAddress = 24
	lw	$t0, 24($fp)		 #loading local (scope) variable: c
	nop
	lw	$t1, 20($fp)		 #loading local variable b
	mult	$t0, $t1		 #multiplying by 3
	mflo	$v0
	sw	$v0, 20($fp)		 #storing b after increment
#unlock $t0
#unlock $t1

#lock $t0
#lock $t1
	 #varAddress = 28
	lw	$t0, 28($fp)		 #loading local (scope) variable: d
	nop
	lw	$t1, 24($fp)		 #loading local variable c
	addu	$t0, $t1, $t0		 #incrementing by 100
	sw	$t0, 24($fp)		 #storing c after increment
#unlock $t0
#unlock $t1

#lock $t0
#lock $t1
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: a
	nop
	lw	$t1, 28($fp)		 #loading local variable d
	div	$t1, $t0		 #dividing by 10
	mflo	$v0
	sw	$v0, 28($fp)		 #storing d after increment
#unlock $t0
#unlock $t1

#lock $t0
#lock $t1
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: a
	nop
#lock $t2
#lock $t3
	 #varAddress = 20
	lw	$t2, 20($fp)		 #loading local (scope) variable: b
	nop
#lock $t4
#lock $t5
	 #varAddress = 24
	lw	$t4, 24($fp)		 #loading local (scope) variable: c
	nop
	 #varAddress = 28
	lw	$t5, 28($fp)		 #loading local (scope) variable: d
	nop
	addu	$t3, $t4, $t5
#unlock $t4
#unlock $t5
	addu	$t1, $t2, $t3
#unlock $t2
#unlock $t3
	addu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	b all_ass_fun_function_end_1	 #return

#Current Scope Level = 0
all_ass_fun_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,90($sp)
	nop
	lw	$fp,86($sp)
	nop
	addiu	$sp, $sp,94
	j	$ra
	nop

	.end	all_ass_fun

