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


	li	$v0, 1
	sw	$v0, 16($fp)		#assigning value 1 to local variable x

while_loop_0_begin:	#Begin while loop
	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: x
	nop
	beq	$0, $2, end_loop_0
	nop

#Current Scope Level = 2
	li	$v0, 0
	sw	$v0, 16($fp)		#assigning value 0 to local variable x

#Current Scope Level = 1
	b	while_loop_0_begin
	nop
end_loop_0:	#End while loop

	li	$v0, 19937
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

