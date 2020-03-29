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


	li	$v0, 20
	sw	$v0, 16($fp)		#assigning value 20 to local variable x

while_loop_0_begin:	#Begin while loop
#lock $t0
	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: x
	nop
	li	$t0, 10
	slt	$v0, $t0, $v0		 #comparison (>)
#unlock $t0
	beq	$0, $2, end_loop_0
	nop

#Current Scope Level = 2
#lock $t0
#lock $t1
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: x
	nop
	li	$t1, 1
	subu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	sw	$v0, 16($fp)		#assigning value 18 to local variable x

#Current Scope Level = 1
	b	while_loop_0_begin
	nop
end_loop_0:	#End while loop

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

