	.text

#----------FUNCTION fib----------
	.globl	fib
	.ent	fib
	.type	fib, @function
fib:
#ALLOCATING STACK
	addiu	$sp, $sp,-78
	sw	$ra,74($sp)
	sw	$fp,70($sp)
	move	$fp, $sp
	sw	$a0, 78($fp)	#storing param argument register
	sw	$a1, 82($fp)	#storing param argument register
	sw	$a2, 86($fp)	#storing param argument register
	sw	$a3, 90($fp)	#storing param argument register

#Current Scope Level = 1
#lock $t0
#lock $t1
	 #varAddress = 0
	lw	$t0, 78($fp)		 #variable x is a function parameter
	nop
	li	$t1, 0
	slt	$t0, $t0, $t1		 #comparison (<=) less than check
	xori	$t0, $t0, 1	#equals check
#unlock $t1
	beq	$zero, $t0, else_0
	nop	
#unlock $t0

#Current Scope Level = 2
	li	$v0, 0
	b fib_function_end_1	 #return

#Current Scope Level = 1
	b	ifelse_end_0
	nop
else_0:
ifelse_end_0:

#lock $t0
#lock $t1
	 #varAddress = 0
	lw	$t0, 78($fp)		 #variable x is a function parameter
	nop
	li	$t1, 1
	xor	$t0, $t0, $t1		 #comparison (==)
	sltiu	$t0, $t0, 1
#unlock $t1
	beq	$zero, $t0, else_1
	nop	
#unlock $t0

#Current Scope Level = 2
	li	$v0, 1
	b fib_function_end_1	 #return

#Current Scope Level = 1
	b	ifelse_end_1
	nop
else_1:
ifelse_end_1:

#lock $t0
#lock $t1
	sw	$8, 20($fp)	#Storing temp register: 8
	sw	$9, 24($fp)	#Storing temp register: 9
#lock $a0
#lock $t2
#lock $t3
	 #varAddress = 0
	lw	$t2, 78($fp)		 #variable x is a function parameter
	nop
	li	$t3, 1
	subu	$t0, $t2, $t3
#unlock $t2
#unlock $t3
	move	$a0, $8	 #move param to arg reg
#unlock $a0
	jal	fib	#Function called
	lw	$9, 24($fp)	#Loading temp register: 9
	nop
	lw	$8, 20($fp)	#Loading temp register: 8
	nop
	move	$t0, $v0		 #storing result of function call in temporary register
	sw	$8, 20($fp)	#Storing temp register: 8
	sw	$9, 24($fp)	#Storing temp register: 9
#lock $a0
#lock $t2
#lock $t3
	 #varAddress = 0
	lw	$t2, 78($fp)		 #variable x is a function parameter
	nop
	li	$t3, 2
	subu	$t1, $t2, $t3
#unlock $t2
#unlock $t3
	move	$a0, $9	 #move param to arg reg
#unlock $a0
	jal	fib	#Function called
	lw	$9, 24($fp)	#Loading temp register: 9
	nop
	lw	$8, 20($fp)	#Loading temp register: 8
	nop
	move	$t1, $v0		 #storing result of function call in temporary register
	addu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	b fib_function_end_1	 #return

#Current Scope Level = 0
fib_function_end_1:
#DEALLOCATING STACK
	lw	$4, 78($fp)	#loading param argument register
	nop
	lw	$5, 82($fp)	#loading param argument register
	nop
	lw	$6, 86($fp)	#loading param argument register
	nop
	lw	$7, 90($fp)	#loading param argument register
	nop
	move	$sp, $fp
	lw	$ra,74($sp)
	nop
	lw	$fp,70($sp)
	nop
	addiu	$sp, $sp,78
	j	$ra
	nop

	.end	fib

