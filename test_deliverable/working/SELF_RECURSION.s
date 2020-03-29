	.text

#----------FUNCTION rec_fun----------
	.globl	rec_fun
	.ent	rec_fun
	.type	rec_fun, @function
rec_fun:
#ALLOCATING STACK
	addiu	$sp, $sp,-78
	sw	$ra,74($sp)
	sw	$fp,70($sp)
	move	$fp, $sp
	sw	$a0, 94($fp)	#storing param argument register
	sw	$a1, 98($fp)	#storing param argument register
	sw	$a2, 102($fp)	#storing param argument register
	sw	$a3, 106($fp)	#storing param argument register

#Current Scope Level = 1
#lock $t0
#lock $t1
	 #varAddress = 0
	lw	$t0, 78($fp)		 #variable x is a function parameter
	nop
	li	$t1, 1
	slt	$t0, $t1, $t0		 #comparison (>)
#unlock $t1
	beq	$zero, $t0, else_0
	nop	
#unlock $t0

#Current Scope Level = 2
#lock $t0
#lock $t1
	 #varAddress = 0
	lw	$t0, 78($fp)		 #variable x is a function parameter
	nop
	sw	$8, 20($fp)	#Storing temp register: 8
	sw	$9, 24($fp)	#Storing temp register: 9
#lock $a0
#lock $t2
#lock $t3
	 #varAddress = 0
	lw	$t2, 78($fp)		 #variable x is a function parameter
	nop
	li	$t3, 1
	subu	$t1, $t2, $t3
#unlock $t2
#unlock $t3
	move	$a0, $9	 #move param to arg reg
#unlock $a0
	jal	rec_fun	#Function called
	lw	$9, 24($fp)	#Loading temp register: 9
	nop
	lw	$8, 20($fp)	#Loading temp register: 8
	nop
	move	$t1, $v0		 #storing result of function call in temporary register
	mult	$t0, $t1
	mflo	$v0
#unlock $t0
#unlock $t1
	b rec_fun_function_end_1	 #return

#Current Scope Level = 1
	b	ifelse_end_0
	nop
else_0:

#Current Scope Level = 2
	 #varAddress = 0
	lw	$v0, 78($fp)		 #variable x is a function parameter
	nop
	b rec_fun_function_end_1	 #return

#Current Scope Level = 1
ifelse_end_0:

#Current Scope Level = 0
rec_fun_function_end_1:
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

	.end	rec_fun

