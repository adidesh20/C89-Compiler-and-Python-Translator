	.text

#----------FUNCTION multiply----------
	.globl	multiply
	.ent	multiply
	.type	multiply, @function
multiply:
#ALLOCATING STACK
	addiu	$sp, $sp,-82
	sw	$ra,78($sp)
	sw	$fp,74($sp)
	move	$fp, $sp
	sw	$a0, 82($fp)	#storing param argument register
	sw	$a1, 86($fp)	#storing param argument register
	sw	$a2, 90($fp)	#storing param argument register
	sw	$a3, 94($fp)	#storing param argument register

#Current Scope Level = 1
	li	$v0, 0
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable acc


#lock $t0
#lock $t1
	 #varAddress = 0
	lw	$t0, 82($fp)		 #variable x is a function parameter
	nop
	li	$t1, 0
	slt	$t0, $t0, $t1		 #comparison (<)
#unlock $t1
	beq	$zero, $t0, else_0
	nop	
#unlock $t0

#Current Scope Level = 2
#lock $a0
	 #varAddress = 0
	lw	$v0, 82($fp)		 #variable x is a function parameter
	nop
	sub	$v0, $zero, $v0		 #negative operator 
	move	$a0, $2	 #move param to arg reg
#lock $a1
	 #varAddress = 0
	lw	$v0, 86($fp)		 #variable y is a function parameter
	nop
	move	$a1, $2	 #move param to arg reg
#unlock $a1
#unlock $a0
	jal	multiply	#Function called
	sub	$v0, $zero, $v0		 #negative operator 
	b multiply_function_end_1	 #return

#Current Scope Level = 1
	b	ifelse_end_0
	nop
else_0:
ifelse_end_0:

while_loop_0_begin:	#Begin while loop
#lock $t0
	 #varAddress = 0
	lw	$v0, 82($fp)		 #variable x is a function parameter
	nop
	li	$t0, 0
	slt	$v0, $t0, $v0		 #comparison (>)
#unlock $t0
	beq	$0, $2, end_loop_0
	nop

#Current Scope Level = 2
#lock $t0
#lock $t1
	 #varAddress = 0
	lw	$t0, 86($fp)		 #variable y is a function parameter
	nop
	lw	$t1, 16($fp)		 #loading local variable acc
