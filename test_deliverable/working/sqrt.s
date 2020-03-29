	.text

#----------FUNCTION bsqrt----------
	.globl	bsqrt
	.ent	bsqrt
	.type	bsqrt, @function
bsqrt:
#ALLOCATING STACK
	addiu	$sp, $sp,-86
	sw	$ra,82($sp)
	sw	$fp,78($sp)
	move	$fp, $sp
	sw	$a0, 86($fp)	#storing param argument register
	sw	$a1, 90($fp)	#storing param argument register
	sw	$a2, 94($fp)	#storing param argument register
	sw	$a3, 98($fp)	#storing param argument register

#Current Scope Level = 1
while_loop_0_begin:	#Begin while loop
#lock $t0
#lock $t1
#lock $t2
	 #varAddress = 0
	lw	$t1, 86($fp)		 #variable lo is a function parameter
	nop
	li	$t2, 1
	addu	$v0, $t1, $t2
#unlock $t1
#unlock $t2
	 #varAddress = 0
	lw	$t0, 90($fp)		 #variable hi is a function parameter
	nop
	slt	$v0, $v0, $t0		 #comparison (<)
#unlock $t0
	beq	$0, $2, end_loop_0
	nop

#Current Scope Level = 2
#lock $t0
#lock $t1
#lock $t2
	 #varAddress = 0
	lw	$t1, 86($fp)		 #variable lo is a function parameter
	nop
	 #varAddress = 0
	lw	$t2, 90($fp)		 #variable hi is a function parameter
	nop
	addu	$v0, $t1, $t2
#unlock $t1
#unlock $t2
	li	$t0, 1
	srlv	$2, $2, $8	# << operator
#unlock $t0
